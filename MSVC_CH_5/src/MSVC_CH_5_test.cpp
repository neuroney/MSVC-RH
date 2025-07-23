#include "MSVC_CH_5_test.h"

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

// Helper function to print environment parameters
void printEnv(const Env& env) {
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

void MSVC_CH_5_TEST(int d, int m, int t, int secpar) {
    std::cout << "===================================================" << std::endl;
    std::cout << "MSVC_CH_5 Protocol Test with d=" << d << ", t=" << t << ", secpar=" << secpar << std::endl;
    std::cout << "===================================================" << std::endl;
    std::cout << std::endl;

    // Step 1: Initialize the environment
    std::cout << "Step 1: Initializing environment..." << std::endl;
    Env env;
    Initialize(env, t, secpar);

    // Step 2: Create the polynomial F
    std::cout << "Step 2: Creating polynomial F..." << std::endl;
    MultiPoly<Fq> F = generateFullPoly(m, d, to_ZZ_p(1));
    // 2 variables, max degree 2 (as per d=2)
    //MultiPoly<Fq> F(3, 3);  // 3 variables, max degree 3 (as per d=3)
    // Creating a simple polynomial: F = x^2 + y + 1
    // F.addTerm({2, 0}, to_ZZ_p(1));  // x^2
    // F.addTerm({0, 1}, to_ZZ_p(1));  // y
    // F.addTerm({0, 0}, to_ZZ_p(1));  // constant term
    vector<string> varNames;
    for (int i = 0; i < m; ++i) {
        varNames.push_back("x_" + std::to_string(i));
    }
    std::cout << "F = " << F.toString(varNames) << std::endl << std::endl;

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
    
    std::cout << "Evaluation Key (ek) size: " << ek.length() << std::endl;
    std::cout << "First and last EK elements:" << std::endl;
    if (ek.length() > 0) {
        std::cout << "ek[0] = " << ek[0].toString({"x_0", "x_1"}) << std::endl;
        if (ek.length() > 1) {
            std::cout << "ek[" << (ek.length()-1) << "] = " 
                      << ek[ek.length()-1].toString({"x_0", "x_1"}) << std::endl;
        }
    }
    std::cout << std::endl;

    // Step 4: Generate problem instance
    std::cout << "Step 4: Generating problem instance (ProbGen)..." << std::endl;
    Vec<Fq> X;
    X.SetLength(env.m);
    // Setting X values
    for (int i = 0; i < env.m; ++i) {
        X[i] = random_ZZ_p();  // Simple values: X = [1, 2, ...]
    }
    printVector(X, "Input Vector X");
    
    Mat<Fq> sigma;
    VK_X vk_x;
    ProbGen(vk_x, sigma, env, pk, X);
    
    std::cout << "ProbGen completed." << std::endl;
    printMatrix(sigma, "sigma Matrix");

    // Step 5: Compute proofs for each component
    std::cout << "Step 5: Computing proofs..." << std::endl;
    Mat<Fq> pi;
    pi.SetDims(env.k, 2);

    std::cout << "Computing individual proofs pi[i]:" << std::endl;
    for (int i = 0; i < env.k; ++i) {
        Vec<Fq> sigma_i;
        sigma_i.SetLength(env.m + 1);
        for (int j = 0; j < env.m + 1; ++j) {
            sigma_i[j] = sigma[i][j];
        }
        
        std::cout << "Computing proof for index " << i << std::endl;
        printVector(sigma_i, "sigma[" + std::to_string(i) + "]");
        
        Compute(pi[i], i, ek[i], sigma_i, env);
        std::cout << "pi[" << i << "] = " << pi[i] << std::endl;
        std::cout << std::endl;
    }
    
    // Output ZZ_pX polynomial interpolation result
    std::cout << "Interpolating polynomial from proofs:" << std::endl;
    Vec<Fq> k_vec, pi0_vec, pi1_vec;
    k_vec.SetLength(env.k);
    pi0_vec.SetLength(env.k);
    pi1_vec.SetLength(env.k);
    for (int i = 0; i < env.k; ++i) {
        k_vec[i] = to_ZZ_p(i);
        pi0_vec[i] = pi[i][0];
        pi1_vec[i] = pi[i][1];
    }
    cout << "length: " << pi[0].length() << std::endl;
    ZZ_pX phi = interpolate(k_vec, pi0_vec);
    printZZ_pX(phi, "Interpolated Polynomial phi");

    // Step 6: Verify the proofs
    std::cout << "Step 6: Verifying proofs..." << std::endl;
    Fq result;
    bool verified = Verify(result, vk_f, vk_x, pi, env);
    
    std::cout << "Verification result: " << (verified ? "Success" : "Failed") << std::endl;
    std::cout << "Final result: " << result << std::endl;
    
    // Step 7: Verify the result independently
    std::cout << "Step 7: Independent verification..." << std::endl;
    
    // Calculate F(X) directly
    std::vector<Fq> X_vec(env.m);
    for (int i = 0; i < env.m; ++i) {
        X_vec[i] = X[i];
    }
    Fq direct_result = F.evaluate(X_vec);
    std::cout << "Direct F(X) calculation: " << direct_result << std::endl;
    
    std::cout << "Result check: " << (direct_result == result ? "Matching" : "Different") << std::endl;
    
    return;
}