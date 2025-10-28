// random_excursions_variant_test.cpp
#include "random_excursions_variant_test.hpp"
#include "../common.hpp"
#include <cmath>
#include <algorithm>
#include <array>

namespace nist_sts {

TestResult RandomExcursionsVariantTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Convert bits to -1/+1 random walk
    std::vector<int> S(data.size() + 1, 0);
    for (size_t i = 0; i < data.size(); i++) {
        S[i + 1] = S[i] + (data[i] ? 1 : -1);
    }
    
    // Count cycles in random walk
    int J = 0;
    for (size_t i = 0; i < S.size(); i++) {
        if (S[i] == 0) {
            J++;
        }
    }
    
    // Check if we have enough cycles
    double constraint = std::max(0.005 * std::sqrt(data.size()), 500.0);
    if (J < constraint) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["cycles"] = J;
        result.statistics["constraint"] = constraint;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // States to examine: -9 to -1 and 1 to 9
    std::array<int, 18> stateX = {-9, -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Store results for each state
    for (size_t i = 0; i < stateX.size(); i++) {
        int x = stateX[i];
        
        // Count visits to state x
        int count = 0;
        for (size_t j = 0; j < S.size(); j++) {
            if (S[j] == x) {
                count++;
            }
        }
        
        // Calculate p-value
        double p_value = erfc(std::abs(count - J) / 
                             (std::sqrt(2.0 * J * (4.0 * std::abs(x) - 2.0))));
        
        // Store results for this state
        std::string key = "state_" + std::to_string(x);
        result.statistics[key + "_visits"] = count;
        result.statistics[key + "_p_value"] = p_value;
        
        // For the first state, use its p-value as the result
        if (i == 0) {
            result.p_value = p_value;
            result.success = isSuccess(p_value);
        }
    }
    
    return result;
}

} // namespace nist_sts