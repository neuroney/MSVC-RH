#pragma once
#include "MSVC_CH_5.h"
#include <cassert>
#include "MultiPoly.h"
#include <iostream>
#include <iomanip>


// Print polynomials using MultiPoly's toString method
void printMultiPoly(const MultiPoly<Fq>& poly, const std::string& name);

// Print ZZ_pX polynomials, referencing NTL format
void printZZ_pX(const ZZ_pX& poly, const std::string& name);

// Helper function to print vectors
template<typename T>
void printVector(const Vec<T>& vec, const std::string& name);

// Helper function to print matrices
template<typename T>
void printMatrix(const Mat<T>& mat, const std::string& name);
// Helper function to print environment parameters
void printEnv(const Env& env);
void MSVC_CH_5_TEST(int d, int m, int t, int secpar);