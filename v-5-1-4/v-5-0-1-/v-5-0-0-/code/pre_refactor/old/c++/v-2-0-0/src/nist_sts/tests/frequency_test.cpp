// frequency_test.cpp
#include "frequency_test.hpp"
#include "common.hpp"
#include <cmath>

namespace nist_sts {

TestResult FrequencyTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    double sum = 0.0;
    for (size_t i = 0; i < data.size(); i++) {
        sum += 2 * static_cast<int>(data[i]) - 1;  // Convert to +1/-1
    }
    
    double s_obs = std::abs(sum) / std::sqrt(data.size());
    double p_value = erfc(s_obs / std::sqrt(2.0));
    
    result.statistics["sum"] = sum;
    result.statistics["s_obs"] = s_obs;
    result.statistics["normalized_sum"] = sum / data.size();
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

} // namespace nist_sts