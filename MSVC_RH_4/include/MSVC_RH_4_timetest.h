#pragma once
#include "MSVC_RH_4.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
namespace RH4 {

// Function to run simple timing tests for MSVC_RH_4
TestResultData MSVC_RH_4_TIMETEST(int d, int m, int t, int secpar, int iterations = 10, bool silent = false);

SimpleTimingResult runSingleTest(int t, int m, int d, int secpar);
}