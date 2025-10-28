// File: src/main.cpp
#include "caca/core/bitsequence.hpp"
#include "caca/ca/cellular_automata.hpp"
#include "caca/tests/nist_tests.hpp"
#include "caca/simd/cpu_features.hpp"
#include "caca/monitoring/progress_monitor.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <filesystem>
#include <stdexcept>

// Command line parsing library
#include <argparse/argparse.hpp>

namespace fs = std::filesystem;

/**
 * @brief Main application class for CACA
 */
class CACAApplication {
private:
    // Configuration
    std::string m_input_file;
    std::string m_output_prefix;
    bool m_ascii_mode = false;
    bool m_verbose = false;
    size_t m_iterations = 5;
    std::vector<caca::ca::CARule> m_rules = {
        caca::ca::CARule::Rule30,
        caca::ca::CARule::Rule82,
        caca::ca::CARule::Rule110,
        caca::ca::CARule::Rule150
    };
    
    // Load input file
    caca::ByteSequence loadInputFile() {
        std::ifstream file(m_input_file, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open input file: " + m_input_file);
        }
        
        // Read file content
        return caca::ByteSequence(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }
    
    // Write results to output file
    void writeResults(const std::string& suffix, const std::vector<caca::tests::TestResult>& results) {
        std::string filename = m_output_prefix + "_" + suffix + ".txt";
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Warning: Could not open output file: " << filename << std::endl;
            return;
        }
        
        file << "CACA Analysis Results - " << suffix << std::endl;
        file << "====================================" << std::endl;
        file << std::endl;
        
        for (const auto& result : results) {
            file << result.test_name << ":" << std::endl;
            file << "  P-value: " << result.p_value << std::endl;
            file << "  Result: " << (result.passed ? "PASS" : "FAIL") << std::endl;
            
            // Additional metrics
            if (!result.additional_metrics.empty()) {
                file << "  Additional metrics:" << std::endl;
                for (const auto& [key, value] : result.additional_metrics) {
                    file << "    " << key << ": " << value << std::endl;
                }
            }
            
            file << std::endl;
        }
        
        if (m_verbose) {
            std::cout << "Results written to " << filename << std::endl;
        }
    }
    
    // Setup NIST test suite
    std::unique_ptr<caca::tests::TestSuite> setupTestSuite() {
        auto suite = std::make_unique<caca::tests::TestSuite>();
        
        // Add tests
        suite->addTest(std::make_unique<caca::tests::nist::FrequencyTest>());
        suite->addTest(std::make_unique<caca::tests::nist::BlockFrequencyTest>());
        suite->addTest(std::make_unique<caca::tests::nist::RunsTest>());
        suite->addTest(std::make_unique<caca::tests::nist::LongestRunTest>());
        suite->addTest(std::make_unique<caca::tests::nist::DFTTest>());
        suite->addTest(std::make_unique<caca::tests::nist::NonOverlappingTemplateTest>());
        suite->addTest(std::make_unique<caca::tests::nist::OverlappingTemplateTest>());
        
        return suite;
    }
    
    // Process data with cellular automata
    caca::ByteSequence processWithCA(
        const caca::ByteSequence& data,
        caca::ca::CARule rule,
        size_t iterations
    ) {
        if (m_verbose) {
            std::cout << "Processing with " << static_cast<int>(rule) 
                      << " for " << iterations << " iterations..." << std::endl;
        }
        
        // Create CA processor
        caca::ca::CellularAutomataProcessor processor(
            data,
            rule,
            caca::ca::CANeighborhood::Moore
        );
        
        // Process data
        return processor.processBytes(iterations);
    }
    
    // Run all tests on data
    std::vector<caca::tests::TestResult> runTests(const caca::ByteSequence& data) {
        if (m_verbose) {
            std::cout << "Running NIST statistical tests..." << std::endl;
        }
        
        auto suite = setupTestSuite();
        return suite->runTests(data);
    }

public:
    // Parse command line arguments
    bool parseArgs(int argc, char* argv[]) {
        try {
            argparse::ArgumentParser program("caca_app", "1.0");
            
            program.add_argument("input")
                .help("Input file to analyze");
                
            program.add_argument("-o", "--output")
                .help("Output file prefix")
                .default_value(std::string("caca_results"));
                
            program.add_argument("-a", "--ascii")
                .help("Treat input as ASCII (default: binary)")
                .default_value(false)
                .implicit_value(true);
                
            program.add_argument("-i", "--iterations")
                .help("Number of CA iterations")
                .default_value(5)
                .scan<'i', int>();
                
            program.add_argument("-r", "--ca-rules")
                .help("Comma-separated CA rules (default: 30,82,110,150)")
                .default_value(std::string("30,82,110,150"));
                
            program.add_argument("-v", "--verbose")
                .help("Verbose output")
                .default_value(false)
                .implicit_value(true);
            
            program.parse_args(argc, argv);
            
            // Get arguments
            m_input_file = program.get<std::string>("input");
            m_output_prefix = program.get<std::string>("--output");
            m_ascii_mode = program.get<bool>("--ascii");
            m_verbose = program.get<bool>("--verbose");
            m_iterations = program.get<int>("--iterations");
            
            // Parse CA rules
            std::string rules_str = program.get<std::string>("--ca-rules");
            m_rules.clear();
            
            size_t pos = 0;
            while ((pos = rules_str.find(',')) != std::string::npos) {
                std::string token = rules_str.substr(0, pos);
                m_rules.push_back(static_cast<caca::ca::CARule>(std::stoi(token)));
                rules_str.erase(0, pos + 1);
            }
            
            if (!rules_str.empty()) {
                m_rules.push_back(static_cast<caca::ca::CARule>(std::stoi(rules_str)));
            }
            
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing arguments: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Main execution method
    int run() {
        try {
            // Print CPU features if verbose
            if (m_verbose) {
                auto features = caca::simd::CPUFeatures::detect();
                std::cout << "CPU Features:" << std::endl;
                features->printFeatureInfo();
                std::cout << std::endl;
            }
            
            // Start progress monitor
            caca::monitoring::ProgressMonitor monitor;
            
            // Load input file
            if (m_verbose) {
                std::cout << "Loading input file: " << m_input_file << std::endl;
            }
            
            caca::ByteSequence input_data;
            try {
                input_data = loadInputFile();
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading input file: " << e.what() << std::endl;
                return 1;
            }
            
            // Run tests on raw data
            if (m_verbose) {
                std::cout << "Analyzing raw input data..." << std::endl;
            }
            
            monitor.update_progress("Raw Data Analysis", 0, 1);
            auto raw_results = runTests(input_data);
            monitor.update_progress("Raw Data Analysis", 1, 1);
            
            writeResults("raw", raw_results);
            
            // Process with each CA rule
            for (size_t i = 0; i < m_rules.size(); ++i) {
                auto rule = m_rules[i];
                std::string rule_name = "Rule" + std::to_string(static_cast<int>(rule));
                
                if (m_verbose) {
                    std::cout << "Processing with " << rule_name << "..." << std::endl;
                }
                
                monitor.update_progress(rule_name, 0, m_iterations);
                
                // Process data with increasing iterations
                for (size_t iter = 1; iter <= m_iterations; ++iter) {
                    std::string iter_suffix = rule_name + "_iter" + std::to_string(iter);
                    
                    // Process data
                    auto processed_data = processWithCA(input_data, rule, iter);
                    
                    // Run tests
                    auto results = runTests(processed_data);
                    
                    // Write results
                    writeResults(iter_suffix, results);
                    
                    // Update progress
                    monitor.update_progress(rule_name, iter, m_iterations);
                }
            }
            
            if (m_verbose) {
                std::cout << "Analysis complete!" << std::endl;
            }
            
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
};

int main(int argc, char* argv[]) {
    CACAApplication app;
    
    if (!app.parseArgs(argc, argv)) {
        return 1;
    }
    
    return app.run();
}