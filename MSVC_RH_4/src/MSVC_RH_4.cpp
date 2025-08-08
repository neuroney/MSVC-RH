#include "MSVC_RH_4.h"
namespace RH4 {

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
    if (F.varCount() == 0 || F.maxDegree() < 0)
        throw std::invalid_argument("Invalid polynomial F");

    pk.kill();
    vk.ell.kill();
    ek.kill();

    env.m = F.varCount();
    env.d = F.maxDegree();
    env.k = env.d * (env.t + 1) + 1;

    MultiPoly<Fq> poly(1, 1);
    vector<MultiPoly<Fq>> ell;
    for (size_t i = 0; i < F.varCount(); ++i)
    {
        ell.push_back(poly);
        ell.back().addTerm({0}, random_ZZ_p()); // Random coefficient for constant
        ell.back().addTerm({1}, random_ZZ_p()); // Random coefficient for u^1
    }
    auto f = compose(F, ell);

    for (int i = 0; i < F.varCount(); ++i)
    {
        pk.append(ell[i]);
    }

    vk.ell = pk;
    vk.f = f;
    for (int i = 0; i < env.k; ++i)
    {
        ek.append(F);
    }
}

void ProbGen(VK_X &vk, Mat<Fq> &sigma, const Env &env, const PK_F &pk, Vec<Fq> X)
{
    if (pk.length() == 0)
        throw std::invalid_argument("Public key pk is empty");

    if (X.length() != env.m)
        throw std::invalid_argument("Length of X must match number of variables in pk");

    // Step 1: Sample a, alpha
    ZZ_p a, alpha;
    do
    {
        a = random_ZZ_p();
    } while (IsZero(a));

    ZZ k_bound = ZZ(env.k);
    do
    {
        alpha = random_ZZ_p();
    } while (rep(alpha) < k_bound);

    // Step 2: pk(a)
    Vec<Fq> aa;
    aa.SetLength(pk.length());
    for (int i = 0; i < pk.length(); ++i)
    {
        aa[i] = pk[i].evaluate({a});
    }

    Mat<Fq> r;
    r = random_mat_ZZ_p(env.t + 1, env.m);
    // Step 4: compute rMulalpha = sum r_s * alpha^s
    Vec<Fq> rMulalpha;
    rMulalpha.SetLength(env.m);

    Fq alpha_exp = alpha;
    for (int i = 0; i < env.t; ++i)
    {
        for (int j = 0; j < env.m; ++j)
        {
            rMulalpha[j] += r[i][j] * alpha_exp;
        }
        alpha_exp *= alpha;
    }

    ZZ_p inv_alpha = inv(alpha_exp);
    for (int i = 0; i < env.m; ++i)
    {
        r[env.t][i] = inv_alpha * (aa[i] - (X[i] + rMulalpha[i]));
    }

    Vec<ZZ_pX> c;
    c.SetLength(env.m);
    for (int i = 0; i < env.m; ++i)
    {
        SetCoeff(c[i], 0, X[i]); // Set constant term
        for (int j = 0; j < env.t + 1; ++j)
        {
            SetCoeff(c[i], j + 1, r[j][i]);
        }
    }

    ZZ_pX b;
    ZZ_pX q, factor;
    random(q, env.k - 1);        // Random polynomial of degree k-2
    SetCoeff(q, 0, 0);           // Set constant term of q to 0, so q(0) = 0
    SetCoeff(factor, 0, -alpha); // factor = (x - alpha)
    SetCoeff(factor, 1, 1);
    b = factor * q; // b(x) = (x - alpha) * q(x), so b(alpha) = 0 and b(0) = 0

    sigma.SetDims(env.k, env.m + 1);
    for (int i = 0; i < env.k; ++i)
    {
        for (int j = 0; j < env.m; ++j)
        {
            eval(sigma[i][j], c[j], to_ZZ_p(i));
        }
        eval(sigma[i][env.m], b, to_ZZ_p(i));
    }

    vk.a.SetLength(env.d);
    vk.alpha.SetLength(env.k - 1);

    ZZ_p a_power(1);
    for (int i = 0; i < env.d; ++i)
    {
        a_power *= a;
        PowerMod(vk.a[i], env.g, rep(a_power), env.fq);
    }

    ZZ_p alpha_power(1);
    for (int i = 0; i < env.k - 1; ++i)
    {
        alpha_power *= alpha;
        PowerMod(vk.alpha[i], env.g, rep(alpha_power), env.fq);
    }

    //vk.a_bk = a;
    //vk.alpha_bk = alpha;
}

void Compute(Fq &pi_i, int idx, const MultiPoly<Fq> &ek_i, const Vec<Fq> &sigma_i, const Fq &theta_i, const Env &env)
{
    if (idx < 0 || idx >= env.k)
        throw std::out_of_range("Index out of range in Compute");

    if (ek_i.varCount() != sigma_i.length() - 1)
        throw std::invalid_argument("Length of sigma_i must match number of variables in ek_i");

    vector<Fq> eval_points(ek_i.varCount());
    for (int i = 0; i < env.m; ++i)
    {
        eval_points[i] = sigma_i[i];
    }

    pi_i = ek_i.evaluate(eval_points) + sigma_i[env.m] + theta_i;
}

bool Verify(const VK_F &vk_f, const VK_theta &vk_theta, Vec<Fq> pi, const Env &env)
{
    if (pi.length() != env.k)
        throw std::invalid_argument("Length of pi must match k in Env");

    // Generate a vec contains [1,...k]
    Vec<Fq> k_vec;
    k_vec.SetLength(env.k);
    for (int i = 0; i < env.k; ++i)
    {
        k_vec[i] = to_ZZ_p(i);
    }

    ZZ_pX phi = interpolate(k_vec, pi);
    ZZ res1 = compute_g_fa(phi, vk_theta.vkx.alpha, env.g, env.fq);

    ZZ_pX f = toZZ_pX(vk_f.f);
    ZZ res2 = compute_g_fa(f, vk_theta.vkx.a, env.g, env.fq);
    MulMod(res2, res2, vk_theta.gbeta, env.fq);

    if (res1 != res2)
    {
        std::cerr << "Verification failed: vk_f.f evaluated at vk_x.a does not match vk_x.alpha" << std::endl;
        return false;
    }
    //cout << "Verification successful." << endl;
    return true;
}

void MaskGen(VK_theta &vk, SK_theta &sk, Vec<Fq> &theta, const Env &env, const VK_X &vk_x)
{
    random(sk);
    vk.vkx = vk_x;
    PowerMod(vk.gbeta, env.g, rep(sk), env.fq);

    theta.SetLength(env.k);
    ZZ_pX z;
    random(z, env.k);        // Random polynomial of degree k-1
    SetCoeff(z, 0, sk);           // Set constant term of z to beta,
    for (int i = 0; i < env.k; ++i)
    {
        eval(theta[i], z, to_ZZ_p(i));
    }
    PowerMod(vk.gbeta, env.g, rep(sk), env.fq);
    
    ZZ tmp;
    for (int i = 0; i < env.k - 1; ++i)
    {
        PowerMod(tmp, vk_x.alpha[i], rep(z[i+1]), env.fq);
        MulMod(vk.gbeta, vk.gbeta, tmp, env.fq);
    }
    //vk.beta = sk;
}

void Reconstruct(Fq &res, const SK_theta &sk, const Vec<Fq> &pi, const Env &env)
{
    Vec<Fq> k_vec;
    k_vec.SetLength(env.k);
    for (int i = 0; i < env.k; ++i)
    {
        k_vec[i] = to_ZZ_p(i);
    }

    ZZ_pX phi = interpolate(k_vec, pi);
    eval(res, phi, ZZ_p(0));
    res -= sk;
}
}