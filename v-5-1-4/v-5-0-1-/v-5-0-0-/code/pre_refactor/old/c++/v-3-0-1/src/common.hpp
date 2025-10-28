// common.hpp
#pragma once
#include <string>
#include <map>
#include <vector>
#include <bitset>

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

namespace nist_sts {

    struct TestResult {
        std::string testName;
        double p_value;
        bool success;
        std::map<std::string, double> statistics;
    };
    
    } // namespace nist_sts

struct TestResult {
    std::string testName;
    double p_value = 0.0;
    bool success = false;
    std::map<std::string, double> statistics;  // Additional statistics
};

struct TestResults {
    std::vector<TestResult> test_results;
    size_t data_size = 0;
};

// Base class for all statistical tests
class StatisticalTest {
public:
    virtual ~StatisticalTest() = default;
    virtual TestResult execute(const class BitSequence& data) = 0;
    virtual std::string getName() const = 0;
    
protected:
    bool isSuccess(double p_value) const {
        return p_value >= ALPHA;
    }
};

} // namespace nist_sts