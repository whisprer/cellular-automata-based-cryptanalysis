#pragma once

#include "core_config.hpp"
#include "error_handling.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace caca::tests {
    // Base class for statistical tests
    class StatisticalTest {
    public:
        // Virtual destructor for proper inheritance
        virtual ~StatisticalTest() = default;

        // Pure virtual method to execute the test
        virtual bool execute(const ByteSequence& data) = 0;

        // Get the name of the test
        virtual std::string getName() const = 0;

        // Optional method for additional test configuration
        virtual void configure(const std::vector<double>& params) {}
    };

    // Test result structure
    struct TestResult {
        bool passed;
        double p_value;
        std::string test_name;
        std::vector<double> additional_metrics;
    };

    // Statistical test suite management
    class TestSuite {
    private:
        std::vector<std::unique_ptr<StatisticalTest>> tests;

    public:
        // Add a test to the suite
        void addTest(std::unique_ptr<StatisticalTest> test) {
            tests.push_back(std::move(test));
        }

        // Run all tests on a given data sequence
        std::vector<TestResult> runTests(const ByteSequence& data) {
            std::vector<TestResult> results;

            for (const auto& test : tests) {
                TestResult result;
                result.test_name = test->getName();
                
                try {
                    result.passed = test->execute(data);
                } catch (const std::exception& e) {
                    error::ErrorLogger::log(e, "Test Execution");
                    result.passed = false;
                }

                results.push_back(result);
            }

            return results;
        }

        // Clear all tests from the suite
        void clear() {
            tests.clear();
        }
    };

    // Utility functions for statistical computations
    namespace statistical_utils {
        // Error function (erf) implementation
        double erf(double x) {
            // Abramowitz and Stegun approximation
            const double a1 =  0.254829592;
            const double a2 = -0.284496736;
            const double a3 =  1.421413741;
            const double a4 = -1.453152027;
            const double a5 =  1.061405429;
            const double p  =  0.3275911;

            // Save the sign of x
            int sign = (x < 0) ? -1 : 1;
            x = std::abs(x);

            // A&S formula
            double t = 1.0 / (1.0 + p * x);
            double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);

            return sign * y;
        }

        // Complementary error function
        double erfc(double x) {
            return 1.0 - erf(x);
        }
    }
}