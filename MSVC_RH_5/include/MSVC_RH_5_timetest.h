#pragma once
#include "MSVC_RH_5.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
namespace RH5 {

// Function to run simple timing tests for MSVC_RH_5
TestResultData MSVC_RH_5_TIMETEST(int d, int m, int t, int secpar, int iterations = 10, bool silent = false);

SimpleTimingResult runSingleTest(int d, int m, int t, int secpar);
}