#include "MSVC_RH_4_test.h"
int main() {
    int t = 1;
    int m = 10; // Number of polynomials
    int d = 2; // Degree of polynomials
    int secpar = 128; // Security parameter
    MSVC_RH_4_TEST(t, m, d, secpar);
    return 0;
}
