#include "core/core_config.hpp"
#include "core/error_handling.hpp"
#include "tests/nist_tests.hpp"
#include "tests/statistical_tests.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <argparse/argparse.hpp>

namespace fs = std::filesystem;

// Main CACA application class
class CACApplication {
private:
    // Test suite for running NIST statistical tests
    caca::tests::TestSuite test_suite;

    // Configuration for the application
    caca::CACAConfig& config;

    // Logging and error handling
    caca::error::ErrorLogger logger;

    // Load input file into byte sequence
    caca::ByteSequence load_input_file(const fs::path& input_path) {
        try {
            // Open file in binary mode
            std::ifstream file(input_path, std::ios::binary);
            
            if (!file) {
                CACA_THROW(caca::error::IOError, 
                    "Unable to open input file: " + input_path.string());
            }

            // Read file contents
            return caca::ByteSequence(
                (std::istreambuf_iterator<char>(file)), 
                std::istreambuf_iterator<char>()
            );
        } catch (const std::exception& e) {
            logger.log(e, "File Loading");
            throw;
        }
    }

    // Configure test suite based on user preferences
    void setup_test_suite() {
        // Add default NIST tests
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

public:
    // Constructor
    CACApplication() : 
        config(caca::CACAConfig::getInstance()) {
        // Initialize application with default configuration
        setup_test_suite();
    }

    // Main application logic
    int run(int argc, char* argv[]) {
        // Set up argument parsing
        argparse::ArgumentParser program("caca", "1.0");

        program.add_argument("-f", "--file")
            .help("Input file to analyze")
            .required();

        program.add_argument("-o", "--output")
            .help("Output file for results")
            .default_value("caca_results.txt");

        program.add_argument("-v", "--verbose")
            .help("Enable verbose logging")
            .default_value(false)
            .implicit_value(true);

        try {
            // Parse arguments
            program.parse_args(argc, argv);

            // Set verbosity
            config.verbose_logging = 
                program.get<bool>("verbose");

            // Get input and output paths
            fs::path input_path = program.get<std::string>("file");
            fs::path output_path = program.get<std::string>("output");

            // Load input data
            auto input_data = load_input_file(input_path);

            // Run statistical tests
            auto test_results = test_suite.runTests(input_data);

            // Write results to output file
            std::ofstream results_file(output_path);
            
            results_file << "CACA Statistical Test Results\n";
            results_file << "===========================\n";
            
            for (const auto& result : test_results) {
                results_file << "Test: " << result.test_name 
                             << " | Result: " 
                             << (result.passed ? "PASS" : "FAIL") 
                             << "\n";
            }

            // Optional verbose logging
            if (config.verbose_logging) {
                logger.log("Analysis complete. Results written to " + 
                           output_path.string());
            }

            return 0;

        } catch (const std::exception& e) {
            logger.log(e);
            return 1;
        }
    }
};

// Main entry point
int main(int argc, char* argv[]) {
    try {
        CACApplication app;
        return app.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
}