#pragma once
#include <string>
#include <vector>
#include "MSVC_CH_5_timetest.h"
#include "MSVC_RH_4_timetest.h"
#include "MSVC_RH_5_timetest.h"
#include "MSVC_SP_4_timetest.h"
#include "MSVC_SP_5_timetest.h"

// Test result structure
struct TestResult {
    std::string algorithm_name;
    double initialize_time;
    double keygen_time;
    double probgen_time;
    double maskgen_time;  // RH variants only
    double compute_time;
    double verify_time;
    double reconstruct_time;  // RH variants only
    double direct_compute_time;
    double total_time;
    double overhead_factor;
    int successful_runs;
    int total_runs;
};

// Forward declarations for timetest functions

    TestResult CH_5_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent = true);



    TestResult RH_4_TIMETEST(int t, int m, int d, int secpar, int iterations, bool silent = true);



    TestResult RH_5_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent = true);


    TestResult SP_4_TIMETEST(int t, int m, int d, int secpar, int iterations, bool silent = true);



    TestResult SP_5_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent = true);
