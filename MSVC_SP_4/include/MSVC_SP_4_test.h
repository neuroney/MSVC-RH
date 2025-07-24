#pragma once
#include "MSVC_SP_4.h"
#include <cassert>
#include "MultiPoly.h"
#include <iostream>
#include <iomanip>
namespace SP4
{
    // Helper function to print environment parameters
    void printEnv(const Env &env);
    void MSVC_SP_4_TEST(int d, int m, int t, int secpar);
}