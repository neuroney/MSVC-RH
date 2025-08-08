#pragma once
#include "helper.h"
#include "MultiPoly.h"
namespace RH4 {

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

using PK_F = Vec<MultiPoly<Fq>>;
struct VK_F {
    Vec<MultiPoly<Fq>> ell;
    MultiPoly<Fq> f;
};
using EK_F = Vec<MultiPoly<Fq>>;
struct VK_X {
    Vec<ZZ> a;
    Vec<ZZ> alpha; 
    //Fq a_bk;
    //Fq alpha_bk;
};

struct VK_theta {
    VK_X vkx;
    ZZ gbeta;
    //Fq beta;
};
using SK_theta = Fq;

void Initialize(Env &env, int t, int secpar = 256);

void KeyGen(PK_F &pk, VK_F &vk, EK_F &ek, Env &env, const MultiPoly<Fq> &F);

void ProbGen(VK_X &vk, Mat<Fq> &sigma, const Env& env, const PK_F &pk, Vec<Fq> X);

void Compute(Fq & pi_i, int idx, const MultiPoly<Fq> &ek_i, const Vec<Fq> &sigma_i, const Fq& theta_i, const Env &env);

bool Verify(const VK_F &vk_f, const VK_theta & vk_theta, Vec<Fq> pi, const Env &env);

void MaskGen(VK_theta &vk, SK_theta &sk, Vec<Fq> &theta, const Env& env, const VK_X &vk_x);

void Reconstruct(Fq & res, const SK_theta &sk, const Vec<Fq> &pi, const Env &env);
}