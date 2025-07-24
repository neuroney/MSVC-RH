#include "helper.h"
#include "MultiPoly.h"


// Print polynomials using MultiPoly's toString method
void printMultiPoly(const MultiPoly<Fq>& poly, const std::string& name) {
    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "Degree: " << poly.maxDegree() << ", Variables: " << poly.varCount() << std::endl;
    
    std::vector<std::string> varNames;
    for (size_t i = 0; i < poly.varCount(); ++i) {
        varNames.push_back("x_" + std::to_string(i));
    }
    
    std::cout << poly.toString(varNames) << std::endl << std::endl;
}

// Print ZZ_pX polynomials, referencing NTL format
void printZZ_pX(const ZZ_pX& poly, const std::string& name) {
    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "Degree: " << deg(poly) << std::endl;
    std::cout << "[";
    for (int i = 0; i <= deg(poly); ++i) {
        std::cout << coeff(poly, i);
        if (i < deg(poly)) std::cout << " ";
    }
    std::cout << "]" << std::endl << std::endl;
}


ZZ compute_g_fa(const ZZ_pX &f,
                  const Vec<ZZ> &gPows,
                  const ZZ &g, 
                const ZZ &mod)
{
    long d = deg(f);
    long provided = static_cast<long>(gPows.length());
    // Check if polynomial degree exceeds precomputed powers length
    if (d > provided) {
        throw std::invalid_argument(
            "Polynomial degree (" + std::to_string(d) +
            ") exceeds number of precomputed powers (" +
            std::to_string(provided) + ")");
    }

    // Start with identity element (g^0 = 1)
    ZZ result(1);

    // Handle constant term f0: g^{f0}
    ZZ_p f0 = coeff(f, 0);
    if (f0 != 0) {
        PowerMod(result, g, rep(f0),  mod);
    }

    // For each i=1...d, multiply by (g^{a^i})^{f_i}
    for (long i = 1; i <= d; ++i)
    {
        ZZ_p ci = coeff(f, i);
        if (ci == 0)
            continue;
        // gPows[i-1] == g^{a^i}, so we want (g^{a^i})^{ci}
        result = result * PowerMod(gPows[i - 1], rep(ci), mod) % mod;
    }
    return result;
}

ZZ FindGen(const ZZ& p, const ZZ& q, long max_trials) {
    ZZ a, leg, g;
    for (long trials = 0; trials < max_trials; trials++) {
        // 1) 随机选 a ∈ [2, q-2]
        a = RandomBnd(q - 3) + 2;
        // 2) 计算勒让德符号：leg ≡ a^p mod q
        leg = PowerMod(a, p, q);
        // 如果 a 是二次非剩余（leg ≡ -1），则 ord(a)=2p
        if (leg == q - 1) {
            // g = a^2 mod q, 则 ord(g) = p
            g = PowerMod(a, 2, q);
            if (g != 1)  // 排除退化 g=1
                return g;
        }
    }
    // 尝试了 max_trials 次仍然失败
    return ZZ(0);
}

void DataProcess(double &mean, double &stdev, double *Time, int cyctimes)
{
    double temp;
    double sum = 0;
    for (int i = 0; i < cyctimes; i++)
    {
        sum = sum + Time[i];
    }
    mean = sum / cyctimes;
    double temp_sum = 0;
    for (int i = 0; i < cyctimes; i++)
    {
        temp = mean - Time[i];
        temp = temp * temp;
        temp_sum = temp_sum + temp;
    }
    stdev = sqrt(temp_sum / cyctimes);
    stdev = stdev / mean;
}

ZZ PRF_ZZ(const int &prfkey, const ZZ &mmod)
{
    ZZ res;
    SetSeed(ZZ(prfkey));
    RandomBnd(res, mmod);
    return res;
}

// Function to calculate average timing results after removing max and min values
TestResultData calculateAverageWithoutExtremes(const std::vector<SimpleTimingResult>& results) {
    TestResultData testResult = {0};
    testResult.total_runs = results.size();
    testResult.successful_runs = results.size();
    
    if (results.empty()) {
        std::cout << "No successful runs!" << std::endl;
        return testResult;
    }
    
    // Check if we have enough results to remove max and min
    if (results.size() <= 2) {
        std::cout << "Warning: Not enough successful runs to remove max and min values. "
                  << "Using all available data." << std::endl;
        
        // Calculate regular averages if we have less than 3 results
        double avg_initialize = 0, avg_keygen = 0, avg_probgen = 0, avg_maskgen = 0;
        double avg_compute = 0, avg_verify = 0, avg_reconstruct = 0, avg_direct_compute = 0, avg_total = 0;

        for (const auto &r : results) {
            avg_initialize += r.initialize_time;
            avg_keygen += r.keygen_time;
            avg_probgen += r.probgen_time;
            avg_maskgen += r.maskgen_time;
            avg_compute += r.compute_time;
            avg_verify += r.verify_time;
            avg_reconstruct += r.reconstruct_time;
            avg_direct_compute += r.direct_compute_time;
            avg_total += r.total_time;
        }

        size_t count = results.size();
        avg_initialize /= count;
        avg_keygen /= count;
        avg_probgen /= count;
        avg_maskgen /= count;
        avg_compute /= count;
        avg_verify /= count;
        avg_reconstruct /= count;
        avg_direct_compute /= count;
        avg_total /= count;

        testResult.initialize_time = avg_initialize;
        testResult.keygen_time = avg_keygen;
        testResult.probgen_time = avg_probgen;
        testResult.maskgen_time = avg_maskgen;
        testResult.compute_time = avg_compute;
        testResult.verify_time = avg_verify;
        testResult.reconstruct_time = avg_reconstruct;
        testResult.direct_compute_time = avg_direct_compute;
        testResult.total_time = avg_total;
        testResult.overhead_factor = (avg_direct_compute > 0) ? (avg_total / avg_direct_compute) : 0;
        
        return testResult;
    }
    
    // Extract values from results into separate vectors for each metric
    std::vector<double> initialize_times, keygen_times, probgen_times, maskgen_times;
    std::vector<double> compute_times, verify_times, reconstruct_times, direct_compute_times, total_times;
    
    for (const auto& r : results) {
        initialize_times.push_back(r.initialize_time);
        keygen_times.push_back(r.keygen_time);
        probgen_times.push_back(r.probgen_time);
        maskgen_times.push_back(r.maskgen_time);
        compute_times.push_back(r.compute_time);
        verify_times.push_back(r.verify_time);
        reconstruct_times.push_back(r.reconstruct_time);
        direct_compute_times.push_back(r.direct_compute_time);
        total_times.push_back(r.total_time);
    }
    
    // Sort each vector to easily find and remove max and min
    std::sort(initialize_times.begin(), initialize_times.end());
    std::sort(keygen_times.begin(), keygen_times.end());
    std::sort(probgen_times.begin(), probgen_times.end());
    std::sort(maskgen_times.begin(), maskgen_times.end());
    std::sort(compute_times.begin(), compute_times.end());
    std::sort(verify_times.begin(), verify_times.end());
    std::sort(reconstruct_times.begin(), reconstruct_times.end());
    std::sort(direct_compute_times.begin(), direct_compute_times.end());
    std::sort(total_times.begin(), total_times.end());
    
    // Calculate sums (excluding first and last elements - min and max)
    double sum_initialize = 0, sum_keygen = 0, sum_probgen = 0, sum_maskgen = 0;
    double sum_compute = 0, sum_verify = 0, sum_reconstruct = 0, sum_direct_compute = 0, sum_total = 0;
    
    for (size_t i = 1; i < initialize_times.size() - 1; ++i) {
        sum_initialize += initialize_times[i];
        sum_keygen += keygen_times[i];
        sum_probgen += probgen_times[i];
        sum_maskgen += maskgen_times[i];
        sum_compute += compute_times[i];
        sum_verify += verify_times[i];
        sum_reconstruct += reconstruct_times[i];
        sum_direct_compute += direct_compute_times[i];
        sum_total += total_times[i];
    }
    
    // Calculate averages (excluding max and min)
    size_t count_without_extremes = results.size() - 2;
    
    testResult.initialize_time = sum_initialize / count_without_extremes;
    testResult.keygen_time = sum_keygen / count_without_extremes;
    testResult.probgen_time = sum_probgen / count_without_extremes;
    testResult.maskgen_time = sum_maskgen / count_without_extremes;
    testResult.compute_time = sum_compute / count_without_extremes;
    testResult.verify_time = sum_verify / count_without_extremes;
    testResult.reconstruct_time = sum_reconstruct / count_without_extremes;
    testResult.direct_compute_time = sum_direct_compute / count_without_extremes;
    testResult.total_time = sum_total / count_without_extremes;
    
    // Calculate overhead factor
    testResult.overhead_factor = (testResult.direct_compute_time > 0) ? 
                                 (testResult.total_time / testResult.direct_compute_time) : 0;
    
    return testResult;
}


// Generate simple random input
Vec<Fq> generateSimpleInput(int size)
{
    Vec<Fq> X;
    X.SetLength(size);
    for (int i = 0; i < size; ++i)
    {
        X[i] = to_ZZ_p(i + 1); // Use sequential integers starting from 1
    }
    return X;
}