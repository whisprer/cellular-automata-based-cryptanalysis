// non_overlapping_template_test.cpp
#include "non_overlapping_template_test.hpp"
#include "../common.hpp"
#include "../math_functions.hpp"
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace nist_sts {

NonOverlappingTemplateTest::NonOverlappingTemplateTest(size_t blockLength)
    : blockLength(blockLength) {
    // Default: Generate templates
    generateTemplates();
}

TestResult NonOverlappingTemplateTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // Ensure we have enough data and templates
    if (data.size() < 8 * blockLength || templates.empty()) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Define test parameters
    const int N = 8;  // Number of blocks
    const int M = data.size() / N;  // Length of each block
    
    // Calculate lambda for theoretical distribution
    double lambda = (M - blockLength + 1) / std::pow(2, blockLength);
    double variance = M * (1.0 / std::pow(2.0, blockLength) - 
                         (2.0 * blockLength - 1.0) / std::pow(2.0, 2.0 * blockLength));
    
    // Process a subset of templates (for efficiency)
    int maxTemplates = 148;  // Default value for m=9
    if (blockLength <= 10) {
        // Template count based on NIST implementation
        int templateCounts[] = {0, 0, 2, 4, 6, 12, 20, 40, 74, 148, 284};
        maxTemplates = templateCounts[blockLength];
    }
    
    // Limit number of templates to process
    int templateCount = std::min(maxTemplates, static_cast<int>(templates.size()));
    
    // Store overall minimum p-value
    double min_p_value = 1.0;
    
    // Process each template
    for (int t = 0; t < templateCount; t++) {
        const std::vector<bool>& templ = templates[t];
        
        // Counters for each block
        std::vector<unsigned int> W(N, 0);
        
        // Process each block
        for (int i = 0; i < N; i++) {
            // Process positions within the block
            for (int j = 0; j < M - blockLength + 1; j++) {
                bool match = true;
                
                // Check if template matches at this position
                for (size_t k = 0; k < blockLength; k++) {
                    if (templ[k] != data[i * M + j + k]) {
                        match = false;
                        break;
                    }
                }
                
                if (match) {
                    W[i]++;
                    j += blockLength - 1;  // Skip ahead (non-overlapping)
                }
            }
        }
        
        // Calculate chi-squared statistic
        double chi_squared = 0.0;
        for (int i = 0; i < N; i++) {
            chi_squared += std::pow(W[i] - lambda, 2) / variance;
        }
        
        // Calculate p-value
        double p_value = igamc(N / 2.0, chi_squared / 2.0);
        
        // Store template results
        std::string templateKey = "template_" + std::to_string(t);
        result.statistics[templateKey + "_chi_squared"] = chi_squared;
        result.statistics[templateKey + "_p_value"] = p_value;
        
        // Track minimum p-value
        min_p_value = std::min(min_p_value, p_value);
    }
    
    // Store overall test statistics
    result.statistics["lambda"] = lambda;
    result.statistics["M"] = M;
    result.statistics["N"] = N;
    result.statistics["templates_tested"] = templateCount;
    
    // Use minimum p-value as the overall result
    result.p_value = min_p_value;
    result.success = isSuccess(min_p_value);
    
    return result;
}

void NonOverlappingTemplateTest::loadTemplates(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open template file: " + filename);
    }
    
    templates.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.length() >= blockLength) {
            std::vector<bool> templ(blockLength);
            for (size_t i = 0; i < blockLength; i++) {
                templ[i] = (line[i] == '1');
            }
            
            // Only use aperiodic templates
            if (isAperiodic(templ)) {
                templates.push_back(templ);
            }
        }
    }
    
    if (templates.empty()) {
        throw std::runtime_error("No valid templates found in file: " + filename);
    }
}

void NonOverlappingTemplateTest::generateTemplates() {
    templates.clear();
    
    // For m=9, NIST uses 148 templates
    // Here we generate a subset of possible templates for demonstration
    // In practice, you would want to use the standard NIST templates
    
    // Generate some example templates (all 1s, all 0s, alternating, etc.)
    std::vector<bool> allOnes(blockLength, true);
    std::vector<bool> allZeros(blockLength, false);
    
    std::vector<bool> alternating1(blockLength);
    std::vector<bool> alternating2(blockLength);
    for (size_t i = 0; i < blockLength; i++) {
        alternating1[i] = (i % 2 == 0);
        alternating2[i] = (i % 2 == 1);
    }
    
    templates.push_back(allOnes);
    templates.push_back(allZeros);
    templates.push_back(alternating1);
    templates.push_back(alternating2);
    
    // Add more templates as needed
    // In a full implementation, you would generate or load the complete template set
}

bool NonOverlappingTemplateTest::isAperiodic(const std::vector<bool>& templ) {
    // Check if template is aperiodic (has no proper period)
    for (size_t period = 1; period < templ.size(); period++) {
        bool isPeriodic = true;
        
        for (size_t i = 0; i < templ.size() - period; i++) {
            if (templ[i] != templ[i + period]) {
                isPeriodic = false;
                break;
            }
        }
        
        if (isPeriodic) {
            return false;  // Template has a proper period
        }
    }
    
    return true;  // Template is aperiodic
}

} // namespace nist_sts