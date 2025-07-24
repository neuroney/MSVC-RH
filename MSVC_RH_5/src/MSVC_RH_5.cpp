#include "MSVC_RH_5.h"

namespace RH5 {
void Initialize(Env &env, int t, int secpar)
{
    env.secpar = secpar;
    env.t = t;
        //GenGermainPrime(env.ord, env.secpar);
    conv<ZZ>(env.ord, "241231170316424564953358597862841670333");
    env.fq = 2 * env.ord + 1;
    ZZ_p::init(env.ord);
    env.g = FindGen(env.ord, env.fq, 10000);
}

void KeyGen(PK_F &pk, VK_F &vk, EK_F &ek, Env &env, const MultiPoly<Fq> &F)
{
    double tt = GetTime();
    if (F.varCount() == 0 || F.maxDegree() < 0)
        throw std::invalid_argument("Invalid polynomial F");

    env.m = F.varCount();
    env.d = F.maxDegree();
    env.k = (env.d + 1) * env.t + 1;

    pk = 0;
    vk = 0;
    ek.SetLength(env.k);
    for (int i = 0; i < env.k; ++i)
    {
        ek[i] = F;
    }
}

void ProbGen(VK_X &vk, Mat<Fq> &sigma, const Env &env, const PK_F &pk, const Vec<Fq> &X)
{
    if (X.length() != env.m)
        throw std::invalid_argument("Length of X must match number of variables in pk");

    // Step 1: Sample alpha

    ZZ_p alpha = random_ZZ_p();
    while (alpha == 0)
    {
        alpha = random_ZZ_p(); // Ensure alpha is non-zero
    }

    Vec<ZZ_pX> c;
    c.SetLength(env.m);
    for (int i = 0; i < env.m; ++i)
    {
        random(c[i], env.t + 1);
        SetCoeff(c[i], 0, X[i]); // Set constant term
    }

    ZZ_pX b;
    random(b, env.t + 1);
    SetCoeff(b, 0, alpha);
    sigma.SetDims(env.k, env.m + 3);

    // Context hiding, generating 0's shares
    ZZ_pX a_poly, e_poly;
    random(a_poly, env.k);
    random(e_poly, env.k);
    SetCoeff(a_poly, 0, 0); // Set constant term to 0
    SetCoeff(e_poly, 0, 0); // Set constant term to 0

    // eval
    ZZ_p tmp;
    for (int i = 0; i < env.k; ++i)
    {
        tmp = to_ZZ_p(i);
        for (int j = 0; j < env.m; ++j)
        {
            eval(sigma[i][j], c[j], tmp);
        }
        eval(sigma[i][env.m], b, tmp);
        eval(sigma[i][env.m + 1], a_poly, tmp);
        eval(sigma[i][env.m + 2], e_poly, tmp);
    }

    PowerMod(vk, env.g, rep(alpha), env.fq);
}

void Compute(Vec<Fq> &pi_i, int idx, const MultiPoly<Fq> &ek_i, const Vec<Fq> &sigma_i, const Fq &theta_i, const Env &env)
{
    if (idx < 0 || idx >= env.k)
        throw std::out_of_range("Index out of range in Compute");

    if (ek_i.varCount() != sigma_i.length() - 3)
        throw std::invalid_argument("Length of sigma_i must match number of variables in ek_i");

    // pi_i.kill();
    // pi_i.SetLength(2);

    vector<Fq> eval_points(ek_i.varCount());
    for (int i = 0; i < env.m; ++i)
    {
        eval_points[i] = sigma_i[i];
    }

    pi_i[0] = ek_i.evaluate(eval_points) + sigma_i[env.m + 1] + theta_i;
    pi_i[1] = sigma_i[env.m] * pi_i[0] + sigma_i[env.m + 2];
}

bool Verify(const VK_F &vk_f, const VK_X &vk_x, const Mat<Fq> &pi, const Env &env)
{
    if (pi.NumRows() != env.k || pi.NumCols() != 2)
        throw std::invalid_argument("Length of pi must match k in Env");

    // Generate a vec contains [1,...k]
    Vec<Fq> k_vec, pi0_vec, pi1_vec;
    k_vec.SetLength(env.k);
    pi0_vec.SetLength(env.k);
    pi1_vec.SetLength(env.k);
    for (int i = 0; i < env.k; ++i)
    {
        k_vec[i] = to_ZZ_p(i);
        pi0_vec[i] = pi[i][0];
        pi1_vec[i] = pi[i][1];
    }

    ZZ_pX phi = interpolate(k_vec, pi0_vec);
    ZZ_pX psi = interpolate(k_vec, pi1_vec);

    if (PowerMod(env.g, rep(psi[0]), env.fq) != PowerMod(vk_x, rep(phi[0]), env.fq))
    {
        std::cerr << "Verification failed: vk_x.a does not match vk_x.alpha" << std::endl;
        return false;
    }
    //cout << "Verification successful" << endl;
    return true;
}

void MaskGen(VK_theta &vk, SK_theta &sk, Vec<Fq> &theta, const Env &env, const VK_X &vk_x)
{
    vk = vk_x;
    ZZ_pX h, eta;
    random(sk);
    random(h, env.k);
    SetCoeff(h, 0, sk); // Set constant term to beta

    theta.SetLength(env.k);
    ZZ_p tmp;
    for (int i = 0; i < env.k; ++i)
    {
        tmp = to_ZZ_p(i);
        eval(theta[i], h, tmp);
    }
}

void Reconstruct(Fq & res, const SK_theta &sk, const Mat<Fq> &pi, const Env &env)
{
    Vec<Fq> k_vec, pi0_vec;
    k_vec.SetLength(env.k);
    pi0_vec.SetLength(env.k);
    for (int i = 0; i < env.k; ++i)
    {
        k_vec[i] = to_ZZ_p(i);
        pi0_vec[i] = pi[i][0];
    }

    ZZ_pX phi = interpolate(k_vec, pi0_vec);
    res = phi[0] - sk;
}
}