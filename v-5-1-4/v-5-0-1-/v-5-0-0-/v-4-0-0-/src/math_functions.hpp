// math_functions.hpp
#pragma once
#include <cmath>
#include <limits>

namespace nist_sts {

// Error complement function
inline double erfc(double x) {
    return std::erfc(x);
}

// Incomplete gamma function (complementary)
inline double igamc(double a, double x) {
    // Simple implementation of complementary incomplete gamma function
    if (x < 0.0 || a <= 0.0) return 1.0;
    
    // For small x or a
    if (x < 1.0 || x < a) {
        // Approximate using series expansion
        double sum = 1.0;
        double term = 1.0;
        double k = 0.0;
        
        while (std::abs(term) > std::numeric_limits<double>::epsilon()) {
            k += 1.0;
            term *= x / (a + k);
            sum += term;
        }
        
        return 1.0 - std::pow(x, a) * sum * std::exp(-x) / std::tgamma(a);
    }
    
    // For large arguments
    return 0.0;
}

// Logarithm of gamma function
inline double lgam(double x) {
    return std::lgamma(x);
}

} // namespace nist_sts