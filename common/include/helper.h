#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "NTL/ZZ.h"
#include "NTL/ZZX.h"
#include <array>
#include "MultiPoly.h"

using namespace std;
using namespace NTL;
using Fq = ZZ_p;

struct TestResultData {
    double initialize_time;
    double keygen_time;
    double probgen_time;
    double maskgen_time;
    double compute_time;
    double verify_time;
    double reconstruct_time;
    double direct_compute_time;
    double total_time;
    double overhead_factor;
    int successful_runs;
    int total_runs;
};

void DataProcess(double &mean, double &stdev, double *Time, int cyctimes);
ZZ PRF_ZZ(const int &prfkey, const ZZ &mmod);

ZZ FindGen(const ZZ &p, const ZZ &q, long max_trials = 10000);

ZZ compute_g_fa(const ZZ_pX &f,
                  const Vec<ZZ> &gPows,
                  const ZZ &g,
                  const ZZ &mod);


class SimpleTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    double elapsed_ms() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0; // Convert to milliseconds
    }
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

TestResultData calculateAverageWithoutExtremes(const std::vector<SimpleTimingResult>& results);

// Print polynomials using MultiPoly's toString method
void printMultiPoly(const MultiPoly<Fq>& poly, const std::string& name);

// Print ZZ_pX polynomials, referencing NTL format
void printZZ_pX(const ZZ_pX& poly, const std::string& name);


// Helper function to print vectors
template<typename T>
void printVector(const Vec<T>& vec, const std::string& name) {
    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "Length: " << vec.length() << std::endl;
    std::cout << "[";
    for (int i = 0; i < vec.length(); ++i) {
        std::cout << vec[i];
        if (i < vec.length() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl << std::endl;
}

// Helper function to print matrices
template<typename T>
void printMatrix(const Mat<T>& mat, const std::string& name) {
    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "Dimensions: " << mat.NumRows() << " x " << mat.NumCols() << std::endl;
    std::cout << "[" << std::endl;
    for (int i = 0; i < mat.NumRows(); ++i) {
        std::cout << "  [";
        for (int j = 0; j < mat.NumCols(); ++j) {
            std::cout << mat[i][j];
            if (j < mat.NumCols() - 1) std::cout << ", ";
        }
        std::cout << "]";
        if (i < mat.NumRows() - 1) std::cout << ",";
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl << std::endl;
}
