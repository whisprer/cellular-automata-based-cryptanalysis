// test_suite.hpp
#pragma once
#include "common.hpp"
#include "bitsequence.hpp"
#include "statistical_test.hpp"
#include "random_number_generator.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

namespace nist_sts {

class TestSuite {
private:
    std::vector<std::unique_ptr<StatisticalTest>> tests;
    TestParameters params;
    
public:
    TestSuite();
    
    // Test management
    void addTest(std::unique_ptr<StatisticalTest> test);
    void removeTest(const std::string& testName);
    void clearTests();
    
    // Parameter management
    void setParameters(const TestParameters& params);
    TestParameters getParameters() const;
    
    // Running tests
    TestResults runTests(const BitSequence& data);
    TestResults runTests(const std::string& filename, bool isAscii = true);
    
    // Default test suite creation - STATIC METHOD
    static TestSuite createDefaultSuite();
    
    // Reporting
    void saveResults(const TestResults& results, const std::string& filename) const;
    std::string generateSummary(const TestResults& results) const;

    // Test a specific generator
    TestResults testGenerator(RandomNumberGenerator& generator, size_t length);
    
    // Test all available generators
    std::map<std::string, TestResults> testAllGenerators(size_t length);
};

} // namespace nist_sts