#include "MSVC_CH_5.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>

// Simple timer class
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

// Generate simple random input
Vec<Fq> generateSimpleInput(int size) {
    Vec<Fq> X;
    X.SetLength(size);
    for (int i = 0; i < size; ++i) {
        X[i] = to_ZZ_p(i + 1); // Use sequential integers starting from 1
    }
    return X;
}

// Run a single test and return timing results
struct SimpleTimingResult {
    double initialize_time;
    double keygen_time;
    double probgen_time;
    double compute_time;
    double verify_time;
    double direct_compute_time;
    double total_time;
    bool success;
};

SimpleTimingResult runSingleTest(int d, int m, int t, int secpar) {
    SimpleTimingResult result;
    result.success = true;
    
    SimpleTimer timer;
    
    try {
        // Step 1: Initialize
        timer.start();
        Env env;
        Initialize(env, t, secpar);
        result.initialize_time = timer.elapsed_ms();
        
        // Step 2: Create polynomial
        MultiPoly<Fq> F = generateFullPoly(m, d, random_ZZ_p());
        
        // Step 3: KeyGen
        timer.start();
        PK_F pk;
        VK_F vk_f;
        EK_F ek;
        KeyGen(pk, vk_f, ek, env, F);
        result.keygen_time = timer.elapsed_ms();
        
        // Step 4: Generate problem instance
        Vec<Fq> X = generateSimpleInput(env.m);
        
        timer.start();
        Mat<Fq> sigma;
        VK_X vk_x;
        ProbGen(vk_x, sigma, env, pk, X);
        result.probgen_time = timer.elapsed_ms();
        
        // Step 5: Compute proofs
        // timer.start();
        // Mat<Fq> pi;
        // pi.SetDims(env.k, 2);
        
        // for (int i = 0; i < env.k; ++i) {
        //     Vec<Fq> sigma_i;
        //     sigma_i.SetLength(env.m + 1);
        //     for (int j = 0; j < env.m + 1; ++j) {
        //         sigma_i[j] = sigma[i][j];
        //     }
        //     Compute(pi[i], i, ek[i], sigma_i, env);
        // }
        // result.compute_time = timer.elapsed_ms();
        Mat<Fq> pi;
        pi.SetDims(env.k, 2);
        
        double max_compute_time = 0.0;
        for (int i = 0; i < env.k; ++i) {
            Vec<Fq> sigma_i;
            sigma_i.SetLength(env.m + 3);
            for (int j = 0; j < env.m + 3; ++j) {
                sigma_i[j] = sigma[i][j];
            }
            
            timer.start();
            Compute(pi[i], i, ek[i], sigma_i, env);
            double single_compute_time = timer.elapsed_ms();
            
            if (single_compute_time > max_compute_time) {
                max_compute_time = single_compute_time;
            }
        }
        result.compute_time = max_compute_time;
        
        // Step 6: Verify
        timer.start();
        Fq verification_result;
        bool verified = Verify(verification_result, vk_f, vk_x, pi, env);
        result.verify_time = timer.elapsed_ms();
        
        // Step 7: Direct compute for comparison
        timer.start();
        std::vector<Fq> X_vec(env.m);
        for (int i = 0; i < env.m; ++i) {
            X_vec[i] = X[i];
        }
        Fq direct_result = F.evaluate(X_vec);
        result.direct_compute_time = timer.elapsed_ms();
        
        result.total_time = result.initialize_time + result.keygen_time + 
                           result.probgen_time + result.compute_time + result.verify_time;
        
        if (!verified) {
            result.success = false;
            std::cout << "Verification failed!" << std::endl;
        }
        
        // Optional: Check if protocol result matches direct computation
        if (verified && direct_result != verification_result) {
            std::cout << "Warning: Protocol result doesn't match direct computation!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return result;
}

// Main timing test function
void MSVC_CH_5_TIMETEST(int d, int m, int t, int secpar, int iterations = 10) {
    std::cout << "=========================================" << std::endl;
    std::cout << "    MSVC_CH_5 Simple Timing Test        " << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << std::endl;
    
    // Test configuration
    std::cout << "Configuration:" << std::endl;
    std::cout << "  Privacy (t): " << t << std::endl;
    std::cout << "  Fq size (secpar): " << secpar << std::endl;
    std::cout << "  Polynomial degree (d): " << d << std::endl;
    std::cout << "  Number of variables (m): " << m << std::endl;
    std::cout << "  Iterations: " << iterations << std::endl;
    std::cout << std::endl;

    // Storage for results
    std::vector<SimpleTimingResult> results;
    
    // Run multiple tests
    std::cout << "Running " << iterations << " iterations..." << std::endl;
    for (int i = 0; i < iterations; ++i) {
        std::cout << "  Iteration " << (i + 1) << "/" << iterations << "...";
        
        SimpleTimingResult result = runSingleTest(d, m, t, secpar);
        
        if (result.success) {
            results.push_back(result);
            std::cout << " OK (" << std::fixed << std::setprecision(2) 
                      << result.total_time << " ms)" << std::endl;
        } else {
            std::cout << " FAILED" << std::endl;
        }
    }
    
    if (results.empty()) {
        std::cout << "No successful runs!" << std::endl;
        return;
    }
    
    // Calculate averages
    double avg_initialize = 0, avg_keygen = 0, avg_probgen = 0, avg_compute = 0, avg_verify = 0, avg_direct_compute = 0, avg_total = 0;
    
    for (const auto& r : results) {
        avg_initialize += r.initialize_time;
        avg_keygen += r.keygen_time;
        avg_probgen += r.probgen_time;
        avg_compute += r.compute_time;
        avg_verify += r.verify_time;
        avg_direct_compute += r.direct_compute_time;
        avg_total += r.total_time;
    }
    
    size_t count = results.size();
    avg_initialize /= count;
    avg_keygen /= count;
    avg_probgen /= count;
    avg_compute /= count;
    avg_verify /= count;
    avg_direct_compute /= count;
    avg_total /= count;
    
    // Print results
    std::cout << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "              RESULTS                   " << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Successful runs: " << count << "/" << iterations << std::endl;
    std::cout << std::endl;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Average Times (ms):" << std::endl;
    std::cout << "  Initialize:      " << std::setw(10) << avg_initialize << std::endl;
    std::cout << "  KeyGen:          " << std::setw(10) << avg_keygen << std::endl;
    std::cout << "  ProbGen:         " << std::setw(10) << avg_probgen << std::endl;
    std::cout << "  Compute:         " << std::setw(10) << avg_compute << std::endl;
    std::cout << "  Verify:          " << std::setw(10) << avg_verify << std::endl;
    std::cout << "  DirectCompute:   " << std::setw(10) << avg_direct_compute << std::endl;
    std::cout << "  -----------" << std::endl;
    std::cout << "  Total (Protocol):" << std::setw(10) << avg_total << std::endl;
    std::cout << std::endl;
    
    // Show breakdown percentages
    std::cout << "Time Breakdown (%):" << std::endl;
    std::cout << "  Initialize:      " << std::setw(6) << std::setprecision(1) << (avg_initialize/avg_total)*100 << "%" << std::endl;
    std::cout << "  KeyGen:          " << std::setw(6) << std::setprecision(1) << (avg_keygen/avg_total)*100 << "%" << std::endl;
    std::cout << "  ProbGen:         " << std::setw(6) << std::setprecision(1) << (avg_probgen/avg_total)*100 << "%" << std::endl;
    std::cout << "  Compute:         " << std::setw(6) << std::setprecision(1) << (avg_compute/avg_total)*100 << "%" << std::endl;
    std::cout << "  Verify:          " << std::setw(6) << std::setprecision(1) << (avg_verify/avg_total)*100 << "%" << std::endl;
    std::cout << std::endl;
    
    // Show protocol vs direct computation comparison
    std::cout << "Protocol vs Direct Computation:" << std::endl;
    std::cout << "  Protocol Total:  " << std::setw(10) << std::setprecision(3) << avg_total << " ms" << std::endl;
    std::cout << "  Direct Compute:  " << std::setw(10) << std::setprecision(3) << avg_direct_compute << " ms" << std::endl;
    std::cout << "  Overhead Factor: " << std::setw(10) << std::setprecision(1) << (avg_total/avg_direct_compute) << "x" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Test completed successfully!" << std::endl;
}

int main() {
    int d = 2;        // Polynomial degree
    int m = 100;       // Number of variables
    int t = 1;        // Number of variables
    int secpar = 32; // Security parameter
    int iterations = 100; // Number of iterations for averaging
    MSVC_CH_5_TIMETEST(d, m, t, secpar, iterations);
    return 0;
}
