// test_suite.cpp
// test_suite.cpp
#include "test_suite.hpp"
#include "nist_sts/tests/frequency_test.hpp"
#include "nist_sts/tests/block_frequency_test.hpp"
#include "nist_sts/tests/runs_test.hpp"
#include "nist_sts/tests/longest_run_test.hpp"
#include "nist_sts/tests/rank_test.hpp"
#include "nist_sts/tests/dft_test.hpp"
#include "nist_sts/tests/non_overlapping_template_test.hpp"
#include "nist_sts/tests/overlapping_template_test.hpp"
#include "nist_sts/tests/universal_test.hpp"
#include "nist_sts/tests/linear_complexity_test.hpp"
#include "nist_sts/tests/serial_test.hpp"
#include "nist_sts/tests/approximate_entropy_test.hpp"
#include "nist_sts/tests/cumulative_sums_test.hpp"
#include "nist_sts/tests/random_excursions_test.hpp"
#include "nist_sts/tests/random_excursions_variant_test.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace nist_sts {

// Existing methods from previous implementation...

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

TestResults TestSuite::testGenerator(RandomNumberGenerator& generator, size_t length) {
    // Generate a bit sequence of the specified length
    BitSequence data = generator.generate(length);
    
    // Run tests on the generated data
    return runTests(data);
}

std::map<std::string, TestResults> TestSuite::testAllGenerators(size_t length) {
    // This is a placeholder implementation
    // In a real scenario, you'd use a generator factory to create generators
    std::map<std::string, TestResults> results;
    return results;
}

// Constructor implementation
nist_sts::TestSuite::TestSuite() {
    // Initialize with default parameters
}

// Add a test implementation
void nist_sts::TestSuite::addTest(std::unique_ptr<StatisticalTest> test) {
    tests.push_back(std::move(test));
}

// Run tests on bit sequence implementation
nist_sts::TestResults nist_sts::TestSuite::runTests(const BitSequence& data) {
    TestResults results;
    results.data_size = data.size();
    
    for (const auto& test : tests) {
        TestResult result = test->execute(data);
        results.test_results.push_back(result);
    }
    
    return results;
}

// Generate a summary report implementation
std::string nist_sts::TestSuite::generateSummary(const TestResults& results) const {
    std::stringstream ss;
    
    ss << "NIST Statistical Test Suite Results\n";
    ss << "-----------------------------------\n";
    ss << "Data size: " << results.data_size << " bits\n\n";
    
    ss << "Test Name                         P-Value    Result\n";
    ss << "---------------------------------------------------\n";
    
    for (const auto& result : results.test_results) {
        ss << std::left << std::setw(35) << result.testName.substr(0, 34) 
           << std::fixed << std::setprecision(6) << std::setw(10) << result.p_value
           << (result.success ? "SUCCESS" : "FAILURE") << "\n";
    }
    
    return ss.str();
}
} // namespace nist_sts