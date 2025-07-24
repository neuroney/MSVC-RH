#pragma once
#include "MSVC_SP_4.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
namespace SP4
{
    // Function to run simple timing tests for MSVC_SP_4
    TestResultData MSVC_SP_4_TIMETEST(int d, int m, int t, int secpar, int iterations = 10, bool silent = false);
    SimpleTimingResult runSingleTest(int d, int m, int t, int secpar);
}