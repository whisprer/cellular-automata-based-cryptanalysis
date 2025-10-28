// universal_test.cpp
#include "universal_test.hpp"
#include "common.hpp"
#include "math_functions.hpp"
#include <cmath>
#include <vector>
#include <unordered_map>

namespace nist_sts {

TestResult UniversalTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Set parameters based on sequence length
    int L = 5;  // Default block length
    
    // Adjust L based on sequence length
    if (data.size() >= 387840) L = 6;
    if (data.size() >= 904960) L = 7;
    if (data.size() >= 2068480) L = 8;
    if (data.size() >= 4654080) L = 9;
    if (data.size() >= 10342400) L = 10;
    if (data.size() >= 22753280) L = 11;
    if (data.size() >= 49643520) L = 12;
    if (data.size() >= 107560960) L = 13;
    if (data.size() >= 231669760) L = 14;
    if (data.size() >= 496435200) L = 15;
    if (data.size() >= 1059061760) L = 16;
    
    int Q = 10 * (1 << L);
    int K = data.size() / L - Q;  // Number of blocks to test
    
    // Check if we have enough data for this test
    if (L < 6 || L > 16 || data.size() < Q * L) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Expected values for different block lengths
    std::vector<double> expected_value = {
        0, 0, 0, 0, 0, 0, 
        5.2177052, 6.1962507, 7.1836656, 8.1764248, 9.1723243, 
        10.170032, 11.168765, 12.168070, 13.167693, 14.167488, 15.167379
    };
    
    // Variances for different block lengths
    std::vector<double> variance = {
        0, 0, 0, 0, 0, 0, 
        2.954, 3.125, 3.238, 3.311, 3.356, 
        3.384, 3.401, 3.410, 3.416, 3.419, 3.421
    };
    
    // Calculate additional constants
    double c = 0.7 - 0.8 / L + (4 + 32 / L) * std::pow(K, -3.0 / L) / 15;
    double sigma = c * std::sqrt(variance[L] / K);
    
    // Initialize state table
    std::unordered_map<int, int> stateTable;
    
    // Initialize state table
    for (int i = 0; i < Q; i++) {
        int pattern = 0;
        for (int j = 0; j < L; j++) {
            pattern = (pattern << 1) | data[i * L + j];
        }
        stateTable[pattern] = i + 1;  // Store position + 1
    }
    
    // Process blocks
    double sum = 0.0;
    for (int i = Q; i < Q + K; i++) {
        int pattern = 0;
        for (int j = 0; j < L; j++) {
            pattern = (pattern << 1) | data[i * L + j];
        }
        
        int position = stateTable[pattern];
        sum += std::log2(i + 1 - position);
        stateTable[pattern] = i + 1;  // Update position
    }
    
    // Calculate final statistic
    double phi = sum / K;
    double standardized = (phi - expected_value[L]) / (std::sqrt(2) * sigma);
    double p_value = erfc(std::abs(standardized));
    
    // Store results
    result.statistics["L"] = L;
    result.statistics["Q"] = Q;
    result.statistics["K"] = K;
    result.statistics["sum"] = sum;
    result.statistics["sigma"] = sigma;
    result.statistics["expected_value"] = expected_value[L];
    result.statistics["phi"] = phi;
    result.statistics["standardized"] = standardized;
    result.statistics["discarded_bits"] = data.size() - (Q + K) * L;
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

} // namespace nist_sts