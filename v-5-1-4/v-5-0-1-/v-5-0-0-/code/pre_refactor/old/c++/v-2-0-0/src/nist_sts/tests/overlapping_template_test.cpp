// overlapping_template_test.cpp
#include "overlapping_template_test.hpp"
#include "common.hpp"
#include "math_functions.hpp"
#include <cmath>
#include <vector>

namespace nist_sts {

OverlappingTemplateTest::OverlappingTemplateTest(size_t blockLength)
    : blockLength(blockLength) {
    // Default template is all ones
    template_.resize(blockLength, true);
}

TestResult OverlappingTemplateTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Ensure we have enough data
    if (data.size() < 1032) {  // This is the standard minimum size for this test
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Define test parameters
    const int M = 1032;  // Length of each substring analyzed
    const int N = data.size() / M;  // Number of substrings
    const int K = 5;  // Number of categories for the chi-squared test
    
    // Theoretical probabilities for each category
    std::vector<double> pi = {0.364091, 0.185659, 0.139381, 0.100571, 0.0704323, 0.139865};
    
    // Calculate lambda and eta for theoretical distribution
    double lambda = static_cast<double>(M - blockLength + 1) / std::pow(2, blockLength);
    double eta = lambda / 2.0;
    
    // Initialize frequency counts
    std::vector<unsigned int> nu(K + 1, 0);
    
    // Process each substring
    for (int i = 0; i < N; i++) {
        int W_obs = 0;  // Number of template matches in this substring
        
        // Check for matches in the current substring
        for (int j = 0; j < M - blockLength + 1; j++) {
            bool match = true;
            
            // Check if the template matches at this position
            for (size_t k = 0; k < blockLength; k++) {
                if (template_[k] != data[i * M + j + k]) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                W_obs++;
            }
        }
        
        // Update frequency count
        if (W_obs <= 4) {
            nu[W_obs]++;
        } else {
            nu[K]++;
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
    result.statistics["lambda"] = lambda;
    result.statistics["eta"] = eta;
    result.statistics["chi_squared"] = chi_squared;
    
    for (int i = 0; i <= K; i++) {
        result.statistics["nu_" + std::to_string(i)] = nu[i];
    }
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

double OverlappingTemplateTest::calculateProbability(int u, double eta) {
    double sum, p;
    
    if (u == 0) {
        p = std::exp(-eta);
    } else {
        sum = 0.0;
        for (int l = 1; l <= u; l++) {
            sum += std::exp(-eta - u * std::log(2) + l * std::log(eta) - 
                    lgam(l + 1) + lgam(u) - lgam(l) - lgam(u - l + 1));
        }
        p = sum;
    }
    
    return p;
}

} // namespace nist_sts