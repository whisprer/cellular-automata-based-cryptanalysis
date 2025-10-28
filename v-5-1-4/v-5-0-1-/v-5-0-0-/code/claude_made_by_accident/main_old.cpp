#include "core/core_config.hpp"
#include "core/error_handling.hpp"
#include "tests/nist_tests.hpp"
#include "tests/statistical_tests.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <argparse/argparse.hpp>

namespace fs = std::filesystem;

// Main CACA application class
class CACApplication {
private:
    // Test suite for running statistical tests
    caca::tests::TestSuite test_suite;

    // Configuration and logging
    caca::CACAConfig& config;
    std::ofstream log_file;

    // Private method to read input file
    caca::ByteSequence read_input_file(const fs::path& input_path) {
        if (!fs::exists(input_path)) {
            CACA_THROW(caca::error::IOError, 
                "Input file does not exist: " + input_path.string());
        }

        std::ifstream file(input_path, std::ios::binary);
        if (!file) {
            CACA_THROW(caca::error::IOError, 
                "Cannot open input file: " + input_path.string());
        }

        // Read file contents
        return caca::ByteSequence(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }

    // Private method to setup default NIST tests
    void setup_default_tests() {
        test_suite.addTest(
            std::make_unique<caca::tests::nist::FrequencyTest>()
        );
        test_suite.addTest(
            std::make_unique<caca::tests::nist::RunsTest>()
        );
        test_suite.addTest(
            std::make_unique<caca::tests::nist::BlockFrequencyTest>()
        );
    }

    // Logging method
    void log_results(const std::vector<caca::tests::TestResult>& results) {
        if (log_file.is_open()) {
            log_file << "=== Test Results ===\n";
            for (const auto& result : results) {
                log_file << "Test: " << result.test_name 
                         << " | Passed: " << (result.passed ? "Yes" : "No") 
                         << " | P-value: " << result.p_value << "\n";
            }
        }
    }

public:
    // Constructor
    CACApplication() 
        : config(caca::CACAConfig::getInstance()),
          log_file("caca_analysis.log", std::ios::app) {
        // Setup default configuration
        config.verbose_logging = true;
        
        // Setup default tests
        setup_default_tests();
    }

    // Main analysis method
    int analyze_file(const fs::path& input_path) {
        try {
            // Read input file
            auto input_data = read_input_file(input_path);

            // Start timing
            auto start_time = std::chrono::high_resolution_clock::now();

            // Run statistical tests
            auto results = test_suite.runTests(input_data);

            // End timing
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end_time - start_time
            );

            // Log results
            log_results(results);

            // Print summary to console
            std::cout << "Analysis complete for: " << input_path << "\n";
            std::cout << "Total test duration: " << duration.count() << " ms\n";

            // Analyze overall test results
            size_t passed_tests = std::count_if(
                results.begin(), results.end(), 
                [](const auto& result) { return result.passed; }
            );

            std::cout << "Tests passed: " << passed_tests 
                      << "/" << results.size() << "\n";

            return passed_tests == results.size() ? 0 : 1;

        } catch (const caca::error::CACAException& e) {
            caca::error::ErrorLogger::log(e, "File Analysis");
            return -1;
        }
    }

    // Main entry point
    static int run(int argc, char* argv[]) {
        // Setup argument parser
        argparse::ArgumentParser program("caca", "1.0");

        program.add_argument("input")
            .help("Input file to analyze")
            .required();

        program.add_argument("-v", "--verbose")
            .help("Enable verbose logging")
            .default_value(false)
            .implicit_value(true);

        try {
            program.parse_args(argc, argv);
        } catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << program;
            return 1;
        }

        // Create application instance
        CACApplication app;

        // Set verbosity if specified
        app.config.verbose_logging = 
            program.get<bool>("verbose");

        // Get input file path
        fs::path input_path = program.get<std::string>("input");

        // Run analysis
        return app.analyze_file(input_path);
    }
};

// Actual main function
int main(int argc, char* argv[]) {
    try {
        return CACApplication::run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return -1;
    }
}