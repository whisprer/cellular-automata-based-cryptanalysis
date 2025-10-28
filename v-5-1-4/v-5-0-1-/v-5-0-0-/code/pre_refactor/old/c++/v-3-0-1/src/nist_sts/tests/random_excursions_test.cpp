// random_excursions_test.cpp
#include "random_excursions_test.hpp"
#include "common.hpp"
#include "math_functions.hpp"
#include <cmath>
#include <algorithm>

namespace nist_sts {

TestResult RandomExcursionsTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Convert bits to -1/+1 random walk
    std::vector<int> S(data.size() + 1, 0);
    for (size_t i = 0; i < data.size(); i++) {
        S[i + 1] = S[i] + (data[i] ? 1 : -1);
    }
    
    // Find cycles in random walk
    std::vector<int> cycles = findCycles(S);
    int J = static_cast<int>(cycles.size());
    
    // Check if we have enough cycles
    double constraint = std::max(0.005 * std::sqrt(static_cast<double>(data.size())), 500.0);
    if (J < static_cast<int>(constraint)) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["cycles"] = static_cast<double>(J);
        result.statistics["constraint"] = constraint;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // States to examine
    std::vector<int> stateX = {-4, -3, -2, -1, 1, 2, 3, 4};
    
    // Theoretical probabilities for each state and visit count
    std::vector<std::vector<double>> pi = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.5, 0.25, 0.125, 0.0625, 0.0312, 0.0312},
        {0.75, 0.0625, 0.0469, 0.0352, 0.0264, 0.0791},
        {0.8333, 0.0278, 0.0231, 0.0193, 0.0161, 0.0804},
        {0.875, 0.0156, 0.0137, 0.012, 0.0105, 0.0733}
    };
    
    // Store results for each state
    for (size_t x_idx = 0; x_idx < stateX.size(); x_idx++) {
        int x = stateX[x_idx];
        int absX = std::abs(x);
        
        // Count visits to state x in each cycle
        std::vector<int> visitCounts(6, 0);
        
        for (size_t i = 0; i < static_cast<size_t>(J) - 1; i++) {
            int count = 0;
            for (int j = cycles[i]; j < cycles[i+1]; j++) {
                if (S[j] == x) {
                    count++;
                }
            }
            if (count >= 5) {
                visitCounts[5]++;
            } else {
                visitCounts[count]++;
            }
        }
        
        // Calculate chi-squared statistic
        double chi_squared = 0.0;
        for (int k = 0; k < 6; k++) {
            chi_squared += std::pow(visitCounts[k] - J * pi[absX][k], 2) / (J * pi[absX][k]);
        }
        
        // Calculate p-value
        double p_value = igamc(2.5, chi_squared / 2.0);
        
        // Store results for this state
        std::string key = "state_" + std::to_string(x);
        result.statistics[key + "_chi_squared"] = chi_squared;
        result.statistics[key + "_p_value"] = p_value;
        
        // For the first state, use its p-value as the result
        if (x_idx == 0) {
            result.p_value = p_value;
            result.success = isSuccess(p_value);
        }
    }
    
    return result;
}

std::vector<int> RandomExcursionsTest::findCycles(const std::vector<int>& S) {
    std::vector<int> cycles;
    cycles.push_back(0);  // Start of first cycle
    
    for (size_t i = 1; i < S.size(); i++) {
        if (S[i] == 0) {
            cycles.push_back(static_cast<int>(i));
        }
    }
    
    if (S.back() != 0) {
        cycles.push_back(static_cast<int>(S.size() - 1));  // End of last cycle
    }
    
    return cycles;
}

} // namespace nist_sts