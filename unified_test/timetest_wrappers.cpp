#include "timetest_wrappers.h"

TestResult CH_5_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent)
{
    TestResultData result = CH5::MSVC_CH_5_TIMETEST(d, m, t, secpar, iterations, silent);

    TestResult testResult;
    testResult.algorithm_name = "MSVC_CH_5";
    testResult.initialize_time = result.initialize_time;
    testResult.keygen_time = result.keygen_time;
    testResult.probgen_time = result.probgen_time;
    testResult.compute_time = result.compute_time;
    testResult.verify_time = result.verify_time;
    testResult.direct_compute_time = result.direct_compute_time;
    testResult.total_time = result.total_time;
    testResult.overhead_factor = result.overhead_factor;
    testResult.successful_runs = result.successful_runs;
    testResult.total_runs = result.total_runs;

    testResult.maskgen_time = result.maskgen_time;
    testResult.reconstruct_time = result.reconstruct_time;

    return testResult;
}

TestResult RH_4_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent)
{
    TestResultData result = RH4::MSVC_RH_4_TIMETEST(d, m, t, secpar, iterations, silent);

    TestResult testResult;
    testResult.algorithm_name = "MSVC_RH_4";
    testResult.initialize_time = result.initialize_time;
    testResult.keygen_time = result.keygen_time;
    testResult.probgen_time = result.probgen_time;
    testResult.compute_time = result.compute_time;
    testResult.verify_time = result.verify_time;
    testResult.direct_compute_time = result.direct_compute_time;
    testResult.total_time = result.total_time;
    testResult.overhead_factor = result.overhead_factor;
    testResult.successful_runs = result.successful_runs;
    testResult.total_runs = result.total_runs;

    testResult.maskgen_time = result.maskgen_time;
    testResult.reconstruct_time = result.reconstruct_time;

    return testResult;
}

TestResult RH_5_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent)
{
    TestResultData result = RH5::MSVC_RH_5_TIMETEST(d, m, t, secpar, iterations, silent);

    TestResult testResult;
    testResult.algorithm_name = "MSVC_RH_5";
    testResult.initialize_time = result.initialize_time;
    testResult.keygen_time = result.keygen_time;
    testResult.probgen_time = result.probgen_time;
    testResult.compute_time = result.compute_time;
    testResult.verify_time = result.verify_time;
    testResult.direct_compute_time = result.direct_compute_time;
    testResult.total_time = result.total_time;
    testResult.overhead_factor = result.overhead_factor;
    testResult.successful_runs = result.successful_runs;
    testResult.total_runs = result.total_runs;

    testResult.maskgen_time = result.maskgen_time;
    testResult.reconstruct_time = result.reconstruct_time;

    return testResult;
}

TestResult SP_4_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent)
{
    TestResultData result = SP4::MSVC_SP_4_TIMETEST(d, m, t, secpar, iterations, silent);

    TestResult testResult;
    testResult.algorithm_name = "MSVC_SP_4";
    testResult.initialize_time = result.initialize_time;
    testResult.keygen_time = result.keygen_time;
    testResult.probgen_time = result.probgen_time;
    testResult.compute_time = result.compute_time;
    testResult.verify_time = result.verify_time;
    testResult.direct_compute_time = result.direct_compute_time;
    testResult.total_time = result.total_time;
    testResult.overhead_factor = result.overhead_factor;
    testResult.successful_runs = result.successful_runs;
    testResult.total_runs = result.total_runs;

    testResult.maskgen_time = 0;
    testResult.reconstruct_time = 0;

    return testResult;
}

TestResult SP_5_TIMETEST(int d, int m, int t, int secpar, int iterations, bool silent)
{
    TestResultData result = SP5::MSVC_SP_5_TIMETEST(d, m, t, secpar, iterations, silent);

    TestResult testResult;
    testResult.algorithm_name = "MSVC_SP_5";
    testResult.initialize_time = result.initialize_time;
    testResult.keygen_time = result.keygen_time;
    testResult.probgen_time = result.probgen_time;
    testResult.compute_time = result.compute_time;
    testResult.verify_time = result.verify_time;
    testResult.direct_compute_time = result.direct_compute_time;
    testResult.total_time = result.total_time;
    testResult.overhead_factor = result.overhead_factor;
    testResult.successful_runs = result.successful_runs;
    testResult.total_runs = result.total_runs;

    testResult.maskgen_time = 0;
    testResult.reconstruct_time = 0;

    return testResult;
}