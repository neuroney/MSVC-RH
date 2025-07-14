// MultiPolyAllTests.cpp
// Comprehensive tests for MultiPoly class and associated free functions

#include "MultiPoly.h"
#include <iostream>
#include <vector>
#include <cassert>

// Utility to print test header
void printHeader(const char* title) {
    std::cout << "\n=== " << title << " ===\n";
}

void testGenerateFullPoly() {
    printHeader("Test generateFullPoly");
    auto P = generateFullPoly<int>(3, 2); // m=3, d=2
    // Expect term count = C(3+2,2) = 10
    int expected = 10;
    std::cout << "Term count = " << P.termCount() << ", expected " << expected << '\n';
    assert(P.termCount() == expected);
    std::cout << "Polynomial: " << P.toString({"x","y","z"}) << '\n';
}

void testAddition() {
    printHeader("Test addition");
    MultiPoly<double> A(2,3), B(3,2);
    A.addTerm({1,1}, 2.5);    // 2.5*x*y
    A.addTerm({2,0}, 1.0);    // 1.0*x^2
    B.addTerm({0,1,1}, 4.0);  // 4*y*z
    B.addTerm({0,0,0}, -1.5);// -1.5
    auto C = A + B;
    std::cout << "A = " << A.toString({"x","y"}) << '\n';
    std::cout << "B = " << B.toString({"x","y","z"}) << '\n';
    std::cout << "A+B = " << C.toString({"x","y","z"}) << '\n';
    // Expect C to contain all four terms:
    assert(C.termCount() == 4);
}

void testMultiplication() {
    printHeader("Test multiplication");
    MultiPoly<int> A(2,2), B(2,2);
    A.addTerm({1,0}, 3);   // 3x
    A.addTerm({0,1}, 2);   // 2y
    B.addTerm({1,0}, 1);   // x
    B.addTerm({0,1}, 5);   // 5y
    auto P = A * B;
    std::cout << "A = " << A.toString({"x","y"}) << '\n';
    std::cout << "B = " << B.toString({"x","y"}) << '\n';
    std::cout << "A*B = " << P.toString({"x","y"}) << '\n';
    // Expected: 3x*x + 3x*5y + 2y*x + 2y*5y = 3x^2 + 15xy + 2xy + 10y^2
    assert(P.termCount() == 3); // terms: x^2, xy, y^2
    assert(P.toString({"x","y"}).find("3*x^2") != std::string::npos);
    assert(P.toString({"x","y"}).find("17*x*y") != std::string::npos);
    assert(P.toString({"x","y"}).find("10*y^2") != std::string::npos);
}

void testEvaluate() {
    printHeader("Test evaluate");
    MultiPoly<double> P(2,3);
    // P(x,y) = 2x^2y + 3y
    P.addTerm({2,1}, 2.0);
    P.addTerm({0,1}, 3.0);
    double val = P.evaluate({2.0, 4.0});
    // 2*(2^2)*4 + 3*4 = 2*4*4 + 12 = 32 +12 =44
    std::cout << "P(2,4) = " << val << ", expected 44" << '\n';
    assert(std::abs(val - 44.0) < 1e-9);
}

void testPolyPow() {
    printHeader("Test polyPow");
    MultiPoly<int> G(1, 3); // univariate
    G.addTerm({1}, 2);      // G(x) = 2x
    auto H = polyPow(G, 3, 3*1); // (2x)^3
    std::cout << "G = " << G.toString({"x"}) << '\n';
    std::cout << "G^3 = " << H.toString({"x"}) << '\n';
    // Expect 8*x^3
    assert(H.termCount() == 1);
    assert(H.toString({"x"}) == "8*x^3");
}

void testCompose() {
    printHeader("Test compose");
    // f(u,v) = u^2 + v
    MultiPoly<int> f(2,2);
    f.addTerm({2,0}, 1);
    f.addTerm({0,1}, 1);
    // g1(x,y) = x+y, g2(x,y) = 2x
    MultiPoly<int> g1(2,1), g2(2,1);
    g1.addTerm({1,0}, 1); g1.addTerm({0,1}, 1);
    g2.addTerm({1,0}, 2);
    auto h = compose(f, {g1, g2});
    std::cout << "f(u,v) = " << f.toString({"u","v"}) << '\n';
    std::cout << "g1(x,y) = " << g1.toString({"x","y"}) << '\n';
    std::cout << "g2(x,y) = " << g2.toString({"x","y"}) << '\n';
    std::cout << "h(x,y)=f(g1,g2) = " << h.toString({"x","y"}) << '\n';
    // Evaluate at (x,y)=(3,4): g1=7, g2=6 => u^2+v = 7^2+6=55
    int eval = h.evaluate({3,4});
    std::cout << "h(3,4) = " << eval << ", expected 55" << '\n';
    assert(eval == 55);
}

int main() {
    testGenerateFullPoly();
    testAddition();
    testMultiplication();
    testEvaluate();
    testPolyPow();
    testCompose();
    std::cout << "\nAll tests passed!\n";
    return 0;
}