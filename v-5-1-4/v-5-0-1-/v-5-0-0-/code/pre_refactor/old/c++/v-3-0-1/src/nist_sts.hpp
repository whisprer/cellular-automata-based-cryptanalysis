/*****************************************************
 * File: nist_sts.hpp
 *****************************************************/
#pragma once

#include <vector>
#include <string>
#include <map>

namespace nist_sts {

// Forward-declare a struct for test results
struct TestResult {
    std::string testName;
    double p_value = 0.0;
    bool success   = false;
    std::map<std::string, double> statistics;
};

// Forward-declare
class NISTTestSuite {
public:
    // Constructor
    NISTTestSuite() = default;

    // Run all tests on the raw data
    std::vector<TestResult> runAllTests(const std::vector<uint8_t>& data);

    // Generate a summary string for a given data set
    std::string generateSummary(const std::vector<uint8_t>& data);

private:
    // Helper test-group methods
    std::vector<TestResult> runFrequencyTests(const std::vector<uint8_t>& data);
    // Add more as needed
};

} // namespace nist_sts
