#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <map>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

int const MAXN = 180;
int const MAXSOLOMON = MAXN * MAXN / 8;
static_assert(MAXN > 17 + 4 * 40, "MAXN is too small.");

int n;
int group_list1[4];
int group_list2[4];
int ecc_num_per_block1;
int ecc_num_per_block2;
int is_art;
int do_reduce;
long long merge_bound_thresh;
double lambda;
unsigned seed;
int mat1[MAXN][MAXN];
int mat2[MAXN][MAXN];
int sol[MAXN][MAXN];
int img[MAXN][MAXN];
clock_t clock_read_start;
clock_t clock_gen_start;

pair<int, int> error_format_bits;
int layer1[MAXN][MAXN];
int layer2[MAXN][MAXN];
int re1[MAXN][MAXN];
int re2[MAXN][MAXN];


namespace gen {
    int ver, is_art;
    int n;  // n == 17 + 4 * ver

    static int const required_bytes[40][4] = {
        {19, 16, 13, 9}, {34, 28, 22, 16}, {55, 44, 34, 26}, {80, 64, 48, 36}, {108, 86, 62, 46}, {136, 108, 76, 60}, {156, 124, 88, 66}, {194, 154, 110, 86}, {232, 182, 132, 100}, {274, 216, 154, 122}, {324, 254, 180, 140}, {370, 290, 206, 158}, {428, 334, 244, 180}, {461, 365, 261, 197}, {523, 415, 295, 223}, {589, 453, 325, 253}, {647, 507, 367, 283}, {721, 563, 397, 313}, {795, 627, 445, 341}, {861, 669, 485, 385}, {932, 714, 512, 406}, {1006, 782, 568, 442}, {1094, 860, 614, 464}, {1174, 914, 664, 514}, {1276, 1000, 718, 538}, {1370, 1062, 754, 596}, {1468, 1128, 808, 628}, {1531, 1193, 871, 661}, {1631, 1267, 911, 701}, {1735, 1373, 985, 745}, {1843, 1455, 1033, 793}, {1955, 1541, 1115, 845}, {2071, 1631, 1171, 901}, {2191, 1725, 1231, 961}, {2306, 1812, 1286, 986}, {2434, 1914, 1354, 1054}, {2566, 1992, 1426, 1096}, {2702, 2102, 1502, 1142}, {2812, 2216, 1582, 1222}, {2956, 2334, 1666, 1276}
    };
    static initializer_list<int> const grouping_list[40][4] = {
        {{1, 19, 0, 0}, {1, 16, 0, 0}, {1, 13, 0, 0}, {1, 9, 0, 0}}, {{1, 34, 0, 0}, {1, 28, 0, 0}, {1, 22, 0, 0}, {1, 16, 0, 0}}, {{1, 55, 0, 0}, {1, 44, 0, 0}, {2, 17, 0, 0}, {2, 13, 0, 0}}, {{1, 80, 0, 0}, {2, 32, 0, 0}, {2, 24, 0, 0}, {4, 9, 0, 0}}, {{1, 108, 0, 0}, {2, 43, 0, 0}, {2, 15, 2, 16}, {2, 11, 2, 12}}, {{2, 68, 0, 0}, {4, 27, 0, 0}, {4, 19, 0, 0}, {4, 15, 0, 0}}, {{2, 78, 0, 0}, {4, 31, 0, 0}, {2, 14, 4, 15}, {4, 13, 1, 14}}, {{2, 97, 0, 0}, {2, 38, 2, 39}, {4, 18, 2, 19}, {4, 14, 2, 15}}, {{2, 116, 0, 0}, {3, 36, 2, 37}, {4, 16, 4, 17}, {4, 12, 4, 13}}, {{2, 68, 2, 69}, {4, 43, 1, 44}, {6, 19, 2, 20}, {6, 15, 2, 16}}, {{4, 81, 0, 0}, {1, 50, 4, 51}, {4, 22, 4, 23}, {3, 12, 8, 13}}, {{2, 92, 2, 93}, {6, 36, 2, 37}, {4, 20, 6, 21}, {7, 14, 4, 15}}, {{4, 107, 0, 0}, {8, 37, 1, 38}, {8, 20, 4, 21}, {12, 11, 4, 12}}, {{3, 115, 1, 116}, {4, 40, 5, 41}, {11, 16, 5, 17}, {11, 12, 5, 13}}, {{5, 87, 1, 88}, {5, 41, 5, 42}, {5, 24, 7, 25}, {11, 12, 7, 13}}, {{5, 98, 1, 99}, {7, 45, 3, 46}, {15, 19, 2, 20}, {3, 15, 13, 16}}, {{1, 107, 5, 108}, {10, 46, 1, 47}, {1, 22, 15, 23}, {2, 14, 17, 15}}, {{5, 120, 1, 121}, {9, 43, 4, 44}, {17, 22, 1, 23}, {2, 14, 19, 15}}, {{3, 113, 4, 114}, {3, 44, 11, 45}, {17, 21, 4, 22}, {9, 13, 16, 14}}, {{3, 107, 5, 108}, {3, 41, 13, 42}, {15, 24, 5, 25}, {15, 15, 10, 16}}, {{4, 116, 4, 117}, {17, 42, 0, 0}, {17, 22, 6, 23}, {19, 16, 6, 17}}, {{2, 111, 7, 112}, {17, 46, 0, 0}, {7, 24, 16, 25}, {34, 13, 0, 0}}, {{4, 121, 5, 122}, {4, 47, 14, 48}, {11, 24, 14, 25}, {16, 15, 14, 16}}, {{6, 117, 4, 118}, {6, 45, 14, 46}, {11, 24, 16, 25}, {30, 16, 2, 17}}, {{8, 106, 4, 107}, {8, 47, 13, 48}, {7, 24, 22, 25}, {22, 15, 13, 16}}, {{10, 114, 2, 115}, {19, 46, 4, 47}, {28, 22, 6, 23}, {33, 16, 4, 17}}, {{8, 122, 4, 123}, {22, 45, 3, 46}, {8, 23, 26, 24}, {12, 15, 28, 16}}, {{3, 117, 10, 118}, {3, 45, 23, 46}, {4, 24, 31, 25}, {11, 15, 31, 16}}, {{7, 116, 7, 117}, {21, 45, 7, 46}, {1, 23, 37, 24}, {19, 15, 26, 16}}, {{5, 115, 10, 116}, {19, 47, 10, 48}, {15, 24, 25, 25}, {23, 15, 25, 16}}, {{13, 115, 3, 116}, {2, 46, 29, 47}, {42, 24, 1, 25}, {23, 15, 28, 16}}, {{17, 115, 0, 0}, {10, 46, 23, 47}, {10, 24, 35, 25}, {19, 15, 35, 16}}, {{17, 115, 1, 116}, {14, 46, 21, 47}, {29, 24, 19, 25}, {11, 15, 46, 16}}, {{13, 115, 6, 116}, {14, 46, 23, 47}, {44, 24, 7, 25}, {59, 16, 1, 17}}, {{12, 121, 7, 122}, {12, 47, 26, 48}, {39, 24, 14, 25}, {22, 15, 41, 16}}, {{6, 121, 14, 122}, {6, 47, 34, 48}, {46, 24, 10, 25}, {2, 15, 64, 16}}, {{17, 122, 4, 123}, {29, 46, 14, 47}, {49, 24, 10, 25}, {24, 15, 46, 16}}, {{4, 122, 18, 123}, {13, 46, 32, 47}, {48, 24, 14, 25}, {42, 15, 32, 16}}, {{20, 117, 4, 118}, {40, 47, 7, 48}, {43, 24, 22, 25}, {10, 15, 67, 16}}, {{19, 118, 6, 119}, {18, 47, 31, 48}, {34, 24, 34, 25}, {20, 15, 61, 16}}
    };
    static int const ecc_num_per_block[40][4] = {
    {7, 10, 13, 17}, {10, 16, 22, 28}, {15, 26, 18, 22}, {20, 18, 26, 16}, {26, 24, 18, 22}, {18, 16, 24, 28}, {20, 18, 18, 26}, {24, 22, 22, 26}, {30, 22, 20, 24}, {18, 26, 24, 28}, {20, 30, 28, 24}, {24, 22, 26, 28}, {26, 22, 24, 22}, {30, 24, 20, 24}, {22, 24, 30, 24}, {24, 28, 24, 30}, {28, 28, 28, 28}, {30, 26, 28, 28}, {28, 26, 26, 26}, {28, 26, 30, 28}, {28, 26, 28, 30}, {28, 28, 30, 24}, {30, 28, 30, 30}, {30, 28, 30, 30}, {26, 28, 30, 30}, {28, 28, 28, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}, {30, 28, 30, 30}
    };
    static map<int, vector<int> const > GP_list = {
        {7,  {0, 87, 229, 146, 149, 238, 102, 21}},
        {10, {0, 251, 67, 46, 61, 118, 70, 64, 94, 32, 45}},
        {13, {0, 74, 152, 176, 100, 86, 100, 106, 104, 130, 218, 206, 140, 78}},
        {15, {0, 8, 183, 61, 91, 202, 37, 51, 58, 58, 237, 140, 124, 5, 99, 105}},
        {16, {0, 120, 104, 107, 109, 102, 161, 76, 3, 91, 191, 147, 169, 182, 194, 225, 120}},
        {17, {0, 43, 139, 206, 78, 43, 239, 123, 206, 214, 147, 24, 99, 150, 39, 243, 163, 136}},
        {18, {0, 215, 234, 158, 94, 184, 97, 118, 170, 79, 187, 152, 148, 252, 179, 5, 98, 96, 153}},
        {20, {0, 17, 60, 79, 50, 61, 163, 26, 187, 202, 180, 221, 225, 83, 239, 156, 164, 212, 212, 188, 190}},
        {22, {0, 210, 171, 247, 242, 93, 230, 14, 109, 221, 53, 200, 74, 8, 172, 98, 80, 219, 134, 160, 105, 165, 231}},
        {24, {0, 229, 121, 135, 48, 211, 117, 251, 126, 159, 180, 169, 152, 192, 226, 228, 218, 111, 0, 117, 232, 87, 96, 227, 21}},
        {26, {0, 173, 125, 158, 2, 103, 182, 118, 17, 145, 201, 111, 28, 165, 53, 161, 21, 245, 142, 13, 102, 48, 227, 153, 145, 218, 70}},
        {28, {0, 168, 223, 200, 104, 224, 234, 108, 180, 110, 190, 195, 147, 205, 27, 232, 201, 21, 43, 245, 87, 42, 195, 212, 119, 242, 37, 9, 123}},
        {30, {0, 41, 173, 145, 152, 216, 31, 179, 182, 50, 48, 110, 86, 239, 96, 222, 125, 42, 173, 226, 193, 224, 130, 156, 37, 251, 216, 238, 40, 192, 180}},
    };
    static int const gf_po2[257] = {
        1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1
    };
    static int const gf_log[256] = {
        -1, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175
    };
    static int const required_remainder_bits[40] = {0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0};
    static vector<int> const alig_location[39] = {
        {6, 18}, {6, 22}, {6, 26}, {6, 30}, {6, 34}, {6, 22, 38}, {6, 24, 42}, {6, 26, 46}, {6, 28, 50}, {6, 30, 54}, {6, 32, 58}, {6, 34, 62}, {6, 26, 46, 66}, {6, 26, 48, 70}, {6, 26, 50, 74}, {6, 30, 54, 78}, {6, 30, 56, 82}, {6, 30, 58, 86}, {6, 34, 62, 90}, {6, 28, 50, 72, 94}, {6, 26, 50, 74, 98}, {6, 30, 54, 78, 102}, {6, 28, 54, 80, 106}, {6, 32, 58, 84, 110}, {6, 30, 58, 86, 114}, {6, 34, 62, 90, 118}, {6, 26, 50, 74, 98, 122}, {6, 30, 54, 78, 102, 126}, {6, 26, 52, 78, 104, 130}, {6, 30, 56, 82, 108, 134}, {6, 34, 60, 86, 112, 138}, {6, 30, 58, 86, 114, 142}, {6, 34, 62, 90, 118, 146}, {6, 30, 54, 78, 102, 126, 150}, {6, 24, 50, 76, 102, 128, 154}, {6, 28, 54, 80, 106, 132, 158}, {6, 32, 58, 84, 110, 136, 162}, {6, 26, 54, 82, 110, 138, 166}, {6, 30, 58, 86, 114, 142, 170}
    };
    static string const format_info_str[4][8] = {
        {"111011111000100", "111001011110011", "111110110101010", "111100010011101", "110011000101111", "110001100011000", "110110001000001", "110100101110110"}, {"101010000010010", "101000100100101", "101111001111100", "101101101001011", "100010111111001", "100000011001110", "100111110010111", "100101010100000"}, {"011010101011111", "011000001101000", "011111100110001", "011101000000110", "010010010110100", "010000110000011", "010111011011010", "010101111101101"}, {"001011010001001", "001001110111110", "001110011100111", "001100111010000", "000011101100010", "000001001010101", "000110100001100", "000100000111011"}
    };
    static string const version_info_str[34] = {
        "000111110010010100", "001000010110111100", "001001101010011001", "001010010011010011", "001011101111110110", "001100011101100010", "001101100001000111", "001110011000001101", "001111100100101000", "010000101101111000", "010001010001011101", "010010101000010111", "010011010100110010", "010100100110100110", "010101011010000011", "010110100011001001", "010111011111101100", "011000111011000100", "011001000111100001", "011010111110101011", "011011000010001110", "011100110000011010", "011101001100111111", "011110110101110101", "011111001001010000", "100000100111010101", "100001011011110000", "100010100010111010", "100011011110011111", "100100101100001011", "100101010000101110", "100110101001100100", "100111010101000001", "101000110001101001"
    };

    void debug(vector<int> const &a) {
        fprintf(stderr, "[");
        for (size_t i = 0; i < a.size(); i++)
            fprintf(stderr, i == 0 ? "%d" : ", %d", a[i]);
        fprintf(stderr, "]\n");
    }

    int bin2int(string const &bin) {
        int x = 0;
        for (char c: bin)
            x = x * 2 + c - '0';
        return x;
    }
    string int82bin(int x) {
        string res;
        for (size_t _ = 0; _ < 8; _++) {
            res += '0' + x % 2;
            x /= 2;
        }
        reverse(res.begin(), res.end());
        return res;
    }

    template<typename T, typename X>
    vector<vector<T> > get_matrix(vector<T> const &final_bits, int ecl_id, int maskid, T const &white, T const &black, T const &none, X &&mask_xor) {
        vector<vector<T> > m;
        m.reserve(n);
        for (int i = 0; i < n; i++)
            m.emplace_back(n, none);

        // add finder and seperator
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (i == 0 || i == 6)
                    m[i][j] = m[n - i - 1][j] = m[i][n - j - 1] = (j == 7 ? white : black);
                else if (i == 1 || i == 5)
                    m[i][j] = m[n - i - 1][j] = m[i][n - j - 1] = (j == 0 || j == 6 ? black : white);
                else if (i == 7)
                    m[i][j] = m[n - i - 1][j] = m[i][n - j - 1] = white;
                else
                    m[i][j] = m[n - i - 1][j] = m[i][n - j - 1] = (j == 1 || j == 5 || j == 7 ? white : black);
            }
        }
        // add alignment
        if (ver > 1) {
            vector<int> const &coordinates = alig_location[ver - 2];
            for (int row: coordinates)
                for (int col: coordinates)
                    if (m[row][col] == none) {
                        for (int i = row - 2; i < row + 3; i++)
                            for (int j = col - 2; j < col + 3; j++)
                                m[i][j] = (i == row - 2 || i == row + 2 || j == col - 2 || j == col + 2) ? black : white;
                        m[row][col] = black;
                    }
        }

        // add timing, dark and reserving
        for (int i = 8; i < n - 8; i++)
            m[i][6] = m[6][i] = i % 2 == 0 ? black : white;
        for (int j = 0; j < 8; j++)
            m[8][j] = m[8][n - j - 1] = m[j][8] = m[n - j - 1][8] = white;
        m[8][8] = white;
        m[8][6] = m[6][8] = m[n - 8][8] = black;
        if (ver > 6) {
            for (int i = 0; i < 6; i++)
                for (int j = n - 11; j < n - 8; j++)
                    m[i][j] = m[j][i] = white;
        }

        // save none matrix for mask (this should be done before fill)
        auto none_matrix = m;

        // fill bits
        size_t bits_top = 0;
        bool up = true;
        for (int a = n - 1; a > -1; a -= 2) {
            if (a == 6)
                a -= 1;
            if (up) {
                for (int i = n - 1; i > -1; i--)
                    for (int j = a; j > a - 2; j--)
                        if (m[i][j] == none)
                            m[i][j] = final_bits[bits_top++];
            } else {
                for (int i = 0; i < n; i++)
                    for (int j = a; j > a - 2; j--)
                        if (m[i][j] == none)
                            m[i][j] = final_bits[bits_top++];
            }
            up = !up;
        }
        assert(final_bits.size() == bits_top);

        // do mask
        auto formula = [](int maskid, int row, int col) {
            switch (maskid) {
                case 0: return (row + col) % 2 == 0;
                case 1: return row % 2 == 0;
                case 2: return col % 3 == 0;
                case 3: return (row + col) % 3 == 0;
                case 4: return (row / 2 + col / 3) % 2 == 0;
                case 5: return (row * col) % 2 + (row * col) % 3 == 0;
                case 6: return ((row * col) % 2 + (row * col) % 3) % 2 == 0;
                case 7: return ((row + col) % 2 + (row * col) % 3) % 2 == 0;
                default: assert(false); return false;
            }
        };
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (none_matrix[i][j] == none) {
                    int v = formula(maskid, i, j);
                    m[i][j] = mask_xor(m[i][j], v);
                }

        // add format and version string
        string const &fs = format_info_str[ecl_id][maskid];
        for (int j = 0; j < 6; j++) {
            m[8][j] = m[n - j - 1][8] = (fs[j] == '0' ? white : black);
            m[8][n - j - 1] = m[j][8] = (fs[(int)fs.length() - j - 1] == '0' ? white : black);
        }
        m[8][7] = m[n - 7][8] = (fs[6] == '0' ? white : black);
        m[8][8] = m[8][n - 8] = (fs[7] == '0' ? white : black);
        m[7][8] = m[8][n - 7] = (fs[8] == '0' ? white : black);
        if (ver > 6) {
            string const &vs = version_info_str[ver - 7];
            size_t vs_top = 0;
            for (int j = 5; j > -1; j--)
                for (int i = n - 9; i > n - 12; i--)
                    m[i][j] = m[j][i] = (vs[vs_top++] == '0' ? white : black);
            assert(vs.size() == vs_top);
        }
        return m;
    }

    void fill_matrix(string const &p_code, int ecl_id, int maskid, int (&mat)[MAXN][MAXN], int group_list[4], int &p_ecc_num_per_block) {
        string code = p_code;
        vector<int> g(grouping_list[ver - 1][ecl_id]);
        int const ecc_num = ecc_num_per_block[ver - 1][ecl_id];
        int rqbits = 8 * required_bytes[ver - 1][ecl_id];
        int b = rqbits - code.length();
        code += (b >= 4 ? string("0000") : string(b, '0'));
        while (code.length() % 8 != 0)
            code += '0';
        if (is_art) {
            typedef tuple<int, int> ModuleInfo;  // order in code bits, mask value
            vector<ModuleInfo> code_full;
            int rqbits_full = rqbits + 8 * (g[0] + g[2]) * ecc_num + required_remainder_bits[ver - 1];
            for (size_t i = 0; i < code.length(); i++)
                code_full.emplace_back((int)code_full.size(), 0);
            while ((int)code_full.size() < rqbits_full)
                code_full.emplace_back((int)code_full.size(), 0);
            vector<vector<ModuleInfo> > m = get_matrix<ModuleInfo>(
                    code_full,
                    ecl_id,
                    maskid,
                    ModuleInfo(-1, 0),
                    ModuleInfo(-1, 1),
                    ModuleInfo(-1, -1),
                    [](ModuleInfo const &c, bool xr)->ModuleInfo {
                return ModuleInfo(get<0>(c), xr ? 1 : 0);
            });
            vector<pair<int, int> > codeid2rowcol(rqbits_full);
            for (int row = 0; row < n; row++)
                for (int col = 0; col < n; col++) {
                    int id = get<0>(m[row][col]);
                    if (id >= 0)
                        codeid2rowcol[id] = {row, col};
                }

            typedef tuple<int, int, bool> CodewordInfo;  // order in codewords, required value, to be solved by RS
            vector<vector<CodewordInfo> > blocks(g[0] + g[2]);
            int codeword_top = 0;
            for (int _ = 0; _ < (g[0] + g[2]) * g[1]; _++) {
                blocks[codeword_top % (g[0] + g[2])].emplace_back(codeword_top, -1, false);
                codeword_top++;
            }
            for (int _ = 0; _ < g[2]; _++) {
                blocks[(codeword_top + g[0]) % (g[0] + g[2])].emplace_back(codeword_top, -1, false);
                codeword_top++;
            }
            for (int _ = 0; _ < (g[0] + g[2]) * ecc_num; _++) {
                blocks[(codeword_top + g[0]) % (g[0] + g[2])].emplace_back(codeword_top, -1, false);
                codeword_top++;
            }
            codeword_top = 0;
            for (int i = 0; i < g[0] + g[2]; i++) {
                for (int j = 0; j < (i < g[0] ? g[1] : g[3]); j++) {
                    if (8 * codeword_top < (int)code.length())
                        get<1>(blocks[i][j]) = bin2int(code.substr(8 * codeword_top, 8));
                    codeword_top++;
                }
            }
            for (vector<CodewordInfo> &block: blocks) {
                int num_fixed = 0;
                for (CodewordInfo cw: block)
                    if (get<1>(cw) >= 0)
                        num_fixed++;
                int num_changable = (int)block.size() - num_fixed - ecc_num;
                vector<pair<int, size_t> > candidates;  // candidates to be beautified: <sum dist, codeword id>
                for (size_t i = 0; i < block.size(); i++) {
                    if (get<1>(block[i]) == -1) {
                        int id_in_codewords = get<0>(block[i]);
                        int sum_dist = 0;
                        for (size_t j = 0; j < 8; j++) {
                            pair<int, int> rowcol = codeid2rowcol[id_in_codewords * 8 + j];
                            int x = rowcol.first, y = rowcol.second;
                            int dist2 = (x - n / 2) * (x - n / 2) + (y - n / 2) * (y - n / 2);
                            sum_dist += dist2;
                        }
                        candidates.emplace_back(sum_dist, i);
                    }
                }
                sort(candidates.begin(), candidates.end());
                for (int i = 0; i < num_changable; i++) {
                    size_t id_in_block = candidates[i].second;
                    int id_in_codewords = get<0>(block[id_in_block]);
                    int val = 0;
                    for (int j = 0; j < 8; j++) {
                        val *= 2;
                        pair<int, int> rowcol = codeid2rowcol[id_in_codewords * 8 + j];
                        int x = rowcol.first, y = rowcol.second;
                        val += ::img[x][y] ^ get<1>(m[x][y]);
                    }
                    get<1>(block[id_in_block]) = val;
                }
                for (int i = num_changable; i < (int)candidates.size(); i++) {
                    size_t id_in_block = candidates[i].second;
                    get<2>(block[id_in_block]) = true;
                }

                // solve: A x = b
                vector<vector<int> > A(ecc_num);
                vector<int> b(ecc_num);
                for (auto &a: A)
                    a.reserve(ecc_num);
                auto add = [](int a, int b)->int { return a ^ b; };
                auto mul = [](int a, int b)->int { if (a == 0 || b == 0) return 0; else return gf_po2[(gf_log[a] + gf_log[b]) % 255]; };
                auto div = [](int a, int b)->int { if (a == 0 && b != 0) return 0; else return gf_po2[(gf_log[a] - gf_log[b] + 255) % 255]; };
                int sz = (int)block.size();
                for (int i = 0; i < ecc_num; i++) {
                    vector<int> &a(A[i]);
                    int &s(b[i]);
                    for (int j = 0; j < sz; j++) {
                        int aij = gf_po2[(i * (sz - 1 - j)) % 255];
                        if (get<2>(block[j]))
                            a.push_back(aij);
                        else
                            s = add(s, mul(aij, get<1>(block[j])));
                    }
                }
                for (int j = 0; j < ecc_num; j++) {
                    int p = A[j][j];
                    for (int k = j; k < ecc_num; k++)
                        A[j][k] = div(A[j][k], p);
                    b[j] = div(b[j], p);
                    for (int i = 0; i < ecc_num; i++) {
                        if (i == j)
                            continue;
                        int p = A[i][j];
                        for (int k = j; k < ecc_num; k++)
                            A[i][k] = add(A[i][k], mul(A[j][k], p));
                        b[i] = add(b[i], mul(b[j], p));
                    }
                }
                size_t b_top = 0;
                for (CodewordInfo &cw: block) {
                    if (get<2>(cw) == true) {
                        get<1>(cw) = b[b_top++];
                    }
                }
            }

            string new_code;
            for (int i = 0; i < g[0] + g[2]; i++) {
                for (int j = 0; j < (i < g[0] ? g[1] : g[3]); j++) {
                    int x = get<1>(blocks[i][j]);
                    if (x == -1) {
                        x = 0;
                    }
                    new_code += int82bin(x);
                }
            }
            code = new_code;
        } else {
            while ((int)code.length() < rqbits)
                code += rqbits - code.length() == 8 ? "11101100" : "1110110000010001";
        }
        assert((int)code.length() == rqbits);
        vector<int> data_code;
        for (int i = 0; i < rqbits; i += 8) {
            data_code.push_back(bin2int(code.substr(i, 8)));
        }
        vector<vector<int> > data_codewords;
        int i_pos = 0;
        for (int _ = 0; _ < g[0]; _++) {
            data_codewords.emplace_back(data_code.begin() + i_pos, data_code.begin() + i_pos + g[1]);
            i_pos += g[1];
        }
        for (int _ = 0; _ < g[2]; _++) {
            data_codewords.emplace_back(data_code.begin() + i_pos, data_code.begin() + i_pos + g[3]);
            i_pos += g[3];
        }

        // ECC encode
        vector<vector<int> > ecc;
        vector<int> const &gp(GP_list[ecc_num]);
        auto gf_XOR = [](vector<int> &GP, vector<int> &MP)->vector<int> {
            while (GP.size() < MP.size()) GP.push_back(0);
            while (MP.size() < GP.size()) MP.push_back(0);
            vector<int> remainder;
            for (size_t i = 1; i < MP.size(); i++)
                remainder.push_back(MP[i] ^ GP[i]);
            return remainder;
        };
        for (vector<int> const &dc: data_codewords) {
            vector<int> remainder(dc);
            for (size_t _ = 0; _ < dc.size(); _++) {
                vector<int> MP = remainder;
                vector<int> GP = gp;
                if (MP[0]) {
                    for (int &x: GP) {
                        x += gf_log[MP[0]];
                        if (x > 255)
                            x %= 255;
                        x = gf_po2[x];
                    }
                } else {
                    fill(GP.begin(), GP.end(), 0);
                }
                remainder = gf_XOR(GP, MP);
            }
            ecc.push_back(remainder);
        }

        // final bits
        vector<int> final_message;
        for (int i = 0; i < g[1]; i++)
            for (auto const &dc: data_codewords)
                final_message.push_back(dc[i]);
        if (g[2])
            for (int i = g[0]; i < g[0] + g[2]; i++)
                final_message.push_back(data_codewords[i][g[3] - 1]);
        for (int i = 0; i < ecc_num; i++)
            for (auto const &ec: ecc)
                final_message.push_back(ec[i]);
        string final_bits;
        for (int x: final_message)
            final_bits += int82bin(x);
        final_bits += string(required_remainder_bits[ver - 1], '0');

        // fill to mat
        vector<int> final_bits_vector;
        for (char c: final_bits)
            final_bits_vector.push_back((int)(c - '0'));
        auto m = get_matrix<int>(final_bits_vector, ecl_id, maskid, 0, 1, -1, [](int x, bool b)->int { assert(x == 0 || x == 1); if (b) return 1 - x; else return x; });
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                mat[i][j] = m[i][j];
        for (size_t i = 0; i < 4; i++)
            group_list[i] = g[i];
        p_ecc_num_per_block = ecc_num;
    }

    void process() {
        char *buf = new char[1 << 20];
        int maskid1, maskid2;
        string code1, code2;
        int res;
        res = scanf("%d%s%s%d%d%d%d%lld%lf%u", &ver, &buf[0], &buf[2], &maskid1, &maskid2, &is_art, &::do_reduce, &::merge_bound_thresh, &::lambda, &::seed);
        assert(10 == res);
        int ecl_id1 = map<char, int>({{'L', 0}, {'M', 1}, {'Q', 2}, {'H', 3}})[buf[0]];
        int ecl_id2 = map<char, int>({{'L', 0}, {'M', 1}, {'Q', 2}, {'H', 3}})[buf[2]];
        res = scanf("%s", buf);
        assert(1 == res);
        code1 = buf;
        res = scanf("%s", buf);
        assert(1 == res);
        code2 = buf;

        n = 17 + 4 * ver;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (is_art) {
                    res = scanf("%d", &::img[i][j]);
                    assert(1 == res);
                } else {
                    ::img[i][j] = 0;
                }
            }
        }
        assert(EOF != res);
        if (EOF == res)
            exit(1);
        int junk;
        res = scanf("%d", &junk);
        assert(EOF == res);
        if (EOF != res)
            exit(1);

        clock_gen_start = clock();

        fill_matrix(code1, ecl_id1, maskid1, ::mat1, ::group_list1, ::ecc_num_per_block1);
        fill_matrix(code2, ecl_id2, maskid2, ::mat2, ::group_list2, ::ecc_num_per_block2);

        // set ::sol
        vector<int> sol_vector;
        int sol_num = ::group_list1[0] * (::group_list1[1] + ::ecc_num_per_block1) + ::group_list1[2] * (::group_list1[3] + ::ecc_num_per_block1);
        assert(sol_num == ::group_list2[0] * (::group_list2[1] + ::ecc_num_per_block2) + ::group_list2[2] * (::group_list2[3] + ::ecc_num_per_block2));
        for (int i = 0; i < sol_num; i++)
            for (int j = 0; j < 8; j++)
                sol_vector.push_back(i + 2);
        for (int j = 0; j < required_remainder_bits[ver - 1]; j++)
            sol_vector.push_back(sol_num + 2);
        vector<vector<int> > sol = get_matrix<int>(sol_vector, ecl_id1, 0, 0, 1, -1, [](int x, bool)->int { return x; });
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                ::sol[i][j] = sol[i][j];

        // load pre-computed format information
        int info1 = ecl_id1 * 8 + maskid1;
        int info2 = ecl_id2 * 8 + maskid2;
        FILE *fp = fopen("formatinformation.bin", "rb");
        if (!fp) {
            fprintf(stderr, "no such file: %s\n", "formatinformation.bin");
            exit(1);
        }
        fseek(fp, 38 * (info1 * 32 + info2), SEEK_SET);
        auto read = [fp](int &x)->void {
            char c;
            size_t res = fread(&c, sizeof(c), 1, fp);
            assert(1 == res);
            if (1 != res)
                exit(1);
            x = (int)c;
        };
        for (int i = 0; i < 9; i++)
            read(::layer1[8][i]);
        for (int i = 0; i < 10; i++)
            read(::layer2[8][i]);
        for (int i = 0; i < 8; i++)
            read(::layer1[8][n - 8 + i]);
        for (int i = 0; i < 9; i++)
            read(::layer2[8][n - 8 + i]);
        int err1, err2;
        read(err1);
        read(err2);
        fclose(fp);
        assert(max<int>(err1, err2) < 3);
        if (max<int>(err1, err2) >= 3)
            exit(1);
        ::error_format_bits = make_pair(err1, err2);
        ::n = n;
        ::is_art = is_art;
        delete[] buf;
        fprintf(stderr, "finish QR Code generating\n");
    }
}  // namespace gen

template<typename T>
bool set_lte(std::set<T> const &a, std::set<T> const &b) {
    if (!do_reduce)
        return false;
    for (T const &x: a) {
        if (b.find(x) == b.end())
            return false;
    }
    return true;
}

template<typename T>
bool set_lte(std::vector<T> const &a, std::vector<T> const &b) { // `a' and `b' should be sorted
    if (!do_reduce)
        return false;
    if (a.size() > b.size())
        return false;
    if (a.size() == 0)
        return true;
    for (auto it_a = a.begin(), it_b = b.begin(); ;) {
        if (*it_a == *it_b) {
            it_a++;
            it_b++;
            if (it_a == a.end())
                return true;
            if (it_b == b.end())
                return false;
        } else if (*it_a < *it_b) {
            return false;
        } else {
            it_b++;
            if (it_b == b.end())
                return false;
        }
    }
}

bool cnt_lte(std::vector<int> const &a, std::vector<int> const &b) {
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] > b[i])
            return false;
    }
    return true;
}

inline int merge_bit(int l1c, int l2c) {
    return l2c == -1 ? l1c : l2c;
}

int sol2groupno(int sol_id, int group_list[4]) {
    int n_group = group_list[0] + group_list[2];
    int s = sol_id - 2;
    int t = n_group * group_list[1];
    if (s < t)
        return s % n_group;
    s -= t;
    if (s < group_list[2])
        return group_list[0] + s;
    s -= group_list[2];
    return s % n_group;
}

int sol2groupno1(int sol_id) {
    int n_group = group_list1[0] + group_list1[2];
    int s = sol_id - 2;
    int t = n_group * group_list1[1];
    if (s < t)
        return s % n_group;
    s -= t;
    if (s < group_list1[2])
        return group_list1[0] + s;
    s -= group_list1[2];
    return s % n_group;
}

class SolPairHash {
public:
    std::size_t operator()(std::pair<int, int> const &rhs) const {
        return (size_t)(rhs.first - 1) * MAXSOLOMON + (size_t)rhs.second;
    }
    std::size_t upbound() const {
        return 2 * MAXSOLOMON;
    }
};


int main() {
    clock_read_start = clock();
    gen::process();

    long long max_merge_bound = 0;
    int const n_group1 = group_list1[0] + group_list1[2];
    int const n_group2 = group_list2[0] + group_list2[2];

    clock_t clock_alg_start = clock();
    vector<vector<vector<pair<int, int> > > > a;
    set<pair<int, int> > sol_in_only_solution;
    set<pair<int, int> > must_01_fail;
    for (int i = 0; i < n; i++) {
        int (&l1)[MAXN] = layer1[i];
        int (&l2)[MAXN] = layer2[i];
        int (&so)[MAXN] = sol[i];
        int (&m1)[MAXN] = mat1[i];
        int (&m2)[MAXN] = mat2[i];
        for (int j = 0; j < n; j++) {
            if (i != 8 || (j >= 9 && j < n - 8)) { // skip format information in line 8
                if (m1[j] == m2[j])
                    l1[j] = m1[j];
                else
                    l1[j] = 2;
            }
            if (i != 8 || (j >= 10 && j < n - 8)) {
                if (j == 0 || m1[j] == m2[j - 1])
                    l2[j] = m1[j];
                else
                    l2[j] = 2;
            }
        }
        if (i == 8) {
            if (l2[9] != -1 && l2[9] != l1[9])
                l1[9] = 2;
            if (l2[n - 8] != -1 && l2[n - 8] != l1[n - 9])
                l1[n - 9] = 2;
        } else {
            l2[n] = (int)m2[n - 1];
        }
        auto decide_layer_pos = [&](int k)->int & {
            if (k % 2 == 0)
                return l2[k / 2];
            else
                return l1[k / 2];
        };
        auto dfs_init = [](int k)->int {
            return k % 2 == 0 ? -2 : -1;
        };
        auto is_sure = [&](int k)->bool {
            return decide_layer_pos(k) != 2;
        };

        int lo = 0;
        while (1) {
            while (lo <= n * 2 && is_sure(lo))
                lo++;
            if (lo > n * 2)
                break;
            int hi = lo + 1;
            while (hi <= n * 2 + 1 && !is_sure(hi))
                hi++;
            assert(lo < n * 2 && hi <= n * 2);
            if (hi - lo >= 15) {
                int mid = lo + (hi - lo - 1) / 2;
                if (mid % 2 != 0)
                    mid++;
                l2[mid / 2] = -1;
                continue;
            }

            stack<int> s;
            s.push(dfs_init(lo));
            set<vector<pair<int, int> > > possible_failed_sol;
            vector<pair<int, int> > current_failed;
            while (1) {
                int cur = lo + (int)s.size() - 1;
                if (s.empty())
                    break;
                s.top()++;
                int t = s.top();
                if (t > 1) {
                    s.pop();
                    current_failed.pop_back();
                    continue;
                }
                pair<int, int> fail_sol = {-1, -1};
                if (cur != hi)
                    decide_layer_pos(cur) = t;
                if (cur % 2 == 0) {
                    if (m2[cur / 2 - 1] != merge_bit(l1[cur / 2 - 1], l2[cur / 2])) {
                        fail_sol = {2, so[cur / 2 - 1]};
                    }
                } else {
                    if (m1[cur / 2] != merge_bit(l1[cur / 2], l2[cur / 2])) {
                        fail_sol = {1, so[cur / 2]};
                    }
                }
                if (cur != hi) {
                    current_failed.push_back(fail_sol);
                    s.push(dfs_init(cur + 1));
                } else {
                    current_failed.push_back(fail_sol);
                    set<pair<int, int> > st;
                    for (size_t j = 0; j < current_failed.size(); j++) {
                        if (current_failed[j].first!= -1)
                            st.insert(current_failed[j]);
                    }
                    possible_failed_sol.emplace(st.begin(), st.end());
                    current_failed.pop_back();
                }
            }
            vector<vector<pair<int, int> > > minimum_possible;
            for (vector<pair<int, int> > const &one_option: possible_failed_sol) {
                bool is_min = true;
                for (vector<pair<int, int> > const &other_option: possible_failed_sol) {
                    if (one_option != other_option && set_lte(other_option, one_option)) {
                        is_min = false;
                        break;
                    }
                }
                if (is_min) {
                    bool has_zero = false;
                    for (pair<int, int> const &p: one_option) {
                        if (p.second == 0 || p.second == 1)
                            has_zero = true;
                    }
                    if (!has_zero) // 不允许关键格式信息（sol==0,1）出错误
                        minimum_possible.push_back(one_option);
                    // TODO: allow errors on timing pattern
                }
            }
            assert(possible_failed_sol.size() != 0);
            if (minimum_possible.size() == 0) { // Note: since we have skipped format information in line 8, minimum_possible.size() should never be 0.
                fprintf(stderr, "no no 0,1 solution: line %d, [%d, %d)\n", i, lo, hi);
                assert(i == 8);
                must_01_fail.emplace(i, lo);
            }
            if (minimum_possible.size() == 1 && 0 != minimum_possible.begin()->size()) {
                fprintf(stderr, "only one sulution, line %d, [%d, %d): %zu\n", i, lo, hi, minimum_possible.begin()->size());
                sol_in_only_solution.insert(minimum_possible.begin()->begin(), minimum_possible.begin()->end());
            }
            if (minimum_possible.size() > 1) // TODO：对于必选的错误，记录下来，不重复加入 minimum_possible 可以加快求解速度
            {
                fprintf(stderr, "line %d, [%d, %d): fail candidate %zu -> %zu\n", i, lo, hi, possible_failed_sol.size(), minimum_possible.size());
                vector<vector<pair<int, int> > > b;
                for (vector<pair<int, int> > const &one_option: minimum_possible) {
                    if (do_reduce)
                        assert(one_option.size() != 0);
                    vector<pair<int, int> > c;
                    for (pair<int, int> const &p: one_option) {
                        fprintf(stderr, "  (%d, %d)", p.first, p.second);
                        c.push_back(p);
                    }
                    fprintf(stderr, "\n");
                    sort(c.begin(), c.end());
                    b.push_back(c);
                }
                a.push_back(b);
            }
            lo = hi + 1;
        }
    }
    assert(must_01_fail.empty());
    for (auto &b: a)
        for (auto &c: b)
            for (pair<int, int> const &p: sol_in_only_solution)
                c.resize(remove(c.begin(), c.end(), p) - c.begin());

    default_random_engine generator(seed);

    // merge groups
    vector<size_t> a2group;
    set<size_t> group_alive;
    vector<vector<pair<int, int> > > group_sol;
    vector<vector<vector<pair<int, int> > > > group_options;
    vector<vector<vector<pair<int, int> > > > sol_fail_cand;
    vector<size_t> a2cand(a.size());

    group_sol.reserve(a.size() * 2);
    group_options.reserve(a.size() * 2);
    for (size_t i = 0; i < a.size(); i++) {
        auto const &b = a[i];
        a2group.push_back(i);
        group_alive.insert(i);
        set<pair<int, int> > sols;
        for (auto const &c: b) {
            sols.insert(c.begin(), c.end());
        }
        group_sol.emplace_back(sols.begin(), sols.end());
        group_options.push_back(b);
    }
    map<pair<int, int>, vector<size_t> > sol2group;
    while (1) {
        bool group_changed = false;
        for (auto it = sol2group.begin(); it != sol2group.end(); it++)
            it->second.clear();
        vector<size_t> gi_order(group_alive.begin(), group_alive.end());
        vector<size_t> option_idx_order;
        shuffle(gi_order.begin(), gi_order.end(), generator);
        for (size_t gi: gi_order) {
            option_idx_order.resize(group_sol[gi].size());
            for (size_t i = 0; i < option_idx_order.size(); i++)
                option_idx_order[i] = i;
            shuffle(option_idx_order.begin(), option_idx_order.end(), generator);
            for (size_t option_idx: option_idx_order) {
                pair<int, int> const &option(group_sol[gi][option_idx_order[option_idx]]);
                vector<size_t> &prev_groups(sol2group[option]);
                for (size_t gj: prev_groups) {
                    if ((long long)group_options[gi].size() * (long long)group_options[gj].size() < merge_bound_thresh) {
                        max_merge_bound = std::max<long long>(max_merge_bound, (long long)group_options[gi].size() * (long long)group_options[gj].size());
                        size_t new_group_id = group_sol.size();
                        group_sol.push_back(vector<pair<int, int> >(group_sol[gi].size() + group_sol[gj].size()));
                        merge(group_sol[gi].begin(), group_sol[gi].end(), group_sol[gj].begin(), group_sol[gj].end(), group_sol.back().begin());
                        auto bound = unique(group_sol.back().begin(), group_sol.back().end());
                        group_sol.back().resize(bound - group_sol.back().begin());
                        group_options.emplace_back();
                        vector<vector<pair<int, int> > > &new_options(group_options.back());
                        for (auto const &option_i: group_options[gi]) {
                            for (auto const &option_j: group_options[gj]) {
                                vector<pair<int, int> > merged(option_i.size() + option_j.size());
                                merge(option_i.begin(), option_i.end(), option_j.begin(), option_j.end(), merged.begin());
                                auto bound = unique(merged.begin(), merged.end());
                                merged.resize(bound - merged.begin());
                                bool is_smaller = true;
                                for (auto const &option: new_options) {
                                    if (set_lte(option, merged)) {
                                        is_smaller = false;
                                        break;
                                    }
                                }
                                if (is_smaller) {
                                    auto bound = partition(new_options.begin(), new_options.end(), [&](vector<pair<int, int> > const &option) {
                                        return !set_lte(merged, option);
                                    });
                                    new_options.resize(bound - new_options.begin());
                                    new_options.push_back(merged);
                                }
                            }
                        }
                        for (size_t k = 0; k < a2group.size(); k++)
                            if (a2group[k] == gi || a2group[k] == gj)
                                a2group[k] = new_group_id;
                        group_alive.erase(gi);
                        group_alive.erase(gj);
                        group_alive.insert(new_group_id);
                        group_changed = true;
                        fprintf(stderr, "merge group: %zu * %zu => %zu\n", group_options[gi].size(), group_options[gj].size(), new_options.size());
                        group_options[gi].clear(); // release memory
                        group_options[gj].clear();
                    }
                    if (group_changed)
                        break;
                }
                if (group_changed)
                    break;
                prev_groups.push_back(gi);
                shuffle(prev_groups.begin(), prev_groups.end(), generator);
            }
            if (group_changed)
                break;
        }
        if (!group_changed)
            break;
    }
    sol_fail_cand.reserve(group_alive.size());
    for (size_t group_id: group_alive) {
        vector<size_t> a_in_group;
        for (size_t j = 0; j < a.size(); j++) {
            if (a2group[j] == group_id)
                a_in_group.push_back(j);
        }
        for (size_t a_id: a_in_group)
            a2cand[a_id] = sol_fail_cand.size();
        sol_fail_cand.emplace_back();
        auto &cands = sol_fail_cand.back();
        for (auto const &options: group_options[group_id])
            cands.emplace_back(options.begin(), options.end());
    }

    fprintf(stderr, "#subgroup=%zu\n", sol_fail_cand.size());
    fprintf(stderr, "sol blocks group (%d, %d, %d, %d), (%d, %d, %d, %d), ecc_num_per_block = %d, %d\n", group_list1[0], group_list1[1], group_list1[2], group_list1[3], group_list2[0], group_list2[1], group_list2[2], group_list2[3], ecc_num_per_block1, ecc_num_per_block2);
    vector<size_t> best_sel(sol_fail_cand.size(), 0);

#ifdef TLQR_FAST_COUNT
    int max_solomon = 2 + group_list1[0] * group_list1[1] + group_list1[2] * group_list1[3] + (group_list1[0] + group_list1[2]) * ecc_num_per_block1;
    assert(max_solomon == 2 + group_list2[0] * group_list2[1] + group_list2[2] * group_list2[3] + (group_list2[0] + group_list2[2]) * ecc_num_per_block2);
    auto hasher = [&](pair<int, int> const &rhs)->size_t {
        return (size_t)(rhs.first - 1) * max_solomon + (size_t)rhs.second;
    };
    vector<int> hash2cntind(2 * max_solomon);
    for (int i = 0; i < max_solomon; i++) {
        pair<int, int> p1(1, i);
        pair<int, int> p2(2, i);
        hash2cntind[hasher(p1)] = sol2groupno(i, group_list1);
        hash2cntind[hasher(p2)] = n_group1 + sol2groupno(i, group_list2);
    }
    function<vector<int>(vector<size_t> const &)> sel2cnt = [&](vector<size_t> const &sel)->vector<int> {
        vector<bool> already_counted(2 * max_solomon, 0);
        vector<int> cnt((size_t)(n_group1 + n_group2), 0);
        for (auto const &p: sol_in_only_solution) {
            assert(1 == p.first || 2 == p.first);
            size_t hash = hasher(p);
            if (already_counted[hash] == false) {
                already_counted[hash] = true;
                cnt[hash2cntind[hash]]++;
            }
        }
        for (size_t j = 0; j < sol_fail_cand.size(); j++) {
            for (auto const &p: sol_fail_cand[j][sel[j]]) {
                assert(1 == p.first || 2 == p.first);
                size_t hash = hasher(p);
                if (already_counted[hash] == false) {
                    already_counted[hash] = true;
                    cnt[hash2cntind[hash]]++;
                }
            }
        }
        return cnt;
    };
#else // to be comparable with paper
    SolPairHash hasher;
    function<vector<int>(vector<size_t> const &)> sel2cnt = 0 == memcmp(group_list1, group_list2, sizeof(group_list1)) ? function<vector<int>(vector<size_t> const &)>([&](vector<size_t> const &sel)->vector<int> {
        vector<bool> already_counted(2 * MAXSOLOMON, 0);
        vector<int> cnt((size_t)(n_group1 + n_group2), 0);
        for (auto const &p: sol_in_only_solution) {
            assert(1 == p.first || 2 == p.first);
            size_t hash = hasher(p);
            if (already_counted[hash] == false) {
                already_counted[hash] = true;
                cnt[(size_t)((p.first - 1) * n_group1 + sol2groupno1(p.second))]++;
            }
        }
        for (size_t j = 0; j < sol_fail_cand.size(); j++) {
            for (auto const &p: sol_fail_cand[j][sel[j]]) {
                assert(1 == p.first || 2 == p.first);
                size_t hash = hasher(p);
                if (already_counted[hash] == false) {
                    already_counted[hash] = true;
                    cnt[(size_t)((p.first - 1) * n_group1 + sol2groupno1(p.second))]++;
                }
            }
        }
        return cnt;
    }) : function<vector<int>(vector<size_t> const &)>([&](vector<size_t> const &sel)->vector<int> {
        vector<bool> already_counted(2 * MAXSOLOMON, 0);
        vector<int> cnt((size_t)(n_group1 + n_group2), 0);
        for (auto const &p: sol_in_only_solution) {
            assert(1 == p.first || 2 == p.first);
            size_t hash = hasher(p);
            if (already_counted[hash] == false) {
                already_counted[hash] = true;
                if (p.first == 1)
                    cnt[(size_t)(sol2groupno(p.second, group_list1))]++;
                else
                    cnt[(size_t)(n_group1 + sol2groupno(p.second, group_list2))]++;
            }
        }
        for (size_t j = 0; j < sol_fail_cand.size(); j++) {
            for (auto const &p: sol_fail_cand[j][sel[j]]) {
                assert(1 == p.first || 2 == p.first);
                size_t hash = hasher(p);
                if (already_counted[hash] == false) {
                    already_counted[hash] = true;
                    if (p.first == 1)
                        cnt[(size_t)(sol2groupno(p.second, group_list1))]++;
                    else
                        cnt[(size_t)(n_group1 + sol2groupno(p.second, group_list2))]++;
                }
            }
        }
        return cnt;
    });
#endif

    uniform_int_distribution<int> change_num(1, 10);
    uniform_int_distribution<size_t> get_cand_ind(0, a2cand.size() - 1);
    vector<uniform_int_distribution<size_t> > get_cand;
    for (size_t i = 0; i < sol_fail_cand.size(); i++) {
        assert(sol_fail_cand[i].size() > 0);
        get_cand.emplace_back(0, sol_fail_cand[i].size() - 1);
    }
    uniform_real_distribution<double> annealing_thresh(0., 1.);
    struct Fraction {
        int num, den;
        bool operator <(Fraction const &o) const {
            if ((den > 0) == (o.den > 0))
                return num * o.den < o.num * den;
            else
                return num * o.den > o.num * den;
        }
        double toDouble() const {
            return (double)num / (double)den;
        }
    };
    struct Score {
        int fail_max[4];
        int fail_sum[4];
        Fraction recovery_ratio() const {
            int num_data[4] = {group_list1[1], group_list1[3], group_list2[1], group_list2[3]};
            int num_ecc[4] = {ecc_num_per_block1, ecc_num_per_block1, ecc_num_per_block2, ecc_num_per_block2};
            Fraction x = {1, 1};
            for (size_t i = 0; i < 4; i++) {
                if (num_data[i] > 0) {
                    Fraction x2 = {num_ecc[i] / 2 - fail_max[i], num_data[i] + num_ecc[i]};
                    x = std::min<Fraction>(x, x2);
                }
            }
            return x;
        }
        double sum_fail_ratio() const {
            int num_data[4] = {group_list1[1], group_list1[3], group_list2[1], group_list2[3]};
            int num_ecc[4] = {ecc_num_per_block1, ecc_num_per_block1, ecc_num_per_block2, ecc_num_per_block2};
            double a = 0.;
            for (size_t i = 0; i < 4; i++)
                if (num_data[i] > 0)
                    a += (double)fail_sum[i] / (double)(num_data[i] + num_ecc[i]);
            return a;
        }
        double toDouble() {
            double x = recovery_ratio().toDouble();
            double a = sum_fail_ratio();
            return -x + lambda * a;
        }
        bool operator <(Score const &o) const {
            Fraction f0 = recovery_ratio();
            Fraction f1 = o.recovery_ratio();
            if (f1 < f0)
                return true;
            if (f0 < f1)
                return false;
            return sum_fail_ratio() < o.sum_fail_ratio();
        }
    };
    auto score = [&](vector<size_t> const &sel)->Score {
        vector<int> cnt(sel2cnt(sel));
        Score s = {{0, 0, 0, 0}, {0, 0, 0, 0}};
        size_t i = 0;
        for (int j = 0; j < group_list1[0]; i++, j++) {
            s.fail_max[0] = max(s.fail_max[0], cnt[i]);
            s.fail_sum[0] += cnt[i];
        }
        for (int j = 0; j < group_list1[2]; i++, j++) {
            s.fail_max[1] = max(s.fail_max[1], cnt[i]);
            s.fail_sum[1] += cnt[i];
        }
        for (int j = 0; j < group_list2[0]; i++, j++) {
            s.fail_max[2] = max(s.fail_max[2], cnt[i]);
            s.fail_sum[2] += cnt[i];
        }
        for (int j = 0; j < group_list2[2]; i++, j++) {
            s.fail_max[3] = max(s.fail_max[3], cnt[i]);
            s.fail_sum[3] += cnt[i];
        }
        return s;
    };

    if (a2cand.size() > 0) {
        double T = 1.;
        Score score_best_sel = score(best_sel);
        vector<size_t> cur_sel(sol_fail_cand.size(), 0);
        Score score_cur_sel = score(cur_sel);
        double last_update = T;
        for (int i = 0; true; i++) {
            static int const iter_per_T = 20;
            for (int j = 1; j < iter_per_T; j++) {
                vector<size_t> new_sel = cur_sel;
                for (int k = change_num(generator); k > 0; k--) {
                    size_t s = a2cand[get_cand_ind(generator)];
                    new_sel[s] = get_cand[s](generator);
                }
                Score score_new_sel = score(new_sel);
                if (score_best_sel.recovery_ratio() < score_new_sel.recovery_ratio())
                    last_update = T;
                if (score_new_sel < score_best_sel) {
                    score_best_sel = score_new_sel;
                    best_sel = new_sel;
                    fprintf(stderr, "SA T=%f, fail_max=%d,%d,%d,%d, sum=%d, i=%d\n", T, score_new_sel.fail_max[0], score_new_sel.fail_max[1], score_new_sel.fail_max[2], score_new_sel.fail_max[3], score_new_sel.fail_sum[0] + score_new_sel.fail_sum[1] + score_new_sel.fail_sum[2] + score_new_sel.fail_sum[3], i);
                }
                double delta = score_cur_sel.toDouble() - score_new_sel.toDouble();
                if (score_new_sel < score_best_sel || delta > 0 || exp(delta / T) > annealing_thresh(generator)) {
                    cur_sel = new_sel;
                    score_cur_sel = score_new_sel;
                }
            }
            T *= .999;
            if (T < last_update * 1e-3) {
                fprintf(stderr, "iter = %d\n", i);
                break;
            }
        }
        fprintf(stderr, "T=%f finnaly\n", T);
        fprintf(stderr, "(after annealing)\n");
    }

    vector<int> best_cnt = sel2cnt(best_sel);
    fprintf(stderr, "best cnt: {%d", best_cnt[0]);
    for (size_t i = 1; i < best_cnt.size(); i++)
        fprintf(stderr, ", %d", best_cnt[i]);
    fprintf(stderr, "}\n");

    set<pair<int, int> > sol_giveup;
    for (size_t i = 0; i < sol_fail_cand.size(); i++) {
        vector<pair<int, int> > const &this_sel(sol_fail_cand[i][best_sel[i]]);
        sol_giveup.insert(this_sel.begin(), this_sel.end());
    }
    sol_giveup.insert(sol_in_only_solution.begin(), sol_in_only_solution.end());
    assert(sol_giveup.find({1, 0}) == sol_giveup.end());
    assert(sol_giveup.find({1, 1}) == sol_giveup.end());
    assert(sol_giveup.find({2, 0}) == sol_giveup.end());
    assert(sol_giveup.find({2, 1}) == sol_giveup.end());
    for (int i = 0; i < n; i++) {
        int (&l1)[MAXN] = layer1[i];
        int (&l2)[MAXN] = layer2[i];
        int (&so)[MAXN] = sol[i];
        int (&m1)[MAXN] = mat1[i];
        int (&m2)[MAXN] = mat2[i];
        for (int j = 0; j < n; j++) {
            if (i != 8 || (j >= 9 && j < n - 8)) { // skip format information in line 8
                if (m1[j] == m2[j])
                    l1[j] = m1[j];
                else
                    l1[j] = 2;
            }
            if (i != 8 || (j >= 10 && j < n - 8)) {
                if (j == 0 || m1[j] == m2[j - 1])
                    l2[j] = m1[j];
                else
                    l2[j] = 2;
            }
        }
        if (i == 8) {
            if (l2[9] != -1 && l2[9] != l1[9])
                l1[9] = 2;
            if (l2[n - 8] != -1 && l2[n - 8] != l1[n - 9])
                l1[n - 9] = 2;
        } else {
            l2[0] = m1[0] == m2[0] ? -1 : (int)m1[0];
            l2[n] = m1[n - 1] == m2[n - 1] ? -1 : (int)m2[n - 1];
        }
        l1[n] = 0;
        auto decide_layer_pos = [&](int k)->int & {
            if (k % 2 == 0)
                return l2[k / 2];
            else
                return l1[k / 2];
        };
        auto dfs_init = [](int k)->int {
            return k % 2 == 0 ? -2 : -1;
        };
        auto is_sure = [&](int k)->bool {
            return decide_layer_pos(k) != 2;
        };
        int lo = 0;
        while (1) {
            while (lo <= n * 2 && is_sure(lo))
                lo++;
            if (lo > n * 2)
                break;
            int hi = lo + 1;
            while (hi <= n * 2 + 1 && !is_sure(hi))
                hi++;
            assert(lo < n * 2 && hi <= n * 2);
            if (hi - lo >= 15) {
                int mid = lo + (hi - lo - 1) / 2;
                if (mid % 2 != 0)
                    mid++;
                l2[mid / 2] = -1;
                continue;
            }

            int min_cnt_fail = (hi - lo + 1) * 2;
            stack<int> s;
            vector<pair<int, bool> > flags;
            vector<int> bak_min_fail((size_t)(hi - lo), 0);
            int min_times = 0;
            s.push(dfs_init(lo));
            flags.emplace_back(0, true);
            while (1) {
                int cur = lo + (int)s.size() - 1;
                if (cur > hi) {
                    int forbidden = false;
                    int cnt_fail = 0;
                    for (pair<int, bool> flag: flags) {
                        if (flag.first == 0 && flag.second == false)
                            cnt_fail++;
                        if (flag.first == 1 && flag.second == false)
                            forbidden = true;
                    }
                    if (!forbidden) {
                        if (cnt_fail <= min_cnt_fail) {
                            // 取透明块多的解
                            int cnt_tr_bak = 0;
                            int cnt_tr_nw = 0;
                            int cnt_img_bak = 0;
                            int cnt_img_nw = 0;
                            for (int j = lo; j < hi; j++) {
                                if (-1 == decide_layer_pos(j))
                                    cnt_tr_nw += 2;
                                if (-1 == bak_min_fail[(size_t)(j - lo)])
                                    cnt_tr_bak += 2;
                            }
                            // 尽量取可以使透明块多的解
                            if (lo % 2 == 1 && lo / 2 - 1 >= 0) {
                                if (m2[lo / 2 - 1] == l1[lo / 2 - 1] || sol_giveup.find({2, so[lo / 2 - 1]}) != sol_giveup.end()) {
                                    if (sol_giveup.find({1, so[lo / 2]}) == sol_giveup.end()) {
                                        if (m1[lo / 2] == l1[lo / 2])
                                            cnt_tr_nw += 2;
                                        if (m1[lo / 2] == bak_min_fail[0])
                                            cnt_tr_bak += 2;
                                    }
                                }
                            }
                            if (hi % 2 == 0 && hi / 2 - 1 < n) {
                                if (m2[hi / 2 - 1] == l1[hi / 2 - 1])
                                    cnt_tr_nw++;
                                if (m2[hi / 2 - 1] == bak_min_fail[(size_t)(hi - lo - 1)])
                                    cnt_tr_bak++;
                            }
                            for (int j = lo; j < hi; j++) {
                                if (j % 2 == 0) {
                                    if (j > 0 && img[i][j / 2 - 1] == merge_bit(l1[j / 2 - 1], l2[j / 2]))
                                        cnt_img_nw += 2;
                                    if (j + 1 < hi && img[i][j / 2] == merge_bit(l1[j / 2], l2[j / 2]))
                                        cnt_img_nw += 2;
                                    if (j > 0 && img[i][j / 2 - 1] == merge_bit(j - lo - 1 < 0 ? l1[j / 2 - 1] : bak_min_fail[(size_t)(j - lo - 1)], bak_min_fail[(size_t)(j - lo)]))
                                        cnt_img_bak += 2;
                                    if (j + 1 < hi && img[i][j / 2] == merge_bit(bak_min_fail[(size_t)(j - lo + 1)], bak_min_fail[(size_t)(j - lo)]))
                                        cnt_img_bak += 2;
                                } else {
                                    if (img[i][j / 2] == merge_bit(l1[j / 2], l2[j / 2]))
                                        cnt_img_nw += 2;
                                    if (j + 1 < hi) {
                                        if (img[i][j / 2] == merge_bit(l1[j / 2], l2[j / 2 + 1]))
                                            cnt_img_nw += 2;
                                    } else {
                                        if (img[i][j / 2] == l1[j / 2])
                                            cnt_img_nw += 1;
                                    }
                                    if (img[i][j / 2] == merge_bit(bak_min_fail[(size_t)(j - lo)], j - lo - 1 < 0 ? l2[j / 2] : bak_min_fail[(size_t)(j - lo - 1)]))
                                        cnt_img_bak += 2;
                                    if (j + 1 < hi) {
                                        if (img[i][j / 2] == merge_bit(bak_min_fail[(size_t)(j - lo)], bak_min_fail[(size_t)(j - lo + 1)]))
                                            cnt_img_bak += 2;
                                    } else {
                                        if (img[i][j / 2] == bak_min_fail[(size_t)(j - lo)])
                                            cnt_img_bak += 1;
                                    }
                                }
                            }
                            cnt_img_nw = cnt_img_bak = 0; // TODO: 透明块数量相同时，尽量取与 img 更像的解
                            if (cnt_fail < min_cnt_fail || cnt_tr_bak < cnt_tr_nw || (cnt_tr_bak == cnt_tr_nw && cnt_img_bak < cnt_img_nw)) {
                                min_cnt_fail = cnt_fail;
                                for (int j = lo; j < hi; j++)
                                    bak_min_fail[(size_t)(j - lo)] = decide_layer_pos(j);
                                min_times = 1;
                            } else if (cnt_fail == min_cnt_fail && cnt_tr_bak == cnt_tr_nw && cnt_img_bak == cnt_img_nw) {
                                if (0 == uniform_int_distribution<int>(0, min_times)(generator)) {
                                    min_cnt_fail = cnt_fail;
                                    for (int j = lo; j < hi; j++)
                                        bak_min_fail[(size_t)(j - lo)] = decide_layer_pos(j);
                                }
                                min_times++;
                            }
                        }
                    }
                    s.pop();
                    flags.pop_back();
                    continue;
                }
                if (s.empty())
                    break;
                s.top()++;
                int t = s.top();
                if (t > 1) {
                    s.pop();
                    flags.pop_back();
                    continue;
                }
                pair<int, bool> flag = {0, true};
                if (cur != hi)
                    decide_layer_pos(cur) = t;
                if (cur % 2 == 0) {
                    if (m2[cur / 2 - 1] != merge_bit(l1[cur / 2 - 1], l2[cur / 2])) {
                        int so_id = so[cur / 2 - 1];
                        if (so_id == 0 || so_id == 1)
                            flag = {0, false};
                        else if (sol_giveup.find({2, so_id}) == sol_giveup.end())
                            flag = {1, false};
                    }
                } else {
                    if (m1[cur / 2] != merge_bit(l1[cur / 2], l2[cur / 2])) {
                        int so_id = so[cur / 2];
                        if (so_id == 0 || so_id == 1)
                            flag = {0, false};
                        else if (sol_giveup.find({1, so_id}) == sol_giveup.end())
                            flag = {1, false};
                    }
                }
                s.push(dfs_init(cur + 1));
                flags.push_back(flag);
            }
            assert(min_cnt_fail <= hi - lo);
            assert((min_cnt_fail == 0) == (must_01_fail.find({i, lo}) == must_01_fail.end())); // 在格式块（sol==0,1）附近有可能搜不到成功匹配
            for (int j = lo; j < hi; j++)
                decide_layer_pos(j) = bak_min_fail[(size_t)(j - lo)];
            if (min_cnt_fail != 0) { // TODO: 让 m1、m2 格式信息 bit 错误数量一样多，而不是某个特别多。但是对于 mask 相同的来说，格式信息不会有错误比特
                fprintf(stderr, "!breaksuccess line %d, [%d, %d): min fail bits = %d\n", i, lo, hi, min_cnt_fail);
            }
            lo = hi + 1;
        }
        for (int j = 0; j <= n; j++) { // layer2 更多透明
        if ((j == n || m1[j] == l1[j] || sol_giveup.find({1, so[j]}) != sol_giveup.end())
                        && (j == 0 || m2[j - 1] == l1[j - 1] || sol_giveup.find({2, so[j - 1]}) != sol_giveup.end()))
                l2[j] = -1;
        }
        for (int j = 0; j < n; j++) { // layer1 更像 img
            if ((sol_giveup.find({1, so[j]}) != sol_giveup.end() || merge_bit(l1[j], l2[j]) == merge_bit(1 - l1[j], l2[j]))
                    && (sol_giveup.find({2, so[j]}) != sol_giveup.end() || merge_bit(l1[j], l2[j + 1]) == merge_bit(1 - l1[j], l2[j + 1])))
                l1[j] = img[i][j];
        }
        for (int j = 0; j <= n; j++) {
            re1[i][j] = merge_bit(l1[j], l2[j]);
            re2[i][j] = merge_bit(j == 0 ? 0 : l1[j - 1], l2[j]);
        }
    }

    // assert 检查：实际出错的 codewords 都在优化算法预料之中
    set<pair<int, int> > failed_sol_opt = sol_in_only_solution;
    vector<set<int> > err1(n_group1), err2(n_group2);
    for (size_t i = 0; i < sol_fail_cand.size(); i++)
        failed_sol_opt.insert(sol_fail_cand[i][best_sel[i]].begin(), sol_fail_cand[i][best_sel[i]].end());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (mat1[i][j] != re1[i][j] && sol[i][j] >= 2) {
                assert(failed_sol_opt.find({1, sol[i][j]}) != failed_sol_opt.end());
                err1[sol2groupno(sol[i][j], group_list1)].insert(sol[i][j]);
            }
            if (mat2[i][j] != re2[i][j + 1] && sol[i][j] >= 2) {
                assert(failed_sol_opt.find({2, sol[i][j]}) != failed_sol_opt.end());
                err2[sol2groupno(sol[i][j], group_list2)].insert(sol[i][j]);
            }
        }
    }

    clock_t clock_end = clock();
    fprintf(stderr, "m = %lld (real %lld), time: %f + %f + %f\n",
        merge_bound_thresh, max_merge_bound,
        (double)(clock_gen_start - clock_read_start) / CLOCKS_PER_SEC,
        (double)(clock_alg_start - clock_gen_start) / CLOCKS_PER_SEC,
        (double)(clock_end - clock_alg_start) / CLOCKS_PER_SEC
    );
    printf("%d %d %d %lld %f\n", 0, error_format_bits.first, error_format_bits.second, max_merge_bound, (double)(clock_end - clock_gen_start) / CLOCKS_PER_SEC);
    for (int i = 0; i < n_group1; i++)
        printf("%zu ", err1[i].size());
    printf("\n");
    for (int i = 0; i < n_group2; i++)
        printf("%zu ", err2[i].size());
    printf("\n");

    int layer1_col = n;
    int layer2_col = n + 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", mat1[i][j]);
        printf("\n");
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", mat2[i][j]);
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < layer1_col; j++)
            printf("%d ", layer1[i][j]);
        printf("\n");
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < layer2_col; j++)
            printf("%d ", layer2[i][j]);
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++)
            printf("%d ", re1[i][j]);
        printf("\n");
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++)
            printf("%d ", re2[i][j]);
        printf("\n");
    }

    return 0;
}
