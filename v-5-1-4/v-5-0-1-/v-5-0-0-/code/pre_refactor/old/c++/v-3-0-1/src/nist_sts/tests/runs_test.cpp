// runs_test.cpp
#include "runs_test.hpp"
#include "common.hpp"
#include <cmath>

namespace nist_sts {

TestResult RunsTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Calculate proportion of ones
    size_t S = data.countOnes();
    double pi = static_cast<double>(S) / static_cast<double>(data.size());
    
    // Pre-test: check if proportion is close to 1/2
    if (std::abs(pi - 0.5) > (2.0 / std::sqrt(static_cast<double>(data.size())))) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["pi"] = pi;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Count runs
    size_t V = 1;  // Start with 1 for the first run
    for (size_t k = 1; k < data.size(); k++) {
        if (data[k] != data[k-1]) {
            V++;
        }
    }
    
    // Calculate test statistic
    double erfc_arg = std::abs(static_cast<double>(V) - 2.0 * static_cast<double>(data.size()) * pi * (1-pi)) / 
                     (2.0 * pi * (1-pi) * std::sqrt(2.0 * static_cast<double>(data.size())));
    
    // Calculate p-value
    double p_value = erfc(erfc_arg);
    
    // Store results
    result.statistics["pi"] = pi;
    result.statistics["V_n_obs"] = static_cast<double>(V);
    result.statistics["erfc_arg"] = erfc_arg;
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

} // namespace nist_sts