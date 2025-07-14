#include "helper.h"
#include "MultiPoly.h"


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