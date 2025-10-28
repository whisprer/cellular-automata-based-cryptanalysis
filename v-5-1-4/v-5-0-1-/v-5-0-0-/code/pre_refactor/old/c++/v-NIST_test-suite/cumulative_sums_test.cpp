// cumulative_sums_test.cpp
#include "cumulative_sums_test.hpp"
#include "../common.hpp"
#include <cmath>
#include <algorithm>

namespace nist_sts {

TestResult CumulativeSumsTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Initialize variables
    int S = 0;
    int sup = 0;
    int inf = 0;
    
    // Calculate forward mode
    for (size_t k = 0; k < data.size(); k++) {
        S += (data[k] ? 1 : -1);
        sup = std::max(sup, S);
        inf = std::min(inf, S);
    }
    
    int z = std::max(sup, -inf);
    
    // Calculate backward mode
    S = 0;
    int supRev = 0;
    int infRev = 0;
    
    for (int k = data.size() - 1; k >= 0; k--) {
        S += (data[k] ? 1 : -1);
        supRev = std::max(supRev, S);
        infRev = std::min(infRev, S);
    }
    
    int zRev = std::max(supRev, -infRev);
    
    // Calculate p-values for forward and backward modes
    double p_value_forward = calculatePValue(data.size(), z);
    double p_value_backward = calculatePValue(data.size(), zRev);
    
    // Store results
    result.statistics["forward_max_partial_sum"] = z;
    result.statistics["backward_max_partial_sum"] = zRev;
    result.statistics["p_value_forward"] = p_value_forward;
    result.statistics["p_value_backward"] = p_value_backward;
    
    // Use the minimum p-value as the test result
    result.p_value = std::min(p_value_forward, p_value_backward);
    result.success = isSuccess(result.p_value);
    
    return result;
}

double CumulativeSumsTest::calculatePValue(size_t n, int z) {
    double sum1 = 0.0;
    double sum2 = 0.0;
    
    // Calculate first sum
    for (int k = (-n / z + 1) / 4; k <= (n / z - 1) / 4; k++) {
        sum1 += normal(((4 * k + 1) * z) / std::sqrt(n));
        sum1 -= normal(((4 * k - 1) * z) / std::sqrt(n));
    }
    
    // Calculate second sum
    for (int k = (-n / z - 3) / 4; k <= (n / z - 1) / 4; k++) {
        sum2 += normal(((4 * k + 3) * z) / std::sqrt(n));
        sum2 -= normal(((4 * k + 1) * z) / std::sqrt(n));
    }
    
    double p_value = 1.0 - sum1 + sum2;
    return p_value;
}

double CumulativeSumsTest::normal(double x) {
    if (x > 0) {
        return 0.5 * (1.0 + erf(x / std::sqrt(2.0)));
    } else {
        return 0.5 * (1.0 - erf(-x / std::sqrt(2.0)));
    }
}

} // namespace nist_sts