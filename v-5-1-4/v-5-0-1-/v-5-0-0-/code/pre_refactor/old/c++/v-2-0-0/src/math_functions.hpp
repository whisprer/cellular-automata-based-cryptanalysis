// math_functions.hpp
#pragma once

namespace nist_sts {

// Error function
double erfc(double x);

// Incomplete Gamma function
double igamc(double a, double x);
double igam(double a, double x);

// Gamma function
double lgam(double x);

// Utility functions
double polevl(double x, double* coef, int N);
double p1evl(double x, double* coef, int N);

} // namespace nist_sts