// linear_complexity_test.cpp
#include "linear_complexity_test.hpp"
#include "../common.hpp"
#include "../math_functions.hpp"
#include <cmath>
#include <vector>

namespace nist_sts {

LinearComplexityTest::LinearComplexityTest(size_t sequenceLength)
    : sequenceLength(sequenceLength) {}

TestResult LinearComplexityTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Ensure we have enough data
    if (data.size() < sequenceLength) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Calculate number of blocks
    int N = data.size() / sequenceLength;
    
    // Define constants for the theoretical distribution
    double pi[7] = { 0.01047, 0.03125, 0.12500, 0.50000, 0.25000, 0.06250, 0.020833 };
    
    // Initialize frequency counts for K=6 (standard for this test)
    std::vector<int> nu(7, 0);
    
    for (int i = 0; i < N; i++) {
        // Extract the block
        std::vector<bool> block(sequenceLength);
        for (size_t j = 0; j < sequenceLength; j++) {
            block[j] = data[i * sequenceLength + j];
        }
        
        // Calculate linear complexity using Berlekamp-Massey
        int L = berlekampMassey(block);
        
        // Calculate the theoretical mean
        double mean = sequenceLength / 2.0 + (9.0 + (sequenceLength % 2 == 0 ? -1.0 : 1.0)) / 36.0 - 
                     1.0 / std::pow(2, sequenceLength) * (sequenceLength / 3.0 + 2.0 / 9.0);
        
        // Calculate the T statistic
        double T = (sequenceLength % 2 == 0 ? 1.0 : -1.0) * (L - mean) + 2.0 / 9.0;
        
        // Map T to a frequency bucket
        if (T <= -2.5)
            nu[0]++;
        else if (T <= -1.5)
            nu[1]++;
        else if (T <= -0.5)
            nu[2]++;
        else if (T <= 0.5)
            nu[3]++;
        else if (T <= 1.5)
            nu[4]++;
        else if (T <= 2.5)
            nu[5]++;
        else
            nu[6]++;
    }
    
    // Calculate chi-squared statistic
    double chi_squared = 0.0;
    for (int i = 0; i < 7; i++) {
        chi_squared += std::pow(nu[i] - N * pi[i], 2) / (N * pi[i]);
    }
    
    // Calculate p-value
    double p_value = igamc(3.0, chi_squared / 2.0);
    
    // Store results
    result.statistics["chi_squared"] = chi_squared;
    for (int i = 0; i < 7; i++) {
        result.statistics["nu_" + std::to_string(i)] = nu[i];
    }
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

int LinearComplexityTest::berlekampMassey(const std::vector<bool>& sequence) {
    int L = 0, m = -1;
    std::vector<int> C(sequence.size(), 0);
    std::vector<int> B(sequence.size(), 0);
    std::vector<int> T(sequence.size(), 0);
    
    C[0] = 1;
    B[0] = 1;
    
    for (size_t n = 0; n < sequence.size(); n++) {
        // Compute discrepancy
        int d = sequence[n];
        for (int i = 1; i <= L; i++) {
            d ^= (C[i] & sequence[n - i]);
        }
        
        if (d == 1) {
            // Update connection polynomial
            std::copy(C.begin(), C.end(), T.begin());
            
            for (size_t j = 0; j < sequence.size(); j++) {
                if (n - m + j < sequence.size()) {
                    C[n - m + j] ^= B[j];
                }
            }
            
            if (L <= static_cast<int>(n) / 2) {
                L = n + 1 - L;
                m = n;
                std::copy(T.begin(), T.end(), B.begin());
            }
        }
    }
    
    return L;
}

} // namespace nist_sts