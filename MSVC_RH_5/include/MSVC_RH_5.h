#pragma once
#include "helper.h"
#include "MultiPoly.h"

struct Env {
    ZZ fq;
    ZZ ord;
    ZZ g;
    int secpar;
    int t;
    int k;
    int d;
    int m;
};

using PK_F = ZZ;
using VK_F = ZZ; 
using EK_F = Vec<MultiPoly<Fq>>;
using VK_X = ZZ;
using VK_theta = ZZ;
using SK_theta = Fq;

void Initialize(Env &env, int t, int secpar = 256);

void KeyGen(PK_F &pk, VK_F &vk, EK_F &ek, Env &env, const MultiPoly<Fq> &F);

void ProbGen(VK_X &vk, Mat<Fq> &sigma, const Env& env, const PK_F &pk, const Vec<Fq> &X);

void MaskGen(VK_theta &vk, SK_theta &sk, Vec<Fq> &theta, const Env& env, const VK_X &vk_x);

void Compute(Vec<Fq> & pi_i, int idx, const MultiPoly<Fq> &ek_i, const Vec<Fq> &sigma_i, const Fq &theta_i, const Env &env);

bool Verify(const VK_F &vk_f, const VK_X & vk_x, const Mat<Fq> &pi, const Env &env);

void Reconstruct(Fq & res, const SK_theta &sk, const Mat<Fq> &pi, const Env &env);