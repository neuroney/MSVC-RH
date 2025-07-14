// MultiPoly.h
#pragma once

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <numeric>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <functional>
#include "NTL/ZZ_pX.h" 
// Sparse multivariate polynomial template class
// Coeff: coefficient type (e.g., int, double, NTL::ZZ_p, etc.)
// Constructs with:
//   m = number of variables
//   d = maximum total degree allowed for this polynomial

template <typename Coeff>
class MultiPoly
{
public:
    using Exponents = std::vector<int>;

    // Constructor: specify number of variables m and max total degree d
    MultiPoly() : varCount_(0), maxDegree_(0) {}
    MultiPoly(size_t m, int d)
        : varCount_(m), maxDegree_(d)
    {
        if (m == 0 || d < 0)
            throw std::invalid_argument("Number of variables m must be >0 and max degree d>=0");
    }

    // Accessors
    size_t varCount() const { return varCount_; }
    int maxDegree() const { return maxDegree_; }
    const std::map<Exponents, Coeff> &terms() const { return terms_; }

    // Add term c * x^e; combine like terms and remove zero coefficients
    void addTerm(const Exponents &e, const Coeff &c)
    {
        assert(e.size() == varCount_);
        if (c == Coeff(0))
            return;
        int deg = std::accumulate(e.begin(), e.end(), 0);
        if (deg > maxDegree_)
            throw std::out_of_range("Monomial total degree exceeds maxDegree");
        auto &coef = terms_[e];
        coef += c;
        if (coef == Coeff(0))
            terms_.erase(e);
    }

    // Polynomial addition: unify dimensions and degree bounds
    MultiPoly operator+(const MultiPoly &B) const
    {
        size_t newVar = std::max(varCount_, B.varCount_);
        int newDeg = std::max(maxDegree_, B.maxDegree_);
        MultiPoly R(newVar, newDeg);
        for (auto &[e, c] : terms_)
        {
            Exponents ext = e;
            ext.resize(newVar, 0);
            R.addTerm(ext, c);
        }
        for (auto &[e, c] : B.terms_)
        {
            Exponents ext = e;
            ext.resize(newVar, 0);
            R.addTerm(ext, c);
        }
        return R;
    }

    // Polynomial multiplication
    MultiPoly operator*(const MultiPoly &B) const
    {
        size_t newVar = std::max(varCount_, B.varCount_);
        int newDeg = maxDegree_ + B.maxDegree_;
        MultiPoly R(newVar, newDeg);
        for (auto &[e1, c1] : terms_)
        {
            for (auto &[e2, c2] : B.terms_)
            {
                Exponents ext(newVar);
                for (size_t i = 0; i < newVar; i++)
                {
                    int a = (i < e1.size() ? e1[i] : 0);
                    int b = (i < e2.size() ? e2[i] : 0);
                    ext[i] = a + b;
                }
                R.addTerm(ext, c1 * c2);
            }
        }
        return R;
    }

    // Evaluate at a point pts (length == varCount_)
    Coeff evaluate(const std::vector<Coeff> &pts) const
    {
        assert(pts.size() == varCount_);
        Coeff sum = Coeff(0);
        std::vector<std::vector<Coeff>> powTable(varCount_);
        for (size_t i = 0; i < varCount_; i++)
        {
            powTable[i].resize(maxDegree_ + 1);
            powTable[i][0] = Coeff(1);
            for (int k = 1; k <= maxDegree_; k++)
                powTable[i][k] = powTable[i][k - 1] * pts[i];
        }
        for (auto &[e, c] : terms_)
        {
            Coeff term = c;
            for (size_t i = 0; i < varCount_; i++)
                term *= powTable[i][e[i]];
            sum += term;
        }
        return sum;
    }

    // Convert to string, varNames must match varCount_
    std::string toString(const std::vector<std::string> &varNames) const
    {
        assert(varNames.size() == varCount_);
        if (terms_.empty())
            return "0";
        std::ostringstream oss;
        bool first = true;
        for (auto &[e, c] : terms_)
        {
            if (!first)
                oss << " + ";
            first = false;
            oss << c;
            for (size_t i = 0; i < varCount_; i++)
            {
                if (e[i] > 0)
                {
                    oss << "*" << varNames[i];
                    if (e[i] > 1)
                        oss << "^" << e[i];
                }
            }
        }
        return oss.str();
    }

    size_t termCount() const { return terms_.size(); }

private:
    size_t varCount_;
    int maxDegree_;
    std::map<Exponents, Coeff> terms_;
};

template <typename Coeff>
MultiPoly<Coeff> generateFullPoly(size_t m, int d, const Coeff &c = Coeff(1))
{
    using Exponents = typename MultiPoly<Coeff>::Exponents;
    MultiPoly<Coeff> P(m, d);
    Exponents e(m);
    std::function<void(int, int)> rec = [&](int idx, int rem)
    {
        if (idx == (int)m)
        {
            P.addTerm(e, c);
            return;
        }
        for (int k = 0; k <= rem; ++k)
        {
            e[idx] = k;
            rec(idx + 1, rem - k);
        }
    };
    rec(0, d);
    return P;
}

// Exponentiation: g^exp with degree bound 'bound'
template <typename Coeff>
MultiPoly<Coeff> polyPow(const MultiPoly<Coeff> &g, int exp, int bound)
{
    size_t n = g.varCount();
    MultiPoly<Coeff> result(n, bound);
    typename MultiPoly<Coeff>::Exponents zeroExp(n, 0);
    result.addTerm(zeroExp, Coeff(1));
    if (exp == 0)
        return result;
    MultiPoly<Coeff> base = g;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

// Composition: f(g1, g2, ..., gm)
template <typename Coeff>
MultiPoly<Coeff> compose(
    const MultiPoly<Coeff> &f,
    const std::vector<MultiPoly<Coeff>> &gs)
{
    size_t m = f.varCount();
    assert(gs.size() == m);
    size_t n = gs[0].varCount();
    int maxDegG = 0;
    for (const auto &gi : gs)
    {
        assert(gi.varCount() == n);
        maxDegG = std::max(maxDegG, gi.maxDegree());
    }
    int bound = f.maxDegree() * maxDegG;
    MultiPoly<Coeff> R(n, bound);
    for (const auto &[e, c] : f.terms())
    {
        MultiPoly<Coeff> term(n, bound);
        typename MultiPoly<Coeff>::Exponents zero(n, 0);
        term.addTerm(zero, c);
        for (size_t i = 0; i < m; i++)
            if (e[i] > 0)
                term = term * polyPow(gs[i], e[i], bound);
        R = R + term;
    }
    return R;
}


inline NTL::ZZ_pX toZZ_pX(const MultiPoly<NTL::ZZ_p>& poly) {
    if (poly.varCount() != 1) {
        throw std::invalid_argument("toZZ_pX conversion only works with univariate polynomials");
    }
    
    NTL::ZZ_pX result;
    for (const auto& [exponents, coeff] : poly.terms()) {
        int degree = exponents[0];
        NTL::SetCoeff(result, degree, coeff);
    }
    
    return result;
}