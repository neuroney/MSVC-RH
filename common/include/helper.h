#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "NTL/ZZ.h"
#include "NTL/ZZX.h"
#include <array>
#include "MultiPoly.h"

using namespace std;
using namespace NTL;
using Fq = ZZ_p;

void DataProcess(double &mean, double &stdev, double *Time, int cyctimes);
ZZ PRF_ZZ(const int &prfkey, const ZZ &mmod);

ZZ FindGen(const ZZ &p, const ZZ &q, long max_trials = 10000);

ZZ compute_g_fa(const ZZ_pX &f,
                  const Vec<ZZ> &gPows,
                  const ZZ &g,
                  const ZZ &mod);
