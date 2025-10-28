// ca_analyzer.hpp - Cellular Automata Analyzer
#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <chrono>
#include "nist_sts.hpp"

// Forward declarations
class CellularAutomataProcessor;
class StatAnalyzer;

// Class to handle loading data
class DataLoader {
public:
    static std::vector<uint8_t> loadFile(const std::string& filename);
    static void saveFile(const std::string& filename, const std::vector<uint8_t>& data);
};

// Main analyzer class that coordinates the analysis
class CAAnalyzer {
private:
    std::unique_ptr<CellularAutomataProcessor> caProcessor;
    std::unique_ptr<nist_sts::NISTTestSuite> nistTester;
    std::string inputFilename;
    int iterations;
    
public:
    CAAnalyzer(const std::string& filename, int iters = 5);
    ~CAAnalyzer();
    
    // Run the full analysis
    void runAnalysis();
    
private:
    // Run basic statistical tests
    void runBasicStats(const std::string& label, const std::vector<uint8_t>& data);
    
    // Run NIST tests
    void runNISTTests(const std::string& label, const std::vector<uint8_t>& data);
    
    // Compare results from different rules
    void compareRules(const std::vector<std::vector<uint8_t>>& allRuleData);
};