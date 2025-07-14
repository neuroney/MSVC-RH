#pragma once
#include "MSVC_RH_5.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>

// Function to run simple timing tests for MSVC_RH_5
void MSVC_RH_5_TIMETEST();

// Simple timer utility class
class SimpleTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    
public:
    void start();
    double elapsed_ms();
};

// Simple timing result structure
struct SimpleTimingResult {
    double initialize_time;
    double keygen_time;
    double probgen_time;
    double maskgen_time;
    double compute_time;
    double verify_time;
    double reconstruct_time;
    double direct_compute_time;
    double total_time;
    bool success;
};

// Utility functions
Vec<Fq> generateSimpleInput(int size);
SimpleTimingResult runSingleTest(int d, int m, int t, int secpar);
