// caca_test.cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "ca_analyzer.hpp"
#include "nist_sts.hpp"
#include "stat_analyzer.hpp"

// Helper function to load binary data
std::vector<uint8_t> loadData(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    return std::vector<uint8_t>(
        (std::istreambuf_iterator<char>(file)), 
        std::istreambuf_iterator<char>()
    );
}

// Helper function to save results to a simple CSV file
void saveResults(const std::string& filename, 
                 const std::map<std::string, std::map<std::string, double>>& results) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }
    
    // Write header
    file << "Dataset,";
    for (const auto& [dataset, tests] : results) {
        for (const auto& [test, value] : tests) {
            file << test << ",";
            break; // Just to get test names from first dataset
        }
        break;
    }
    file << "\n";
    
    // Write data
    for (const auto& [dataset, tests] : results) {
        file << dataset << ",";
        for (const auto& [test, value] : tests) {
            file << value << ",";
        }
        file << "\n";
    }
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <encrypted_file> [output_prefix]\n";
            return 1;
        }
        
        std::string inputFile = argv[1];
        std::string outputPrefix = (argc > 2) ? argv[2] : "caca_results";
        
        // Load encrypted data
        std::cout << "Loading data from " << inputFile << "...\n";
        auto encryptedData = loadData(inputFile);
        std::cout << "Loaded " << encryptedData.size() << " bytes\n";
        
        // Create NIST test suite
        nist_sts::NISTTestSuite nistTests;
        
        // Run NIST tests on original data
        std::cout << "Running baseline NIST tests on original data...\n";
        auto baselineResults = nistTests.runTests(encryptedData);
        
        // Display baseline statistics
        std::cout << "\nBaseline statistics:\n";
        std::cout << "Index of Coincidence: " << StatAnalyzer::indexOfCoincidence(encryptedData) << "\n";
        std::cout << "Chi-Square: " << StatAnalyzer::chiSquare(encryptedData) << "\n";
        std::cout << "Serial Correlation: " << StatAnalyzer::serialCorrelation(encryptedData) << "\n";
        
        // Create results container
        std::map<std::string, std::map<std::string, double>> allResults;
        
        // Store baseline p-values
        std::map<std::string, double> baselinePValues;
        for (const auto& result : baselineResults) {
            baselinePValues[result.testName] = result.p_value;
        }
        allResults["Original"] = baselinePValues;
        
        // Define CA rules to test
        std::vector<int> rules = {30, 82, 110, 150};
        
        // Run CA with different rules and iterations
        for (int rule : rules) {
            for (int iterations : {1, 3, 5, 10}) {
                std::cout << "\nApplying CA Rule " << rule << " with " << iterations << " iterations...\n";
                
                // Initialize CA processor
                CellularAutomataProcessor ca(encryptedData.size(), rule);
                ca.initializeFromCiphertext(encryptedData);
                
                // Apply CA iterations
                for (int i = 0; i < iterations; i++) {
                    ca.updateCA_SIMD();
                }
                
                // Extract processed data
                auto processedData = ca.extractProcessedData();
                
                // Run NIST tests on processed data
                auto processedResults = nistTests.runTests(processedData);
                
                // Store p-values
                std::map<std::string, double> processedPValues;
                for (const auto& result : processedResults) {
                    processedPValues[result.testName] = result.p_value;
                }
                
                // Add statistics
                processedPValues["IndexOfCoincidence"] = StatAnalyzer::indexOfCoincidence(processedData);
                processedPValues["ChiSquare"] = StatAnalyzer::chiSquare(processedData);
                processedPValues["SerialCorrelation"] = StatAnalyzer::serialCorrelation(processedData);
                
                // Add to results
                std::string datasetName = "Rule" + std::to_string(rule) + "_Iter" + std::to_string(iterations);
                allResults[datasetName] = processedPValues;
                
                // Display key statistics
                std::cout << "Post-CA statistics:\n";
                std::cout << "Index of Coincidence: " << StatAnalyzer::indexOfCoincidence(processedData) << "\n";
                std::cout << "Chi-Square: " << StatAnalyzer::chiSquare(processedData) << "\n";
                std::cout << "Serial Correlation: " << StatAnalyzer::serialCorrelation(processedData) << "\n";
                
                // Save processed data
                std::string outputFile = outputPrefix + "_rule" + std::to_string(rule) + 
                                        "_iter" + std::to_string(iterations) + ".bin";
                std::ofstream outFile(outputFile, std::ios::binary);
                outFile.write(reinterpret_cast<const char*>(processedData.data()), processedData.size());
                std::cout << "Saved processed data to " << outputFile << "\n";
            }
        }
        
        // Save results to CSV
        std::string resultsFile = outputPrefix + "_results.csv";
        saveResults(resultsFile, allResults);
        std::cout << "\nSaved analysis results to " << resultsFile << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}