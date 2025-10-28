// test_suite.cpp
#include "test_suite.hpp"
#include <fstream>
#include <iomanip>

namespace nist_sts {

TestSuite::TestSuite() {
    // Default initialization
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

TestResults TestSuite::runTests(const BitSequence& data) {
    TestResults results;
    results.data_size = data.size();
    
    for (const auto& test : tests) {
        results.test_results.push_back(test->execute(data));
    }
    
    // Generate summary if needed
    return results;
}

TestResults TestSuite::runTests(RandomNumberGenerator& generator) {
    BitSequence data = generator.generate(params.n);
    return runTests(data);

TestResults TestSuite::testGenerator(RandomNumberGenerator& generator, size_t length) {
    // Generate random bits
    BitSequence data = generator.generate(length);
        
    // Run tests on the generated data
    return runTests(data);
}
    
std::map<std::string, TestResults> TestSuite::testAllGenerators(size_t length) {
    std::map<std::string, TestResults> results;
    auto generators = GeneratorFactory::createAllGenerators();
        
    for (const auto& generator : generators) {
        std::string name = generator->getName();
        results[name] = testGenerator(*generator, length);
    }
        
    return results;
}

} // namespace nist_sts