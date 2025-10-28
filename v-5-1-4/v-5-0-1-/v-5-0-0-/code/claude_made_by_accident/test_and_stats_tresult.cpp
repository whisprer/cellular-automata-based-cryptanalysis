#pragma once

#include <string>
#include <map>
#include <vector>

namespace nist_sts {

// Constants
constexpr double ALPHA = 0.01;  // Default significance level

// Structure to hold test parameters
struct TestParameters {
    size_t n = 0;  // Sequence length
    size_t blockFrequencyBlockLength = 128;
    size_t nonOverlappingTemplateBlockLength = 9;
    size_t overlappingTemplateBlockLength = 9;
    size_t approximateEntropyBlockLength = 10;
    size_t serialBlockLength = 16;
    size_t linearComplexitySequenceLength = 500;
    size_t numOfBitStreams = 1;
};

// Standardized TestResult structure
struct TestResult {
    std::string testName;
    double p_value = 0.0;
    bool success = false;
    std::map<std::string, double> statistics;
};

struct TestResults {
    std::vector<TestResult> test_results;
    size_t data_size = 0;
};

// Base class for statistical tests
class StatisticalTest {
public:
    virtual ~StatisticalTest() = default;
    virtual TestResult execute(const std::vector<uint8_t>& data) = 0;
    virtual std::string getName() const = 0;
    
protected:
    bool isSuccess(double p_value) const {
        return p_value >= ALPHA;
    }
};

} // namespace nist_sts