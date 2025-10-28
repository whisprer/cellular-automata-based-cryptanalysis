// File: include/caca/tests/statistical_tests.hpp
#pragma once

#include "caca/core/bitsequence.hpp"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cmath>

namespace caca::tests {

/**
 * @struct TestResult
 * @brief Structure to hold the result of a statistical test
 */
struct TestResult {
    std::string test_name;
    double p_value = 0.0;
    bool passed = false;
    std::map<std::string, double> additional_metrics;
};

/**
 * @class StatisticalTest
 * @brief Base class for all statistical tests
 * 
 * This abstract class defines the interface for statistical tests
 * used to evaluate the randomness of bit sequences.
 */
class StatisticalTest {
protected:
    // Default significance level (alpha)
    static constexpr double DEFAULT_ALPHA = 0.01;
    
    // Significance level
    double m_alpha = DEFAULT_ALPHA;
    
    // Common statistical functions
    static double erfc(double x) {
        return 1.0 - erf(x);
    }
    
    static double erf(double x) {
        // Constants for Abramowitz and Stegun approximation
        const double a1 = 0.254829592;
        const double a2 = -0.284496736;
        const double a3 = 1.421413741;
        const double a4 = -1.453152027;
        const double a5 = 1.061405429;
        const double p = 0.3275911;
        
        // Save the sign
        int sign = (x < 0) ? -1 : 1;
        x = std::fabs(x);
        
        // A&S formula 7.1.26
        double t = 1.0 / (1.0 + p * x);
        double y = 1.0 - ((((a5 * t + a4) * t + a3) * t + a2) * t + a1) * t * std::exp(-x * x);
        
        return sign * y;
    }
    
    // Check if p-value indicates passing test
    bool isPassed(double p_value) const {
        return p_value >= m_alpha;
    }

public:
    // Virtual destructor
    virtual ~StatisticalTest() = default;
    
    /**
     * @brief Execute the statistical test on bit sequence
     * @param data Input bit sequence
     * @return TestResult containing test results
     */
    virtual TestResult execute(const BitSequence& data) = 0;
    
    /**
     * @brief Execute the statistical test on byte sequence
     * @param data Input byte sequence
     * @return TestResult containing test results
     */
    virtual TestResult execute(const ByteSequence& data) {
        return execute(BitSequence(data));
    }
    
    /**
     * @brief Get the name of the test
     * @return Test name as string
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief Set the significance level (alpha)
     * @param alpha New significance level
     */
    void setAlpha(double alpha) {
        if (alpha <= 0.0 || alpha >= 1.0) {
            throw std::invalid_argument("Alpha must be between 0 and 1");
        }
        m_alpha = alpha;
    }
    
    /**
     * @brief Get current significance level
     * @return Current alpha value
     */
    double getAlpha() const {
        return m_alpha;
    }
};

/**
 * @class TestSuite
 * @brief Container for multiple statistical tests
 * 
 * This class manages a collection of statistical tests and provides
 * methods to run them all on a given data set.
 */
class TestSuite {
private:
    // Collection of statistical tests
    std::vector<std::unique_ptr<StatisticalTest>> m_tests;
    
    // Significance level for all tests
    double m_alpha = StatisticalTest::DEFAULT_ALPHA;

public:
    /**
     * @brief Default constructor
     */
    TestSuite() = default;
    
    /**
     * @brief Add a test to the suite
     * @param test Pointer to test instance (suite takes ownership)
     */
    void addTest(std::unique_ptr<StatisticalTest> test) {
        test->setAlpha(m_alpha);
        m_tests.push_back(std::move(test));
    }
    
    /**
     * @brief Set significance level for all tests
     * @param alpha New significance level
     */
    void setAlpha(double alpha) {
        if (alpha <= 0.0 || alpha >= 1.0) {
            throw std::invalid_argument("Alpha must be between 0 and 1");
        }
        m_alpha = alpha;
        
        // Update all tests
        for (auto& test : m_tests) {
            test->setAlpha(alpha);
        }
    }
    
    /**
     * @brief Run all tests on bit sequence
     * @param data Input bit sequence
     * @return Vector of test results
     */
    std::vector<TestResult> runTests(const BitSequence& data) {
        std::vector<TestResult> results;
        results.reserve(m_tests.size());
        
        for (auto& test : m_tests) {
            results.push_back(test->execute(data));
        }
        
        return results;
    }
    
    /**
     * @brief Run all tests on byte sequence
     * @param data Input byte sequence
     * @return Vector of test results
     */
    std::vector<TestResult> runTests(const ByteSequence& data) {
        std::vector<TestResult> results;
        results.reserve(m_tests.size());
        
        for (auto& test : m_tests) {
            results.push_back(test->execute(data));
        }
        
        return results;
    }
    
    /**
     * @brief Get all tests in the suite
     * @return Reference to the vector of tests
     */
    const std::vector<std::unique_ptr<StatisticalTest>>& getTests() const {
        return m_tests;
    }
    
    /**
     * @brief Clear all tests from the suite
     */
    void clear() {
        m_tests.clear();
    }
};

} // namespace caca::tests