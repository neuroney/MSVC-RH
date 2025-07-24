#include "MSVC_CH_5_timetest.h"
int main()
{
    int d = 2;            // Polynomial degree
    size_t m = 100;       // Number of variables
    int t = 1;            // Number of variables
    int secpar = 128;     // Security parameter
    int iterations = 100; // Number of iterations for averaging
    CH5::MSVC_CH_5_TIMETEST(d, m, t, secpar, iterations);
    return 0;
}
