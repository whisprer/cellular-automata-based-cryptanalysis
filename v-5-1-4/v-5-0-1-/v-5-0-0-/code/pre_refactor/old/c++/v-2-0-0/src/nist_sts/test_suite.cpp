// test_suite.cpp
// test_suite.cpp - Completely fixed version
#include "test_suite.hpp"
#include "tests/frequency_test.hpp"
#include "tests/block_frequency_test.hpp"
#include "tests/runs_test.hpp"
#include "tests/longest_run_test.hpp"
#include "tests/rank_test.hpp"
#include "tests/dft_test.hpp"
#include "tests/non_overlapping_template_test.hpp"
#include "tests/overlapping_template_test.hpp"
#include "tests/universal_test.hpp"
#include "tests/linear_complexity_test.hpp"
#include "tests/serial_test.hpp"
#include "tests/approximate_entropy_test.hpp"
#include "tests/cumulative_sums_test.hpp"
#include "tests/random_excursions_test.hpp"
#include "tests/random_excursions_variant_test.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace nist_sts {

TestSuite::TestSuite() {
    // Default parameters
    params.n = 1000000;  // 1 million bits
    params.blockFrequencyBlockLength = 128;
    params.nonOverlappingTemplateBlockLength = 9;
    params.overlappingTemplateBlockLength = 9;
    params.approximateEntropyBlockLength = 10;
    params.serialBlockLength = 16;
    params.linearComplexitySequenceLength = 500;
    params.numOfBitStreams = 1;
}

void TestSuite::addTest(std::unique_ptr<StatisticalTest> test) {
    tests.push_back(std::move(test));
}

void TestSuite::removeTest(const std::string& testName) {
    tests.erase(
        std::remove_if(tests.begin(), tests.end(),
            [&testName](const auto& test) { 
                return test->getName() == testName; 
            }),
        tests.end()
    );
}

void TestSuite::clearTests() {
    tests.clear();
}

void TestSuite::setParameters(const TestParameters& p) {
    params = p;
}

TestParameters TestSuite::getParameters() const {
    return params;
}

TestResults TestSuite::runTests(const BitSequence& data) {
    TestResults results;
    results.data_size = data.size();
    
    for (const auto& test : tests) {
        try {
            results.test_results.push_back(test->execute(data));
        } catch (const std::exception& e) {
            // Create a failure result for this test
            TestResult result;
            result.testName = test->getName();
            result.p_value = 0.0;
            result.success = false;
            result.statistics["error"] = 1.0;
            result.statistics["error_message"] = 1.0;  // Can't store string in double
            
            results.test_results.push_back(result);
        }
    }
    
    return results;
}

TestResults TestSuite::runTests(const std::string& filename, bool isAscii) {
    BitSequence data;
    
    try {
        if (isAscii) {
            data = BitSequence::fromAsciiFile(filename);
        } else {
            data = BitSequence::fromBinaryFile(filename);
        }
    } catch (const std::exception& e) {
        TestResults results;
        results.data_size = 0;
        // Add empty results with errors
        for (const auto& test : tests) {
            TestResult result;
            result.testName = test->getName();
            result.p_value = 0.0;
            result.success = false;
            result.statistics["error"] = 1.0;
            result.statistics["file_error"] = 1.0;
            
            results.test_results.push_back(result);
        }
        return results;
    }
    
    return runTests(data);
}

TestSuite TestSuite::createDefaultSuite() {
    TestSuite suite;
    
    // Add all tests with default parameters
    suite.addTest(std::make_unique<FrequencyTest>());
    suite.addTest(std::make_unique<BlockFrequencyTest>());
    suite.addTest(std::make_unique<RunsTest>());
    suite.addTest(std::make_unique<LongestRunTest>());
    suite.addTest(std::make_unique<RankTest>());
    suite.addTest(std::make_unique<DiscreteFourierTransformTest>());
    suite.addTest(std::make_unique<NonOverlappingTemplateTest>());
    suite.addTest(std::make_unique<OverlappingTemplateTest>());
    suite.addTest(std::make_unique<UniversalTest>());
    suite.addTest(std::make_unique<LinearComplexityTest>());
    suite.addTest(std::make_unique<SerialTest>());
    suite.addTest(std::make_unique<ApproximateEntropyTest>());
    suite.addTest(std::make_unique<CumulativeSumsTest>());
    suite.addTest(std::make_unique<RandomExcursionsTest>());
    suite.addTest(std::make_unique<RandomExcursionsVariantTest>());
    
    return suite;
}

void TestSuite::saveResults(const TestResults& results, const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }
    
    file << generateSummary(results);
}

std::string TestSuite::generateSummary(const TestResults& results) const {
    std::stringstream ss;
    
    ss << "------------------------------------------------------------------------------\n";
    ss << "RESULTS FOR THE RANDOMNESS TESTS\n";
    ss << "------------------------------------------------------------------------------\n";
    ss << "Sequence length: " << results.data_size << " bits\n";
    ss << "------------------------------------------------------------------------------\n";
    ss << std::left << std::setw(30) << "Test Name" 
       << std::setw(15) << "P-Value" 
       << std::setw(10) << "Result" << "\n";
    ss << "------------------------------------------------------------------------------\n";
    
    for (const auto& result : results.test_results) {
        ss << std::left << std::setw(30) << result.testName 
           << std::setw(15) << std::fixed << std::setprecision(6) << result.p_value
           << std::setw(10) << (result.success ? "SUCCESS" : "FAILURE") << "\n";
        
        // Add additional statistics if available
        for (const auto& pair : result.statistics) {
            const std::string& key = pair.first;
            double value = pair.second;
            
            if (key.find("error") == std::string::npos && 
                key.find("p_value") == std::string::npos) {
                ss << "  " << std::left << std::setw(28) << key 
                   << std::setw(15) << std::fixed << std::setprecision(6) << value << "\n";
            }
        }
        
        ss << "\n";
    }
    
    // Add summary of test results
    int passCount = 0;
    for (const auto& result : results.test_results) {
        if (result.success) {
            passCount++;
        }
    }
    
    double passRate = static_cast<double>(passCount) / results.test_results.size() * 100;
    
    ss << "------------------------------------------------------------------------------\n";
    ss << "SUMMARY\n";
    ss << "------------------------------------------------------------------------------\n";
    ss << "Tests passed: " << passCount << " out of " << results.test_results.size() 
       << " (" << std::fixed << std::setprecision(2) << passRate << "%)\n";
    ss << "Alpha (significance level): " << ALPHA << "\n";
    ss << "------------------------------------------------------------------------------\n";
    
    return ss.str();
}

// Implementation for testGenerator method
TestResults TestSuite::testGenerator(RandomNumberGenerator& generator, size_t length) {
    // Generate a bit sequence of the specified length
    BitSequence data = generator.generate(length);
    
    // Run tests on the generated data
    return runTests(data);
}

// Implementation for testAllGenerators method
std::map<std::string, TestResults> TestSuite::testAllGenerators(size_t length) {
    // This would require some sort of generator factory or registry
    // For now, return an empty map
    return std::map<std::string, TestResults>();
}

} // namespace nist_sts