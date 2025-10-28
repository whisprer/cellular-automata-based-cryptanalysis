// serial_test.cpp
#include "serial_test.hpp"
#include "../common.hpp"
#include "../math_functions.hpp"
#include <cmath>
#include <unordered_map>
#include <string>

namespace nist_sts {

SerialTest::SerialTest(size_t blockLength)
    : blockLength(blockLength) {}

TestResult SerialTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Ensure we have enough data
    if (data.size() < blockLength) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Calculate psi-squared statistics
    double psim0 = psi2(data, blockLength);
    double psim1 = psi2(data, blockLength - 1);
    double psim2 = psi2(data, blockLength - 2);
    
    // Calculate deltas
    double del1 = psim0 - psim1;
    double del2 = psim0 - 2.0 * psim1 + psim2;
    
    // Calculate p-values
    double p_value1 = igamc(std::pow(2, blockLength - 1) / 2.0, del1 / 2.0);
    double p_value2 = igamc(std::pow(2, blockLength - 2) / 2.0, del2 / 2.0);
    
    // Store results
    result.statistics["psi_m"] = psim0;
    result.statistics["psi_m-1"] = psim1;
    result.statistics["psi_m-2"] = psim2;
    result.statistics["del_1"] = del1;
    result.statistics["del_2"] = del2;
    result.statistics["p_value1"] = p_value1;
    result.statistics["p_value2"] = p_value2;
    
    // Use the minimum p-value as the test result
    result.p_value = std::min(p_value1, p_value2);
    result.success = isSuccess(result.p_value);
    
    return result;
}

double SerialTest::psi2(const BitSequence& data, size_t m) {
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
    
    // Calculate psi-squared statistic
    double sum = 0.0;
    for (const auto& pair : frequencies) {
        sum += std::pow(pair.second, 2);
    }
    
    double psi = (sum * std::pow(2, m) / static_cast<double>(data.size())) - static_cast<double>(data.size());
    return psi;
}

} // namespace nist_sts