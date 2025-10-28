// block_frequency_test.cpp
#include "block_frequency_test.hpp"
#include "common.hpp"
#include "math_functions.hpp"
#include <cmath>

namespace nist_sts {

BlockFrequencyTest::BlockFrequencyTest(size_t blockLength)
    : blockLength(blockLength) {}

TestResult BlockFrequencyTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Ensure we have enough data
    if (data.size() < blockLength) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Calculate number of blocks
    int N = data.size() / blockLength;
    double sum = 0.0;
    
    // Process each block
    for (int i = 0; i < N; i++) {
        int blockSum = 0;
        
        // Count ones in the block
        for (size_t j = 0; j < blockLength; j++) {
            blockSum += data[j + i * blockLength];
        }
        
        // Calculate proportion of ones in the block
        double pi = static_cast<double>(blockSum) / blockLength;
        
        // Accumulate chi-squared statistic
        double v = pi - 0.5;
        sum += v * v;
    }
    
    // Calculate final chi-squared statistic
    double chi_squared = 4.0 * blockLength * sum;
    
    // Calculate p-value using incomplete gamma function
    double p_value = igamc(N / 2.0, chi_squared / 2.0);
    
    // Store results
    result.statistics["chi_squared"] = chi_squared;
    result.statistics["num_blocks"] = N;
    result.statistics["block_length"] = blockLength;
    result.statistics["discarded_bits"] = data.size() % blockLength;
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

} // namespace nist_sts