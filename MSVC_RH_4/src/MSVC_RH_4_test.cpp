#include "MSVC_RH_4_test.h"
namespace RH4 {
// Helper function to print environment parameters
void printEnv(const Env &env)
{
    std::cout << "=== Environment Parameters ===" << std::endl;
    std::cout << "secpar: " << env.secpar << std::endl;
    std::cout << "t: " << env.t << std::endl;
    std::cout << "m: " << env.m << std::endl;
    std::cout << "d: " << env.d << std::endl;
    std::cout << "k: " << env.k << std::endl;
    std::cout << "ord: " << env.ord << std::endl;
    std::cout << "fq: " << env.fq << std::endl;
    std::cout << "g: " << env.g << std::endl;
    std::cout << std::endl;
}

void MSVC_RH_4_TEST(int d, int m, int t, int secpar)
{
    std::cout << "===================================================" << std::endl;
    std::cout << "MSVC_RH_4 Protocol Test with d=2, t=1, secpar=128" << std::endl;
    std::cout << "===================================================" << std::endl;
    std::cout << std::endl;

    // Step 1: Initialize the environment
    std::cout << "Step 1: Initializing environment..." << std::endl;
    Env env;
    Initialize(env, t, secpar);

    // Step 2: Create the polynomial F
    std::cout << "Step 2: Creating polynomial F..." << std::endl;
    MultiPoly<Fq> F = generateFullPoly(m, d, random_ZZ_p());
    vector<string> varNames;
    for (int i = 0; i < m; ++i)
    {
        varNames.push_back("x_" + std::to_string(i));
    }
    std::cout << "F = " << F.toString(varNames) << std::endl
              << std::endl;

    // F.addTerm({2, 0, 1}, to_ZZ_p(30));  // x^2
    // F.addTerm({0, 1, 1 }, to_ZZ_p(20));  // y
    // F.addTerm({0, 0, 0}, to_ZZ_p(10));  // constant term
    // std::cout << "F = " << F.toString({"x", "y", "z"}) << std::endl << std::endl;

    // Step 3: Generate keys
    std::cout << "Step 3: Generating keys..." << std::endl;
    PK_F pk;
    VK_F vk_f;
    EK_F ek;
    KeyGen(pk, vk_f, ek, env, F);

    printEnv(env);

    std::cout << "Key generation completed." << std::endl;
    std::cout << "Public Key (pk) size: " << pk.length() << std::endl;
    for (int i = 0; i < pk.length(); ++i)
    {
        std::cout << "Public Key Polynomial " << i << " = ";
        std::cout << pk[i].toString({"u"}) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Verification Key (vk_f) contains:" << std::endl;
    for (int i = 0; i < vk_f.ell.length(); ++i)
    {
        std::cout << "vk_f.ell[" << i << "] = ";
        std::cout << vk_f.ell[i].toString({"u"}) << std::endl;
    }
    std::cout << "vk_f.f = " << vk_f.f.toString({"u"}) << std::endl
              << std::endl;

    std::cout << "Evaluation Key (ek) size: " << ek.length() << std::endl;
    // std::cout << "First and last EK elements:" << std::endl;
    // if (ek.length() > 0)
    // {
    //     std::cout << "ek[0] = " << ek[0].toString({"x_0", "x_1"}) << std::endl;
    //     if (ek.length() > 1)
    //     {
    //         std::cout << "ek[" << (ek.length() - 1) << "] = "
    //                   << ek[ek.length() - 1].toString({"x_0", "x_1"}) << std::endl;
    //     }
    // }
    // std::cout << std::endl;

    // Step 4: Generate problem instance
    std::cout << "Step 4: Generating problem instance (ProbGen)..." << std::endl;
    Vec<Fq> X;
    X.SetLength(env.m);
    // Setting X values
    for (int i = 0; i < env.m; ++i)
    {
        X[i] = random_ZZ_p(); // Simple values: X = [1, 2, ...]
    }
    printVector(X, "Input Vector X");

    Mat<Fq> sigma;
    VK_X vk_x;
    ProbGen(vk_x, sigma, env, pk, X);

    std::cout << "ProbGen completed." << std::endl;
    std::cout << "vk_x.a = " << vk_x.a << std::endl;
    std::cout << "vk_x.alpha = " << vk_x.alpha << std::endl;
    printMatrix(sigma, "sigma Matrix");

    // Step 5: Mask Generation
    std::cout << "Step 5: Generating masks (MaskGen)..." << std::endl;
    VK_theta vk_theta;
    SK_theta sk_theta;
    Vec<Fq> theta;
    MaskGen(vk_theta, sk_theta, theta, env, vk_x);

    // Step 6: Compute proofs for each component
    std::cout << "Step 6: Computing proofs..." << std::endl;
    Vec<Fq> pi;
    pi.SetLength(env.k);

    std::cout << "Computing individual proofs pi[i]:" << std::endl;
    for (int i = 0; i < env.k; ++i)
    {
        Compute(pi[i], i, ek[i], sigma[i], theta[i], env);
        std::cout << "pi[" << i << "] = " << pi[i] << std::endl;
        std::cout << std::endl;
    }

    printVector(pi, "Proof Vector pi");

    // Output ZZ_pX polynomial interpolation result
    std::cout << "Interpolating polynomial from proofs:" << std::endl;
    Vec<Fq> k_vec;
    k_vec.SetLength(env.k);
    for (int i = 0; i < env.k; ++i)
    {
        k_vec[i] = to_ZZ_p(i);
    }
    ZZ_pX phi = interpolate(k_vec, pi);
    printZZ_pX(phi, "Interpolated Polynomial phi");

    // Step 7: Verify the proofs
    std::cout << "Step 7: Verifying proofs..." << std::endl;
    bool verified = Verify(vk_f, vk_theta, pi, env);

    std::cout << "Verification result: " << (verified ? "Success" : "Failed") << std::endl;

    // Step 8: Final result computation
    std::cout << "Step 8: Final result computation..." << std::endl;
    Fq result;
    Reconstruct(result, sk_theta, pi, env);
    std::cout << "Final result: " << result << std::endl;

    // Step 9: Verify the result independently
    std::cout << "Step 9: Independent verification..." << std::endl;

    // Calculate F(X) directly
    std::vector<Fq> X_vec(env.m);
    for (int i = 0; i < env.m; ++i)
    {
        X_vec[i] = X[i];
    }
    Fq direct_result = F.evaluate(X_vec);
    std::cout << "Direct F(X) calculation: " << direct_result << std::endl;

    std::cout << "Result check: " << (direct_result == result ? "Matching" : "Different") << std::endl;

    return;
}
}