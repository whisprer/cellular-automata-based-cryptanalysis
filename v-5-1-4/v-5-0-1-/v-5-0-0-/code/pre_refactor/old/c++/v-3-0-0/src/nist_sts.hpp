// nist_sts.hpp - Wrapper for the NIST Statistical Test Suite
#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace nist_sts {

// Forward declarations
class BitSequence;
struct TestResult;  // Forward declarationt;
class TestResults;
class TestSuite;

// NIST STS Wrapper class
class NISTTestSuite {
private:
    std::unique_ptr<TestSuite> testSuite;
    
public:
    NISTTestSuite();
    ~NISTTestSuite();
    
    // Run tests on binary data
    std::map<std::string, double> runTests(const std::vector<uint8_t>& data);
    
    // Generate a summary report
    std::string generateSummary(const std::vector<uint8_t>& data);
    
private:
    // Convert vector<uint8_t> to BitSequence
    BitSequence convertToBitSequence(const std::vector<uint8_t>& data);
};

} // namespace nist_sts