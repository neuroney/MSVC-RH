#pragma once
#include "MSVC_RH_5.h"
#include <cassert>
#include "MultiPoly.h"
#include <iostream>
#include <iomanip>

namespace RH5 {

// Helper function to print environment parameters
void printEnv(const Env& env);
void MSVC_RH_5_TEST(int d, int m, int t, int secpar);
}