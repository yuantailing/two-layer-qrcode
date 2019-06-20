# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import threading

from six.moves import queue


def synchronized(lock):
    def wrap(f):
        def newFunction(*args, **kw):
            lock.acquire()
            try:
                return f(*args, **kw)
            finally:
                lock.release()
        return newFunction
    return wrap


def multithreaded_tid(func, args_list, num_thread, logfunc=None):
    assert 0 < num_thread
    n = len(args_list)
    args_list = [args if isinstance(args, list) or isinstance(args, tuple)
                 else (args, ) for args in args_list]
    q = queue.Queue()
    for i in range(n):
        q.put(i)
    p = queue.Queue()

    def parallel_work(tid):
        while True:
            try:
                i = q.get(block=False)
                p.put(i)
            except queue.Empty as e:
                return
            func(*args_list[i], tid=tid)
    threads = [threading.Thread(target=parallel_work, args=(i, ))
               for i in range(num_thread)]
    for t in threads:
        t.start()
    if logfunc is not None:
        while 0 < n:
            try:
                logfunc(*args_list[p.get(block=True, timeout=1)])
                n -= 1
            except queue.Empty as e:
                pass
    for t in threads:
        t.join()


def multithreaded(func, args_list, num_thread, logfunc=None):
    def foo(*args, **kwargs):
        func(*args)
    return multithreaded_tid(foo, args_list, num_thread, logfunc=logfunc)
