// approximate_entropy_test.cpp
#include "approximate_entropy_test.hpp"
#include "common.hpp"
#include "math_functions.hpp"
#include <cmath>
#include <unordered_map>
#include <string>

namespace nist_sts {

ApproximateEntropyTest::ApproximateEntropyTest(size_t blockLength)
    : blockLength(blockLength) {}

TestResult ApproximateEntropyTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Ensure we have enough data
    if (data.size() < blockLength) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Calculate ApEn (m) and ApEn (m+1)
    double ApEn[2];
    
    // Calculate phi(m) and phi(m+1)
    ApEn[0] = calculatePhi(data, blockLength);
    ApEn[1] = calculatePhi(data, blockLength + 1);
    
    // Calculate ApEn
    double apen = ApEn[0] - ApEn[1];
    
    // Calculate chi-squared statistic
    double chi_squared = 2.0 * data.size() * (std::log(2) - apen);
    
    // Calculate p-value
    double p_value = igamc(std::pow(2, blockLength - 1), chi_squared / 2.0);
    
    // Check if block length is appropriate for sequence length
    bool warning = false;
    if (blockLength > static_cast<size_t>(std::log(data.size()) / std::log(2) - 5)) {
        warning = true;
        result.statistics["warning"] = 1.0;
    }
    
    // Store results
    result.statistics["phi_m"] = ApEn[0];
    result.statistics["phi_m+1"] = ApEn[1];
    result.statistics["apen"] = apen;
    result.statistics["chi_squared"] = chi_squared;
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

double ApproximateEntropyTest::calculatePhi(const BitSequence& data, size_t m) {
    if (m == 0 || m == static_cast<size_t>(-1)) {
        return 0.0;
    }
    
    // Count frequencies of m-bit patterns
    std::unordered_map<std::string, int> frequencies;
    
    for (size_t i = 0; i < data.size(); i++) {
        std::string pattern;
        for (size_t j = 0; j < m; j++) {
            pattern += data[(i + j) % data.size()] ? '1' : '0';
        }
        frequencies[pattern]++;
    }
    
    // Calculate phi(m)
    double sum = 0.0;
    for (const auto& pair : frequencies) {
        double probability = static_cast<double>(pair.second) / data.size();
        sum += probability * std::log(probability);
    }
    
    return sum;
}

} // namespace nist_sts