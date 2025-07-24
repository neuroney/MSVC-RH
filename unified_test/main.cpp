#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
#include <functional>
#include <chrono>
#include <sstream> // Add this include for std::stringstream

// Include our wrapper header instead of all the individual headers
#include "timetest_wrappers.h"

// Define test configurations
struct TestConfig
{
    int d;      // polynomial degree
    int m;      // number of variables
    int t;      // privacy parameter
    int secpar; // security parameter
    int iterations;
};

// Get current timestamp for logging
std::string getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Run a specific test with given configuration
TestResult runTest(const std::string &testName, const TestConfig &config)
{
    std::cout << "\n\n=========================================" << std::endl;
    std::cout << "    Starting " << testName << " Test" << std::endl;
    std::cout << "    " << getCurrentTimestamp() << std::endl;
    std::cout << "=========================================" << std::endl;

    TestResult result;
    result.algorithm_name = testName;

    try
    {
        // Note: Some tests use different parameter orders (t,m,d vs d,m,t)
        if (testName == "MSVC_RH_4")
        {
            result = RH_4_TIMETEST(config.d, config.m, config.t, config.secpar, config.iterations);
        }
        else if (testName == "MSVC_SP_4")
        {
            result = SP_4_TIMETEST(config.d, config.m, config.t, config.secpar, config.iterations);
        }
        else if (testName == "MSVC_RH_5")
        {
            result = RH_5_TIMETEST(config.d, config.m, config.t, config.secpar, config.iterations);
        }
        else if (testName == "MSVC_SP_5")
        {
            result = SP_5_TIMETEST(config.d, config.m, config.t, config.secpar, config.iterations);
        }
        else if (testName == "MSVC_CH_5")
        {
            result = CH_5_TIMETEST(config.d, config.m, config.t, config.secpar, config.iterations);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error running " << testName << ": " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error running " << testName << std::endl;
    }
    return result;
}

// Print comparison results
void printComparisonResults(const std::vector<TestResult> &results)
{
    if (results.empty())
    {
        std::cout << "No test results to compare." << std::endl;
        return;
    }

    std::cout << "\n\n=============================================================================================" << std::endl;
    std::cout << "        COMPARATIVE RESULTS              " << std::endl;
    std::cout << "=============================================================================================" << std::endl;

    // Print header
    std::cout << std::left << std::setw(12) << "Algorithm"
              << std::right << std::setw(12) << "Total (ms)"
              << std::setw(12) << "KeyGen (ms)"
              <<std::setw(12) << "ProbGen (ms)"
              << std::setw(12) << "MaskGen (ms)"
              << std::setw(12) << "Compute (ms)"
              << std::setw(12) << "Verify (ms)"
              << std::setw(12) << "Reconst (ms)"
              << std::setw(12) << "Direct (ms)"
              << std::setw(12) << "Overhead" << std::endl;

    std::cout << std::string(96, '-') << std::endl;

    // Define the custom order for display
    std::vector<std::string> displayOrder = {"SP_4", "RH_4", "SP_5", "CH_5", "RH_5"};

    // Find and print algorithms in the specified order
    bool dividerPrinted = false;

    for (const auto &algoName : displayOrder)
    {
        // Print divider after RH_4
        if (algoName == "SP_5" && !dividerPrinted)
        {
            std::cout << std::string(96, '-') << std::endl;
            dividerPrinted = true;
        }

        // Find the algorithm in the results
        auto it = std::find_if(results.begin(), results.end(),
                               [&algoName](const TestResult &result)
                               {
                                   return result.algorithm_name.find(algoName) != std::string::npos;
                               });

        if (it != results.end())
        {
            const TestResult &result = *it;
            std::cout << std::fixed << std::setprecision(3);
            std::cout << std::left << std::setw(12) << result.algorithm_name.substr(5) // Remove "MSVC_" prefix
                      << std::right << std::setw(12) << result.total_time
                      << std::setw(12) << result.keygen_time
                      << std::setw(12) << result.probgen_time
                      << std::setw(12) << (result.algorithm_name.find("RH") != std::string::npos ? result.maskgen_time : 0.000)
                      << std::setw(12) << result.compute_time
                      << std::setw(12) << result.verify_time
                      << std::setw(12) << (result.algorithm_name.find("RH") != std::string::npos ? result.reconstruct_time : 0.000)
                      << std::setw(12) << result.direct_compute_time
                      << std::setw(12) << std::setprecision(1) << result.overhead_factor << "x"
                      << std::endl;
        }
    }

    // Add a note about maskGen and Reconstruct
    std::cout << "\nNote: MaskGen and Reconstruct columns only apply to RH4 and RH5 algorithms." << std::endl;
    std::cout << "      SP4, SP5, and CH5 algorithms do not include these steps." << std::endl;
}

// Print usage instructions
void printUsage(const char *programName)
{
    std::cout << "Usage: " << programName << " [options] [tests]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -d <value>       Set polynomial degree (default: 2)" << std::endl;
    std::cout << "  -m <value>       Set number of variables (default: 100)" << std::endl;
    std::cout << "  -t <value>       Set privacy parameter (default: 1)" << std::endl;
    std::cout << "  -secpar <value>  Set security parameter (default: 128)" << std::endl;
    std::cout << "  -iter <value>    Set number of iterations (default: 10)" << std::endl;
    std::cout << "  -all             Run all tests" << std::endl;
    std::cout << "  -h, --help       Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Available tests:" << std::endl;
    std::cout << "  CH5              Run MSVC_CH_5 timetest" << std::endl;
    std::cout << "  RH4              Run MSVC_RH_4 timetest" << std::endl;
    std::cout << "  RH5              Run MSVC_RH_5 timetest" << std::endl;
    std::cout << "  SP4              Run MSVC_SP_4 timetest" << std::endl;
    std::cout << "  SP5              Run MSVC_SP_5 timetest" << std::endl;
}

int main(int argc, char *argv[])
{
    // Default configuration
    TestConfig config = {
        .d = 2,          // polynomial degree
        .m = 100,        // number of variables
        .t = 1,          // privacy parameter
        .secpar = 128,   // security parameter
        .iterations = 10 // iterations
    };

    // Map of available tests
    std::map<std::string, std::string> tests = {
        {"CH5", "MSVC_CH_5"},
        {"RH4", "MSVC_RH_4"},
        {"RH5", "MSVC_RH_5"},
        {"SP4", "MSVC_SP_4"},
        {"SP5", "MSVC_SP_5"}};

    // Track which tests to run
    std::vector<std::string> testsToRun;
    bool runAllTests = false;

    // Parse command line arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (arg == "-d" && i + 1 < argc)
        {
            config.d = std::stoi(argv[++i]);
        }
        else if (arg == "-m" && i + 1 < argc)
        {
            config.m = std::stoi(argv[++i]);
        }
        else if (arg == "-t" && i + 1 < argc)
        {
            config.t = std::stoi(argv[++i]);
        }
        else if (arg == "-secpar" && i + 1 < argc)
        {
            config.secpar = std::stoi(argv[++i]);
        }
        else if (arg == "-iter" && i + 1 < argc)
        {
            config.iterations = std::stoi(argv[++i]);
        }
        else if (arg == "-all")
        {
            runAllTests = true;
        }
        else if (tests.find(arg) != tests.end())
        {
            testsToRun.push_back(arg);
        }
        else
        {
            std::cerr << "Unknown option or test: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    // If no tests specified and not running all, show usage
    if (testsToRun.empty() && !runAllTests)
    {
        std::cout << "No tests specified." << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    // Print configuration
    std::cout << "=============================================================================================" << std::endl;
    std::cout << "    Unified Timetest Runner" << std::endl;
    std::cout << "=============================================================================================" << std::endl;
    std::cout << "Configuration:" << std::endl;
    std::cout << "  - Polynomial degree (d): " << config.d << std::endl;
    std::cout << "  - Number of variables (m): " << config.m << std::endl;
    std::cout << "  - Privacy parameter (t): " << config.t << std::endl;
    std::cout << "  - Security parameter (secpar): " << config.secpar << std::endl;
    std::cout << "  - Iterations: " << config.iterations << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<TestResult> testResults;

    // Run selected tests
    if (runAllTests)
    {
        for (const auto &[key, testName] : tests)
        {
            TestResult result = runTest(testName, config);
            testResults.push_back(result);
        }
    }
    else
    {
        for (const auto &testKey : testsToRun)
        {
            const auto &testName = tests[testKey];
            TestResult result = runTest(testName, config);
            testResults.push_back(result);
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    if (testResults.size() > 1)
    {
        printComparisonResults(testResults);
    }

    std::cout << "\n\n=============================================================================================" << std::endl;
    std::cout << "All tests completed in " << elapsed.count() << " seconds" << std::endl;
    std::cout << getCurrentTimestamp() << std::endl;
    std::cout << "=============================================================================================" << std::endl;

    return 0;
}