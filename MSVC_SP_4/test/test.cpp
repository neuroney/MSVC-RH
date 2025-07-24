#include "MSVC_SP_4_test.h"
int main() {
    int d = 2;        // Polynomial degree
    size_t m = 100;    // Number of variables
    int t = 1;        // Number of variables
    int secpar = 128; // Security parameter
    SP4::MSVC_SP_4_TEST(d, m, t, secpar);
    return 0;
}
