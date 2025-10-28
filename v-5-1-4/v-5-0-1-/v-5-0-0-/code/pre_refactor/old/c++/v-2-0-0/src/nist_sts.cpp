// nist_sts.cpp
#include "nist_sts.hpp"
#include "test_suite.hpp"

namespace nist_sts {

NISTTestSuite::NISTTestSuite() {
    // Create a default test suite
    testSuite = std::make_unique<TestSuite>(TestSuite::createDefaultSuite());
}

NISTTestSuite::~NISTTestSuite() = default;

std::map<std::string, double> NISTTestSuite::runTests(const std::vector<uint8_t>& data) {
    // Convert uint8_t to BitSequence
    BitSequence bitSeq(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            bitSeq.push_back((data[i] >> (7-j)) & 1);
        }
    }
    
    // Run tests and collect p-values
    TestResults results = testSuite->runTests(bitSeq);
    std::map<std::string, double> pValues;
    
    for (const auto& result : results.test_results) {
        pValues[result.testName] = result.p_value;
    }
    
    return pValues;
}

std::string NISTTestSuite::generateSummary(const std::vector<uint8_t>& data) {
    // Convert uint8_t to BitSequence (same as runTests)
    BitSequence bitSeq(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            bitSeq.push_back((data[i] >> (7-j)) & 1);
        }
    }
    
    // Generate summary using test suite
    TestResults results = testSuite->runTests(bitSeq);
    return testSuite->generateSummary(results);
}

} // namespace nist_sts