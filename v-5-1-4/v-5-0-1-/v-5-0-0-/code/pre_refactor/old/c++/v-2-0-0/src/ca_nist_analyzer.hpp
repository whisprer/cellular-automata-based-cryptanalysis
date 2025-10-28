// ca_nist_analyzer.hpp
#pragma once
#include "nist_tests.hpp"
#include <vector>
#include <string>
#include <memory>
#include <bitset>

// Forward declaration of your existing classes
class CellularAutomataProcessor;

// Class to integrate NIST tests with CA analysis
class CANISTAnalyzer {
private:
    std::unique_ptr<CellularAutomataProcessor> caProcessor;
    std::unique_ptr<nist_sts::NISTTestSuite> nistTester;
    std::string inputFilename;
    int iterations;
    
public:
    CANISTAnalyzer(const std::string& filename, int iters = 5) 
        : inputFilename(filename), iterations(iters) {
        nistTester = std::make_unique<nist_sts::NISTTestSuite>();
        
        // Load data and initialize CA processor
        std::vector<uint8_t> data = loadFile(filename);
        caProcessor = std::make_unique<CellularAutomataProcessor>(data.size());
        caProcessor->initializeFromCiphertext(data);
    }
    
    void runAnalysis() {
        // Run CA iterations
        std::cout << "Running " << iterations << " iterations of cellular automata...\n";
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            caProcessor->updateAllRules();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        std::cout << "Completed " << iterations << " iteration(s) for all 4 rules in " 
                  << elapsed << " sec.\n";
        
        // Extract results
        auto data30 = caProcessor->extractDataRule30();
        auto data82 = caProcessor->extractDataRule82();
        auto data110 = caProcessor->extractDataRule110();
        auto data150 = caProcessor->extractDataRule150();
        
        // Run NIST tests on each dataset
        runNISTAnalysis("Rule 30", data30);
        runNISTAnalysis("Rule 82", data82);
        runNISTAnalysis("Rule 110", data110);
        runNISTAnalysis("Rule 150", data150);
        
        // Compare results
        compareRules(data30, data82, data110, data150);
    }
    
private:
    std::vector<uint8_t> loadFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Error: Cannot open file " + filename);
        }
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), {});
    }
    
    void runNISTAnalysis(const std::string& label, const std::vector<uint8_t>& data) {
        std::cout << "\n=== NIST Tests for " << label << " ===\n";
        
        // Run all NIST tests
        auto results = nistTester->runTests(data);
        
        // Display results
        std::cout << "Test Name                         P-Value    Result\n";
        std::cout << "---------------------------------------------------\n";
        
        for (const auto& result : results) {
            std::cout << std::left << std::setw(35) << result.testName.substr(0, 34) 
                      << std::fixed << std::setprecision(6) << std::setw(10) << result.p_value
                      << (result.success ? "SUCCESS" : "FAILURE") << "\n";
        }
    }
    
    void compareRules(const std::vector<uint8_t>& data30,
                     const std::vector<uint8_t>& data82,
                     const std::vector<uint8_t>& data110,
                     const std::vector<uint8_t>& data150) {
        // Compare randomness p-values across rules
        std::cout << "\n=== NIST Test Comparison Across Rules ===\n";
        std::cout << "Test Name                   Rule 30   Rule 82   Rule 110  Rule 150\n";
        std::cout << "---------------------------------------------------------------------\n";
        
        auto p30 = nistTester->getPValues(data30);
        auto p82 = nistTester->getPValues(data82);
        auto p110 = nistTester->getPValues(data110);
        auto p150 = nistTester->getPValues(data150);
        
        // Display comparison
        for (const auto& [name, val] : p30) {
            std::cout << std::left << std::setw(25) << name.substr(0, 24) 
                      << std::fixed << std::setprecision(6) 
                      << std::setw(10) << p30[name]
                      << std::setw(10) << p82[name]
                      << std::setw(10) << p110[name]
                      << std::setw(10) << p150[name]
                      << "\n";
        }
    }
};