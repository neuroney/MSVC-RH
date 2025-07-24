#include "MSVC_SP_4_timetest.h"
int main()
{
    int d = 2;            // Polynomial degree
    size_t m = 100;       // Number of variables
    int t = 1;            // Number of variables
    int secpar = 128;     // Security parameter
    int iterations = 100; // Number of test iterations
    SP4::MSVC_SP_4_TIMETEST(d, m, t, secpar, iterations);
    return 0;
}