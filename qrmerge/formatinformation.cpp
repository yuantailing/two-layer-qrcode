#include <algorithm>
#include <cassert>
#include <cstdio>
#include <functional>
#include <stack>
#include <string>
#include <thread>
#include <vector>

using namespace std;

static string const format_info_str[4][8] = {
    {"111011111000100", "111001011110011", "111110110101010", "111100010011101", "110011000101111", "110001100011000", "110110001000001", "110100101110110"}, {"101010000010010", "101000100100101", "101111001111100", "101101101001011", "100010111111001", "100000011001110", "100111110010111", "100101010100000"}, {"011010101011111", "011000001101000", "011111100110001", "011101000000110", "010010010110100", "010000110000011", "010111011011010", "010101111101101"}, {"001011010001001", "001001110111110", "001110011100111", "001100111010000", "000011101100010", "000001001010101", "000110100001100", "000100000111011"}
};

inline int merge_bit(int l1c, int l2c) {
    return l2c == -1 ? l1c : l2c;
}

void print(vector<int> a) {
    for (size_t i = 0; i < a.size(); i++)
        fprintf(stderr, "%d ", a[i]);
    fprintf(stderr, "\n");
};

struct Solution {
    int l1l[9];
    int l2l[10];
    int l1r[8];
    int l2r[9];
    int err1;
    int err2;
};

static void solve(size_t info1, size_t info2, Solution &solution) {
    string const &s1(format_info_str[0][info1]);
    string const &s2(format_info_str[0][info2]);
    int x1[15], x2[15];
    for (int i = 0; i < 15; i++) {
        x1[i] = s1[i] - '0';
        x2[i] = s2[i] - '0';
    }
    vector<int> const m1l = {x1[0], x1[1], x1[2], x1[3], x1[4], x1[5], 1, x1[6], x1[7]};
    vector<int> const m2l = {x2[0], x2[1], x2[2], x2[3], x2[4], x2[5], 1, x2[6], x2[7]};
    vector<int> const m1r = {x1[7], x1[8], x1[9], x1[10], x1[11], x1[12], x1[13], x1[14]};
    vector<int> const m2r = {x2[7], x2[8], x2[9], x2[10], x2[11], x2[12], x2[13], x2[14]};
    vector<tuple<vector<int>, vector<int>, int> > const m = {make_tuple(m1l, m2l, 1), make_tuple(m1r, m2r, -1)}; // tuple<m1, m2, side>
    struct Option {
        vector<int> l1;
        vector<int> l2;
        int err1;
        int err2;
        int sidetrans;
        int transparent;
    };
    struct Segment {
        size_t block_id;
        size_t lo;
        vector<int> m1;
        vector<int> m2;
        int side;
        vector<Option> options;
        vector<int> l1;
        vector<int> l2;
    };
    vector<Segment> segments;
    for (size_t block_id = 0; block_id < m.size(); block_id++) {
        vector<int> const &m1 = get<0>(m[block_id]);
        vector<int> const &m2 = get<1>(m[block_id]);
        int side = get<2>(m[block_id]);
        assert(m1.size() == m2.size());
        size_t n = m1.size();
        vector<int> seg1, seg2;
        size_t i;
        auto append = [&]() {
            if (!seg1.empty()) {
                Segment seg;
                seg.block_id = block_id;
                seg.lo = i - seg1.size();
                seg.m1 = seg1;
                seg.m2 = seg2;
                if (side == -1 && seg.lo == 0)
                    seg.side = -1;
                else if (side == 1 && i == n)
                    seg.side = 1;
                else
                    seg.side = 0;
                segments.push_back(seg);
                seg1.clear();
                seg2.clear();
            }
        };
        for (i = 0; i < n; i++) {
            if (block_id == 0 && i == 6) { // ignore timing pattern
                append();
            } else {
                seg1.push_back(m1[i]);
                seg2.push_back(m2[i]);
            }
        }
        append();
    }
    typedef decltype(Segment::side) Arg;
    auto travels = [](vector<int> const &m1, vector<int> const &m2, Arg arg, function<void(vector<int> const &, vector<int> const &, Arg)> visit) {
        assert(m1.size() == m2.size());
        int n = m1.size();
        vector<int> l1(n, 2), l2(n + 1, 2);
        stack<int> s({-2});
        while (!s.empty()) {
            s.top()++;
            int t = s.top();
            int cur = (int)s.size() - 1;
            if (t > 1) {
                s.pop();
                continue;
            }
            if (cur % 2 == 0) {
                l2[cur / 2] = t;
            } else {
                l1[cur / 2] = t;
            }
            if (cur != n * 2) {
                s.push(cur % 2 == 0 ? -1 : -2);
            } else {
                visit(l1, l2, arg);
            }
        }
    };
    for (Segment &seg: segments) {
        size_t n = seg.m1.size();
        auto visit = [&](vector<int> const &l1, vector<int> const &l2, int side) {
            int err1 = 0, err2 = 0;
            for (size_t i = 0; i < n; i++) {
                if (merge_bit(l1[i], l2[i]) != seg.m1[i])
                    err1++;
                if (merge_bit(l1[i], l2[i + 1]) != seg.m2[i])
                    err2++;
            }
            int sidetrans = (int)(side == -1 && l2[0] == -1) + (int)(side == 1 && l2[n] == -1);
            int transparent = count_if(l2.begin(), l2.end(), [](int c) { return c == -1; });
            bool found = false;
            for (Option &opt: seg.options) {
                if (err1 == opt.err1 && err2 == opt.err2) {
                    if (make_pair(opt.sidetrans, opt.transparent) < make_pair(sidetrans, transparent)) {
                        opt.sidetrans = sidetrans;
                        opt.transparent = transparent;
                        opt.l1 = l1;
                        opt.l2 = l2;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                seg.options.push_back(Option({l1, l2, err1, err2, sidetrans, transparent}));
            }
        };
        travels(seg.m1, seg.m2, seg.side, visit);
        assert(seg.options.size() > 0);
    }
    struct {
        bool is_null;
        vector<size_t> indices;
        int err1;
        int err2;
        int sidetrans;
        int transparent;
    } best;
    best.is_null = true;
    vector<size_t> indices(segments.size(), 0);
    while (1) {
        int err1 = 0, err2 = 0;
        int sidetrans = 0;
        int transparent = 0;
        for (size_t i = 0; i < segments.size(); i++) {
            Option const &opt = segments[i].options[indices[i]];
            err1 += opt.err1;
            err2 += opt.err2;
            sidetrans += opt.sidetrans;
            transparent += opt.transparent;
        }
        if (best.is_null ||
            make_tuple(max<int>(err1, err2), err1 + err2, -sidetrans, -transparent) < make_tuple(max<int>(best.err1, best.err2), best.err1 + best.err2, -best.sidetrans, -best.transparent)) {
            best.is_null = false;
            best.indices = indices;
            best.err1 = err1;
            best.err2 = err2;
            best.sidetrans = sidetrans;
            best.transparent = transparent;
        }
        size_t i = 0;
        for (i = 0; i < indices.size(); i++) {
            if (indices[i] + 1 < segments[i].options.size()) {
                indices[i]++;
                break;
            } else {
                indices[i] = 0;
            }
        }
        if (i == indices.size())
            break;
    }
    for (size_t i = 0; i < segments.size(); i++) {
        segments[i].l1 = segments[i].options[best.indices[i]].l1;
        segments[i].l2 = segments[i].options[best.indices[i]].l2;
    }
    vector<tuple<vector<int>, vector<int> > > l;
    for (size_t i = 0; i < m.size(); i++) {
        vector<int> l1 = get<0>(m[i]);
        vector<int> l2(l1.size() + 1, -1);
        l.emplace_back(l1, l2);
    }
    for (Segment const &seg: segments) {
        vector<int> &l1 = get<0>(l[seg.block_id]);
        vector<int> &l2 = get<1>(l[seg.block_id]);
        for (size_t i = 0; i < seg.l1.size(); i++)
            l1[seg.lo + i] = seg.l1[i];
        for (size_t i = 0; i < seg.l2.size(); i++)
            l2[seg.lo + i] = seg.l2[i];
    }
        int err1 = 0, err2 = 0;
    for (size_t i = 0; i < m.size(); i++) {
        vector<int> const &m1 = get<0>(m[i]);
        vector<int> const &m2 = get<1>(m[i]);
        vector<int> const &l1 = get<0>(l[i]);
        vector<int> const &l2 = get<1>(l[i]);
        size_t n = l1.size();
        for (size_t j = 0; j < n; j++) {
            if (i == 0 && j == 6) // ignore timing pattern
                continue;
            if (merge_bit(l1[j], l2[j]) != m1[j])
                err1++;
            if (merge_bit(l1[j], l2[j + 1]) != m2[j])
                err2++;
        }
    }
    // fprintf(stderr, "info1, info2 = %zu, %zu, err1, err2 = %d, %d\n", info1, info2, err1, err2);
    assert(best.err1 == err1 && best.err2 == err2);
    assert(2 == l.size());
    assert(9 == get<0>(l[0]).size());
    assert(8 == get<0>(l[1]).size());
    for (size_t i = 0; i < 9; i++)
        solution.l1l[i] = get<0>(l[0])[i];
    for (size_t i = 0; i < 10; i++)
        solution.l2l[i] = get<1>(l[0])[i];
    for (size_t i = 0; i < 8; i++)
        solution.l1r[i] = get<0>(l[1])[i];
    for (size_t i = 0; i < 9; i++)
        solution.l2r[i] = get<1>(l[1])[i];
    solution.err1 = err1;
    solution.err2 = err2;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s output_filename num_thread\n", argv[0]);
        return 1;
    }
    constexpr int format_n = (int)(sizeof(format_info_str) / sizeof(**format_info_str));
    vector<Solution> solutions(format_n * format_n);
    int num_thread = atoi(argv[2]);
    vector<thread> workers;
    auto run = [&solutions, num_thread](int tid)->void {
        for (size_t info1 = 0; info1 < format_n; info1++) {
            for (size_t info2 = 0; info2 < format_n; info2++) {
                int idx = info1 * format_n + info2;
                if (idx % num_thread != tid)
                    continue;
                solve(info1, info2, solutions[idx]);
            }
        }
    };
    for (int i = 0; i < num_thread; i++)
        workers.emplace_back(run, i);
    for (thread &t: workers)
        t.join();
    FILE *fp = fopen(argv[1], "wb");
    if (!fp) {
        fprintf(stderr, "fopen failed");
        return 1;
    }
    for (Solution const &solution: solutions) {
        auto write = [fp](int x)->void {
            char c = (char)x;
            fwrite(&c, sizeof(c), 1, fp);
        };
        for (int x: solution.l1l)
            write(x);
        for (int x: solution.l2l)
            write(x);
        for (int x: solution.l1r)
            write(x);
        for (int x: solution.l2r)
            write(x);
        write(solution.err1);
        write(solution.err2);
    }
    fclose(fp);
    return 0;
}
