#include "MSVC_RH_5_test.h"
int main() {
    int d = 2;        // Polynomial degree
    size_t m = 2000;    // Number of variables
    int t = 1;        // Number of variables
    int secpar = 128; // Security parameter
    RH5::MSVC_RH_5_TEST(d, m, t, secpar);
    return 0;
}
