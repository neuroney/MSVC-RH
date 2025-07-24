#pragma once
#include "MSVC_CH_5.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>

namespace CH5 {

TestResultData MSVC_CH_5_TIMETEST(int d, int m, int t, int secpar, int iterations = 10, bool silent = false);
// Simple timer class
SimpleTimingResult runSingleTest(int d, int m, int t, int secpar);

}