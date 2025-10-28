// ca_analyzer.cpp - Implementation of the CA Analyzer
#include "ca_analyzer.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <cmath>
#include <stdexcept>

// ---------------------- DataLoader Implementation ----------------------
std::vector<uint8_t> DataLoader::loadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Cannot open file " + filename);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), {});
}

void DataLoader::saveFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Cannot create file " + filename);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// ---------------------- CAAnalyzer Implementation ----------------------
CAAnalyzer::CAAnalyzer(const std::string& filename, int iters) 
    : inputFilename(filename), iterations(iters) {
    // Load data
    auto data = DataLoader::loadFile(filename);
    
    // Create processor
    caProcessor = std::make_unique<CellularAutomataProcessor>(data.size());
    caProcessor->initializeFromCiphertext(data);
    
    // Create NIST tester
    nistTester = std::make_unique<nist_sts::NISTTestSuite>();
}

CAAnalyzer::~CAAnalyzer() = default;

void CAAnalyzer::runAnalysis() {
    // Execute CA iterations
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
    
    // Run basic statistical tests
    runBasicStats("Rule 30", data30);
    runBasicStats("Rule 82", data82);
    runBasicStats("Rule 110", data110);
    runBasicStats("Rule 150", data150);
    
    // Run NIST tests
    runNISTTests("Rule 30", data30);
    runNISTTests("Rule 82", data82);
    runNISTTests("Rule 110", data110);
    runNISTTests("Rule 150", data150);
    
    // Compare results
    std::vector<std::vector<uint8_t>> allData = {data30, data82, data110, data150};
    compareRules(allData);
}

void CAAnalyzer::runBasicStats(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << "\n\n=== Basic Stats for " << label << " ===\n";
    StatAnalyzer::displayStats(label, data);
}

void CAAnalyzer::runNISTTests(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << "\n\n=== NIST Test Results for " << label << " ===\n";
    
    // Run tests and get p-values
    auto results = nistTester->runTests(data);
    
    // Display the results
    std::cout << "Test Name                      P-Value    Result\n";
    std::cout << "------------------------------------------------\n";
    for (const auto& [testName, pValue] : results) {
        std::cout << std::left << std::setw(30) << testName
                  << std::setw(10) << std::fixed << std::setprecision(6) << pValue
                  << (pValue >= 0.01 ? "SUCCESS" : "FAILURE") << "\n";
    }
    
    std::cout << "================================================\n";
}

void CAAnalyzer::compareRules(const std::vector<std::vector<uint8_t>>& allRuleData) {
    std::cout << "\n\n=== Comparison Between Rules ===\n";
    
    // For each pair of rules, calculate correlation and similarity
    const std::string ruleNames[] = {"Rule 30", "Rule 82", "Rule 110", "Rule 150"};
    
    for (size_t i = 0; i < allRuleData.size(); i++) {
        for (size_t j = i + 1; j < allRuleData.size(); j++) {
            const auto& data1 = allRuleData[i];
            const auto& data2 = allRuleData[j];
            
            // Calculate correlation
            double correlation = StatAnalyzer::correlationBetweenDatasets(data1, data2);
            
            // Calculate Hamming distance (bit differences)
            double hammingDistance = StatAnalyzer::hammingDistanceBetweenDatasets(data1, data2);
            
            std::cout << ruleNames[i] << " vs " << ruleNames[j] << ":\n";
            std::cout << "  Correlation: " << correlation << "\n";
            std::cout << "  Normalized Hamming Distance: " << hammingDistance << "\n\n";
        }
    }
    
    std::cout << "=====================================\n";
}