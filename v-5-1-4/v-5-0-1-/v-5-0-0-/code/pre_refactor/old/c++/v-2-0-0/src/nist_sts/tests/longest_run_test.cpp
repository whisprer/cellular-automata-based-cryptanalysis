// longest_run_test.cpp
#include "longest_run_test.hpp"
#include "common.hpp"
#include "math_functions.hpp"
#include <cmath>
#include <array>

namespace nist_sts {

TestResult LongestRunTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Check if we have enough data
    if (data.size() < 128) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        result.statistics["message"] = 1.0;  // "n too short"
        return result;
    }
    
    // Determine the parameters based on sequence length
    int K, M;
    std::vector<int> V;
    std::vector<double> pi;
    
    if (data.size() < 6272) {
        K = 3;
        M = 8;
        V = {1, 2, 3, 4};
        pi = {0.21484375, 0.3671875, 0.23046875, 0.1875};
    } else if (data.size() < 750000) {
        K = 5;
        M = 128;
        V = {4, 5, 6, 7, 8, 9};
        pi = {0.1174035788, 0.242955959, 0.249363483, 0.17517706, 0.102701071, 0.112398847};
    } else {
        K = 6;
        M = 10000;
        V = {10, 11, 12, 13, 14, 15, 16};
        pi = {0.0882, 0.2092, 0.2483, 0.1933, 0.1208, 0.0675, 0.0727};
    }
    
    // Number of blocks
    int N = data.size() / M;
    
    // Initialize frequency counts
    std::vector<unsigned int> nu(K + 1, 0);
    
    // Process each block
    for (int i = 0; i < N; i++) {
        int maxRunLength = 0;
        int currentRun = 0;
        
        for (int j = 0; j < M; j++) {
            if (data[i*M + j]) {
                currentRun++;
                if (currentRun > maxRunLength) {
                    maxRunLength = currentRun;
                }
            } else {
                currentRun = 0;
            }
        }
        
        // Update frequency count
        if (maxRunLength < V[0]) {
            nu[0]++;
        } else if (maxRunLength > V[K]) {
            nu[K]++;
        } else {
            for (int j = 0; j <= K; j++) {
                if (maxRunLength == V[j]) {
                    nu[j]++;
                    break;
                }
            }
        }
    }
    
    // Calculate chi-squared statistic
    double chi_squared = 0.0;
    for (int i = 0; i <= K; i++) {
        chi_squared += std::pow(nu[i] - N * pi[i], 2) / (N * pi[i]);
    }
    
    // Calculate p-value
    double p_value = igamc(K / 2.0, chi_squared / 2.0);
    
    // Store results
    result.statistics["chi_squared"] = chi_squared;
    result.statistics["block_count"] = N;
    result.statistics["block_length"] = M;
    
    for (size_t i = 0; i <= K; i++) {
        result.statistics["nu_" + std::to_string(i)] = nu[i];
    }
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

} // namespace nist_sts