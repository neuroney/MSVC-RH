#include "MSVC_SP_4_timetest.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
namespace SP4
{
    SimpleTimingResult runSingleTest(int d, int m, int t, int secpar)
    {
        SimpleTimingResult result;
        result.success = true;
        result.maskgen_time = 0.0;     // Not used in SP4, but included for consistency
        result.reconstruct_time = 0.0; // Not used in SP4, but included for consistency

        SimpleTimer timer;

        try
        {
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
            // Vec<Fq> pi;
            // pi.SetLength(env.k);

            // for (int i = 0; i < env.k; ++i) {
            //     Vec<Fq> sigma_i;
            //     sigma_i.SetLength(env.m);
            //     for (int j = 0; j < env.m; ++j) {
            //         sigma_i[j] = sigma[i][j];
            //     }
            //     Compute(pi[i], i, ek[i], sigma_i, env);
            // }
            // result.compute_time = timer.elapsed_ms();
            Vec<Fq> pi;
            pi.SetLength(env.k);
            double max_compute_time = 0.0;
            for (int i = 0; i < env.k; ++i)
            {
                Vec<Fq> sigma_i;
                sigma_i.SetLength(env.m);
                for (int j = 0; j < env.m; ++j)
                {
                    sigma_i[j] = sigma[i][j];
                }

                timer.start();
                Compute(pi[i], i, ek[i], sigma_i, env);
                double single_compute_time = timer.elapsed_ms();

                if (single_compute_time > max_compute_time)
                {
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
            for (int i = 0; i < env.m; ++i)
            {
                X_vec[i] = X[i];
            }
            Fq direct_result = F.evaluate(X_vec);
            result.direct_compute_time = timer.elapsed_ms();

            result.total_time = result.initialize_time + result.keygen_time +
                                result.probgen_time + result.compute_time + result.verify_time;

            if (!verified)
            {
                result.success = false;
                std::cout << "Verification failed!" << std::endl;
            }

            // Optional: Check if protocol result matches direct computation
            if (verified && direct_result != verification_result)
            {
                std::cout << "Warning: Protocol result doesn't match direct computation!" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            result.success = false;
            std::cout << "Exception: " << e.what() << std::endl;
        }

        return result;
    }

    // Main timing test function
    TestResultData MSVC_SP_4_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent)
    {
        if (!silent)
        {
            std::cout << "=========================================" << std::endl;
            std::cout << "    MSVC_SP_4 Simple Timing Test        " << std::endl;
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
        }

        // Storage for results
        std::vector<SimpleTimingResult> results;

        // Run multiple tests
        if (!silent)
        {
            std::cout << "Running " << iterations << " iterations..." << std::endl;
        }
        for (int i = 0; i < iterations; ++i)
        {
            if (!silent)
            {
                std::cout << "  Iteration " << (i + 1) << "/" << iterations << "...";
            }

            SimpleTimingResult result = runSingleTest(d, m, t, secpar);

            if (result.success)
            {
                results.push_back(result);
                if (!silent)
                {
                    std::cout << " OK (" << std::fixed << std::setprecision(2)
                              << result.total_time << " ms)" << std::endl;
                }
            }
            else
            {
                if (!silent)
                {
                    std::cout << " FAILED" << std::endl;
                }
            }
        }

        TestResultData testResult = calculateAverageWithoutExtremes(results);

        // Print results
        if (!silent)
        {
            std::cout << std::endl;
            std::cout << "=========================================" << std::endl;
            std::cout << "              RESULTS                   " << std::endl;
            std::cout << "=========================================" << std::endl;
            std::cout << "Successful runs: " << testResult.successful_runs << "/" << iterations << std::endl;
            std::cout << std::endl;

            std::cout << std::fixed << std::setprecision(3);
            std::cout << "Average Times (ms):" << std::endl;
            std::cout << "  Initialize:      " << std::setw(10) << testResult.initialize_time << std::endl;
            std::cout << "  KeyGen:          " << std::setw(10) << testResult.keygen_time << std::endl;
            std::cout << "  ProbGen:         " << std::setw(10) << testResult.probgen_time << std::endl;
            std::cout << "  Compute:         " << std::setw(10) << testResult.compute_time << std::endl;
            std::cout << "  Verify:          " << std::setw(10) << testResult.verify_time << std::endl;
            std::cout << "  DirectCompute:   " << std::setw(10) << testResult.direct_compute_time << std::endl;
            std::cout << "  -----------" << std::endl;
            std::cout << "  Total (Protocol):" << std::setw(10) << testResult.total_time << std::endl;
            std::cout << std::endl;

            // Show breakdown percentages
            std::cout << "Time Breakdown (%):" << std::endl;
            std::cout << "  Initialize:      " << std::setw(6) << std::setprecision(1) << (testResult.initialize_time / testResult.total_time) * 100 << "%" << std::endl;
            std::cout << "  KeyGen:          " << std::setw(6) << std::setprecision(1) << (testResult.keygen_time / testResult.total_time) * 100 << "%" << std::endl;
            std::cout << "  ProbGen:         " << std::setw(6) << std::setprecision(1) << (testResult.probgen_time / testResult.total_time) * 100 << "%" << std::endl;
            std::cout << "  Compute:         " << std::setw(6) << std::setprecision(1) << (testResult.compute_time / testResult.total_time) * 100 << "%" << std::endl;
            std::cout << "  Verify:          " << std::setw(6) << std::setprecision(1) << (testResult.verify_time / testResult.total_time) * 100 << "%" << std::endl;
            std::cout << std::endl;

            // Show protocol vs direct computation comparison
            std::cout << "Protocol vs Direct Computation:" << std::endl;
            std::cout << "  Protocol Total:  " << std::setw(10) << std::setprecision(3) << testResult.total_time << " ms" << std::endl;
            std::cout << "  Direct Compute:  " << std::setw(10) << std::setprecision(3) << testResult.direct_compute_time << " ms" << std::endl;
            std::cout << "  Overhead Factor: " << std::setw(10) << std::setprecision(1) << (testResult.total_time / testResult.direct_compute_time) << "x" << std::endl;
            std::cout << std::endl;

            std::cout << "Test completed successfully!" << std::endl;
        }
        return testResult;
    }
}
