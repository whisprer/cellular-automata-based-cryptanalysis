// math_functions.cpp
#include "test_suite.hpp"
#include "math_functions.hpp"
#include <cmath>

namespace nist_sts {

// These are based on the Cephes mathematical library functions
// Adapted for C++

// Constants
static const double rel_error = 1E-12;
static const double MACHEP = 1.11022302462515654042E-16;  // 2**-53
static const double MAXLOG = 7.09782712893383996732224E2; // log(MAXNUM)
static const double MAXNUM = 1.7976931348623158E308;      // 2**1024*(1-MACHEP)
static const double PI = 3.14159265358979323846;          // pi

static double big = 4.503599627370496e15;
static double biginv = 2.22044604925031308085e-16;

// Error function
double erfc(double x) {
    static const double one_sqrtpi = 0.564189583547756287;
    double a = 1, b = x, c = x, d = x*x + 0.5;
    double q1, q2 = b/d, n = 1.0, t;
    
    if (std::abs(x) < 2.2)
        return 1.0 - erf(x);
    if (x < 0)
        return 2.0 - erfc(-x);
    
    do {
        t = a*n + b*x;
        a = b;
        b = t;
        t = c*n + d*x;
        c = d;
        d = t;
        n += 0.5;
        q1 = q2;
        q2 = b/d;
    } while (std::abs(q1-q2)/q2 > rel_error);
    
    return one_sqrtpi*std::exp(-x*x)*q2;
}

// Error function (direct implementation)
double erf(double x) {
    static const double two_sqrtpi = 1.128379167095512574;
    double sum = x, term = x, xsqr = x * x;
    int j = 1;
    
    if (std::abs(x) > 2.2)
        return 1.0 - erfc(x);
    
    do {
        term *= xsqr/j;
        sum -= term/(2*j+1);
        j++;
        term *= xsqr/j;
        sum += term/(2*j+1);
        j++;
    } while (std::abs(term)/sum > rel_error);
    
    return two_sqrtpi*sum;
}

// Incomplete gamma function (complementary)
double igamc(double a, double x) {
    if (x <= 0 || a <= 0)
        return 1.0;
    
    if (x < 1.0 || x < a)
        return 1.0 - igam(a, x);
    
    double ax = a * std::log(x) - x - lgam(a);
    if (ax < -MAXLOG)
        return 0.0;
    
    ax = std::exp(ax);
    
    // Continued fraction
    double y = 1.0 - a;
    double z = x + y + 1.0;
    double c = 0.0;
    double pkm2 = 1.0;
    double qkm2 = x;
    double pkm1 = x + 1.0;
    double qkm1 = z * x;
    double ans = pkm1/qkm1;
    
    double t;
    do {
        c += 1.0;
        y += 1.0;
        z += 2.0;
        double yc = y * c;
        double pk = pkm1 * z - pkm2 * yc;
        double qk = qkm1 * z - qkm2 * yc;
        
        if (qk != 0) {
            double r = pk/qk;
            t = std::abs((ans - r)/r);
            ans = r;
        } else {
            t = 1.0;
        }
        
        pkm2 = pkm1;
        pkm1 = pk;
        qkm2 = qkm1;
        qkm1 = qk;
        
        if (std::abs(pk) > big) {
            pkm2 *= biginv;
            pkm1 *= biginv;
            qkm2 *= biginv;
            qkm1 *= biginv;
        }
    } while (t > MACHEP);
    
    return ans * ax;
}

// Incomplete gamma function
double igam(double a, double x) {
    if (x <= 0 || a <= 0)
        return 0.0;
    
    if (x > 1.0 && x > a)
        return 1.0 - igamc(a, x);
    
    double ax = a * std::log(x) - x - lgam(a);
    if (ax < -MAXLOG)
        return 0.0;
    
    ax = std::exp(ax);
    
    // Power series
    double r = a;
    double c = 1.0;
    double ans = 1.0;
    
    do {
        r += 1.0;
        c *= x/r;
        ans += c;
    } while (c/ans > MACHEP);
    
    return ans * ax/a;
}

// Logarithm of gamma function
double lgam(double x) {
    static double A[] = {
        0.6661, 0.2733, 0.9850, 0x3f4a,
        0xe943, 0xb580, 0x7fbd, 0xbf43,
        0x5ebb, 0x20dc, 0x019f, 0x3f4a,
        0xa5a1, 0x16b0, 0xc16c, 0xbf66,
        0x554b, 0x5555, 0x5555, 0x3fb5
    };
    
    // Implementation simplified for readability
    // This is a approximation for lgamma function
    
    if (x <= 0)
        return MAXNUM; // Error handling
    
    if (x < 0.5)
        return std::lgamma(x); // Use standard library
    
    if (x <= 2.5)
        return std::lgamma(x); // Use standard library
    
    if (x <= 13.0) {
        double z = 1.0;
        double p = 0.0;
        double u = x;
        
        while (u >= 3.0) {
            p -= 1.0;
            u = x + p;
            z *= u;
        }
        
        return std::log(z) + std::lgamma(u);
    }
    
    double q = (x - 0.5) * std::log(x) - x + 0.91893853320467274178;
    
    if (x > 1.0e8)
        return q;
    
    double p = 1.0/(x*x);
    
    if (x >= 1000.0)
        q += ((7.9365079365079365079365e-4 * p
              - 2.7777777777777777777778e-3) * p
             + 0.0833333333333333333333) / x;
    else
        q += polevl(p, A, 4) / x;
    
    return q;
}

// Polynomial evaluation
double polevl(double x, double* coef, int N) {
    double ans = coef[0];
    
    for (int i = 1; i <= N; i++)
        ans = ans * x + coef[i];
    
    return ans;
}

// Polynomial evaluation (with first coefficient 1.0)
double p1evl(double x, double* coef, int N) {
    double ans = x + coef[0];
    
    for (int i = 1; i < N; i++)
        ans = ans * x + coef[i];
    
    return ans;
}

} // namespace nist_sts