// math_functions.cpp
#include "math_functions.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace nist_sts {

// Explicit implementation of mathematical utility functions

double erfc(double x) {
    // Use standard library implementation
    return std::erfc(x);
}

double igamc(double a, double x) {
    // Complementary incomplete gamma function
    if (x < 0.0 || a <= 0.0) return 1.0;
    
    // For small x or a
    if (x < 1.0 || x < a) {
        // Use regularized gamma Q function
        return 1.0 - std::tgamma(a, x) / std::tgamma(a);
    }
    
    // For large arguments
    double logGammaA = std::lgamma(a);
    double ax = a * std::log(x) - x - logGammaA;
    
    if (ax < -std::numeric_limits<double>::max()) {
        return 0.0;
    }
    
    return std::exp(ax);
}

double lgam(double x) {
    // Logarithm of gamma function
    return std::lgamma(x);
}

} // namespace nist_sts