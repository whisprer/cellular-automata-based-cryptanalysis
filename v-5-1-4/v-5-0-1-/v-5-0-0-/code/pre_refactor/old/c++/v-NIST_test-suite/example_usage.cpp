#include "nist_sts/test_suite.hpp"
#include "nist_sts/tests/frequency.hpp"
#include "nist_sts/tests/runs.hpp"
#include "nist_sts/generators/lcg.hpp"
#include <iostream>

int main() {
    using namespace nist_sts;
    
    // Create test suite
    TestSuite suite;
    
    // Add tests
    suite.addTest(std::make_unique<FrequencyTest>());
    suite.addTest(std::make_unique<RunsTest>());
    
    // Set parameters
    TestParameters params;
    params.n = 1000000;  // Sequence length
    suite.setParameters(params);
    
    // Generate data using LCG
    generators::LinearCongruentialGenerator lcg;
    
    // Run tests
    TestResults results = suite.runTests(lcg);
    
    // Display results
    std::cout << "Test Results:" << std::endl;
    for (const auto& result : results.test_results) {
        std::cout << result.testName << ": " 
                  << (result.success ? "SUCCESS" : "FAILURE")
                  << " (p-value = " << result.p_value << ")" << std::endl;
    }
    
    return 0;
}