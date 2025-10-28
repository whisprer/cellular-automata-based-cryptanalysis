// dft_test.cpp
#include "dft_test.hpp"
#include "common.hpp"
#include <cmath>
#include <complex>
#include <vector>
#include <algorithm>

// Define M_PI if not available (common issue in MSVC)
// Define M_PI if not available (common issue in MSVC)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nist_sts {

TestResult DiscreteFourierTransformTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Convert bits to +1/-1
    std::vector<double> X(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        X[i] = 2.0 * static_cast<double>(data[i]) - 1.0;
    }
    
    // Apply FFT (using C++ standard library complex numbers)
    std::vector<std::complex<double>> dft(data.size());
    
    // Compute DFT (simple implementation for clarity)
    // For production code, use a high-performance FFT library
    for (size_t k = 0; k < data.size(); k++) {
        dft[k] = std::complex<double>(0.0, 0.0);
        for (size_t i = 0; i < data.size(); i++) {
            double angle = -2.0 * M_PI * k * i / data.size();
            dft[k] += X[i] * std::complex<double>(std::cos(angle), std::sin(angle));
        }
    }
    
    // Compute magnitudes of the first half of the DFT
    std::vector<double> magnitudes(data.size() / 2);
    magnitudes[0] = std::abs(dft[0].real());
    for (size_t i = 0; i < data.size() / 2; i++) {
        magnitudes[i] = std::sqrt(
            std::pow(dft[i].real(), 2) + std::pow(dft[i].imag(), 2)
        );
    }
    
    // Calculate threshold based on test criteria
    double threshold = std::sqrt(2.995732274 * data.size());
    
    // Count values below threshold
    size_t count = 0;
    for (size_t i = 0; i < data.size() / 2; i++) {
        if (magnitudes[i] < threshold) {
            count++;
        }
    }
    
    // Calculate statistics
    double percentile = 100.0 * static_cast<double>(count) / (data.size() / 2);
    double N_l = static_cast<double>(count);
    double N_o = 0.95 * data.size() / 2.0;
    double d = (N_l - N_o) / std::sqrt(data.size() / 4.0 * 0.95 * 0.05);
    
    // Calculate p-value
    double p_value = erfc(std::abs(d) / std::sqrt(2.0));
    
    // Store results
    result.statistics["percentile"] = percentile;
    result.statistics["N_l"] = N_l;
    result.statistics["N_o"] = N_o;
    result.statistics["d"] = d;
    result.statistics["threshold"] = threshold;
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

} // namespace nist_sts