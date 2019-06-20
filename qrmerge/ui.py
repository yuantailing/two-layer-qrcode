# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import fractions
import io
import multiprocessing
import numpy as np
import os
import subprocess
import threading
import time

from . import qrhelper
from PIL import Image
from pythonapi import common_tools
from six.moves import queue


@common_tools.synchronized(threading.Lock())
def _make_exe():
    p = subprocess.Popen(['make'], stdout=subprocess.PIPE, cwd=os.path.dirname(__file__))
    _, _ = p.communicate()
    assert 0 == p.wait(), p.returncode


def encode(s1, s2, mode, version, level1, level2, image=None, timeout=100, debug=False, *, different_mask=0, do_reduce=1, merge_bound_thresh=300, lamb=.1, seed=0):
    mode1, encode1 = qrhelper.mode_and_encode(s1)
    mode2, encode2 = qrhelper.mode_and_encode(s2)
    v1 = qrhelper.minimal_version(mode1, level1, encode1)
    v2 = qrhelper.minimal_version(mode2, level2, encode2)
    ver = max(v1, v2, int(version))
    code1 = qrhelper.indicators(ver, mode1, len(s1)) + encode1
    code2 = qrhelper.indicators(ver, mode2, len(s2)) + encode2

    n = 17 + ver * 4
    groups1, ecc_num1 = qrhelper.group_and_ecc(ver, level1)
    groups2, ecc_num2 = qrhelper.group_and_ecc(ver, level2)

    _make_exe()

    if mode == 'art':
        if image:
            input = io.BytesIO(image.encode('latin1'))
            img = Image.open(input)
        else:
            img = Image.open('qrmerge/logo.png')
        img = img.convert('L')
        img = img.resize((n, n), resample=Image.BILINEAR)
        img = np.array(img.getdata()).reshape((n, n))
        img = np.array(img <= 127, dtype=np.int).tolist()
    start_time = time.time()

    class Result():
        pass

    def encode_once(maskid1, maskid2):
        input = []
        input.append('{:d} {:s} {:s} {:d} {:d} {:d} {:d} {:d} {:f} {:d}'.format(ver, level1, level2, maskid1, maskid2, int(mode == 'art'), do_reduce, merge_bound_thresh, lamb, seed))
        input.append(code1)
        input.append(code2)
        if mode == 'art':
            for line in img:
                input.append(' '.join([str(b) for b in line]))
        res = Result()
        res.maskid1 = maskid1
        res.maskid2 = maskid2

        isTimeout = [False]

        def timeout_target(p):
            isTimeout[0] = True
            p.kill()
        if debug:
            stderr = None
        else:
            stderr = open(os.devnull, 'w')
        p = subprocess.Popen([os.path.join(os.path.abspath(os.path.dirname(__file__)), 'main.exe')], cwd=os.path.dirname(__file__), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=stderr)
        timer = threading.Timer(timeout, timeout_target, [p])
        timer.start()
        output, errout = p.communicate('\n'.join(input).encode())
        timer.cancel()
        if isTimeout[0]:
            res.error = 1
            res.msg = 'time out'
            return res
        assert 0 == p.wait(), p.returncode
        if stderr is not None:
            stderr.close()
        output = output.decode()

        output = output.split('\n', 3)
        retcode = int(output[0].split()[0])
        if retcode != 0:
            res.error = 1
            res.msg = 'mask id not match'
            return res
        res.error = 0
        error_format_bits_1, error_format_bits_2, max_merge_bound, cpp_time_cost = output[0].split()[1:]
        res.error_format_bits = [error_format_bits_1, error_format_bits_2]
        res.max_merge_bound = int(max_merge_bound)
        res.cpp_time_cost = float(cpp_time_cost)
        res.err1 = [int(x) for x in output[1].split()]
        res.err2 = [int(x) for x in output[2].split()]
        res.matrixes = output[3]
        return res

    q = queue.Queue()

    def target(maskid1, maskid2):
        res = encode_once(maskid1, maskid2)
        q.put(res)
    if different_mask:
        tasks = [(maskid1, maskid2) for maskid1 in range(8) for maskid2 in range(8)]
    else:
        tasks = [(maskid, maskid) for maskid in range(8)]
    common_tools.multithreaded(target, tasks, num_thread=min(8, len(tasks), multiprocessing.cpu_count()))
    results = []
    while not q.empty():
        res = q.get()
        results.append(res)
    assert len(results) == len(tasks)
    results.sort(key=lambda res: (res.maskid1, res.maskid2))
    max_merge_bounds = []
    cpp_time_costs = []
    for res in results:
        if not res.error:
            max_merge_bounds.append(res.max_merge_bound)
            cpp_time_costs.append(res.cpp_time_cost)
        if debug:
            if res.error:
                print('maskid', res.maskid1, res.maskid2, res.msg)
            else:
                print('maskid', res.maskid1, res.maskid2, '#misencoded', res.err1, res.err2)

    rank = list(range(len(results)))

    def cost(result_id):
        res = results[result_id]
        if res.error:
            return (1,)

        def recovery_ratio(groups, ecc_num, err):
            x = fractions.Fraction(1, 1)
            for a in err[:groups[0]]:
                x = min(x, fractions.Fraction(ecc_num // 2 - a, groups[1] + ecc_num))
            for a in err[groups[0]:]:
                x = min(x, fractions.Fraction(ecc_num // 2 - a, groups[3] + ecc_num))
            return x
        return (0, -min(recovery_ratio(groups1, ecc_num1, res.err1), recovery_ratio(groups2, ecc_num2, res.err2)), max(res.error_format_bits), 0 if res.maskid1 == res.maskid2 else 1, sorted(res.err1 + res.err2, reverse=True))
    rank.sort(key=cost)
    res = results[rank[0]]
    time_cost = time.time() - start_time
    if res.error:
        return {
            'error': 1,
            'msg': 'solver timeout',
            'time_cost': time_cost,
        }

    block_length1 = [x + ecc_num1 for x in [groups1[1]] * groups1[0] + [groups1[3]] * groups1[2]]
    block_length2 = [x + ecc_num2 for x in [groups2[1]] * groups2[0] + [groups2[3]] * groups2[2]]
    matrixes = res.matrixes.split('\n')
    m1 = [[int(c) for c in line.split()] for line in matrixes[0:n]]
    m2 = [[int(c) for c in line.split()] for line in matrixes[1 * n:2 * n]]
    l1 = [[int(c) for c in line.split()] for line in matrixes[2 * n:3 * n]]
    l2 = [[int(c) for c in line.split()] for line in matrixes[3 * n:4 * n]]
    o1 = [[int(c) for c in line.split()] for line in matrixes[4 * n:5 * n]]
    o2 = [[int(c) for c in line.split()] for line in matrixes[5 * n:6 * n]]

    return {
        'error': 0,
        'version': ver,
        'level': [level1, level2],
        'mask_id': [res.maskid1, res.maskid2],
        'ecc_num': [ecc_num1, ecc_num2],
        'block_length': [block_length1, block_length2],
        'timeout': timeout,
        'is_art': mode == 'art',
        'different_mask': different_mask,
        'do_reduce': do_reduce,
        'max_merge_bounds': max_merge_bounds,
        'seed': seed,
        'cpp_time_costs': cpp_time_costs,
        'time_cost': time_cost,
        'string_left': s1,
        'string_right': s2,
        'reference_left': m1,
        'reference_right': m2,
        'layer_down': l1,
        'layer_up': l2,
        'qrcode_left': o1,
        'qrcode_right': o2,
        'error_left': res.err1,
        'error_right': res.err2,
        'error_format_bits': res.error_format_bits,
    }


def draw_qrcode(fp, qrmatrix, bg_color='white'):
    def draw_a_unit(p, x, y, ul, c):
        for i in range(ul):
            for j in range(ul):
                p.putpixel((x + i, y + j), c)
    unit_len = 1
    x = y = 2 * unit_len
    pic = Image.new('RGBA', [(len(qrmatrix) + 4) * unit_len] * 2, bg_color)
    for line in qrmatrix:
        for module in line:
            if module is 0 or module is False:
                draw_a_unit(pic, x, y, unit_len, (255, 255, 255, 255))
            elif module is 1 or module is True:
                draw_a_unit(pic, x, y, unit_len, (0, 0, 0, 255))
            elif module is -1:
                draw_a_unit(pic, x, y, unit_len, bg_color)
            else:
                draw_a_unit(pic, x, y, unit_len, (0, 0, 255, 255))
            x += unit_len
        x, y = 2 * unit_len, y + unit_len
    pic.save(fp, format='png')
