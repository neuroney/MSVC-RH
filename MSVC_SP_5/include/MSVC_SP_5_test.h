#pragma once
#include "MSVC_SP_5.h"
#include <cassert>
#include "MultiPoly.h"
#include <iostream>
#include <iomanip>

namespace SP5
{
    // Helper function to print environment parameters
    void printEnv(const Env &env);
    void MSVC_SP_5_TEST(int d, int m, int t, int secpar);
}