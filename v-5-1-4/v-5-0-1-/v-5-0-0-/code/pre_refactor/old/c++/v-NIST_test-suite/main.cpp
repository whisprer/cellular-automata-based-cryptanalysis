// Update main.cpp to include generator testing options
#include "nist_sts/test_suite.hpp"
#include "nist_sts/generator_factory.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "ca_analyzer.hpp"
#include <iostream>
#include <stdexcept>

using namespace nist_sts;

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n\n"
              << "File Input Options:\n"
              << "  -f <input_file>  Input file to test\n"
              << "  -a               ASCII mode (default is binary)\n"
              << "  -o <output_file> Output file (default is stdout)\n"
              << "  -l <length>      Length of bit sequence to test\n\n"
              << "Generator Options:\n"
              << "  -g <generator>   Test a specific generator\n"
              << "  -G               Test all available generators\n"
              << "  -L <length>      Length of sequence to generate (default: 1000000)\n\n"
              << "Other Options:\n"
              << "  -h               Show this help message\n"
              << "  -list            List available generators\n";
}

int main(int argc, char** argv) {
    int main(int argc, char* argv[]) {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <filename> [iterations]\n";
            return 1;
        }
        
        std::string filename = argv[1];
        int iterations = 5; // default
        
        if (argc >= 3) {
            iterations = std::stoi(argv[2]);
        }
        
        try {
            // Create and run the analyzer
            CAAnalyzer analyzer(filename, iterations);
            analyzer.runAnalysis();
            
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1;
        }
        
        return 0;
    }

    std::string inputFile;
    std::string outputFile;
    std::string generatorName;
    bool isAscii = false;
    bool testAllGenerators = false;
    bool listGenerators = false;
    long length = 1000000;  // Default length
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-a") {
            isAscii = true;
        } else if (arg == "-f" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "-l" || arg == "-L" && i + 1 < argc) {
            length = std::stol(argv[++i]);
        } else if (arg == "-g" && i + 1 < argc) {
            generatorName = argv[++i];
        } else if (arg == "-G") {
            testAllGenerators = true;
        } else if (arg == "-list") {
            listGenerators = true;
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (listGenerators) {
        std::cout << "Available Random Number Generators:\n";
        for (const auto& name : GeneratorFactory::getAvailableGenerators()) {
            std::cout << "  - " << name << "\n";
        }
        return 0;
    }
    
    try {
        std::cout << "NIST Statistical Test Suite for Random Number Generators\n";
        std::cout << "----------------------------------------------------\n";
        
        // Create the test suite
        TestSuite suite = TestSuite::createDefaultSuite();
        
        // Set parameters if needed
        TestParameters params = suite.getParameters();
        params.n = length;
        suite.setParameters(params);
        
        TestResults results;
        std::map<std::string, TestResults> allResults;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        if (!inputFile.empty()) {
            std::cout << "Testing file: " << inputFile << " (" 
                      << (isAscii ? "ASCII" : "binary") << " mode)\n";
            results = suite.runTests(inputFile, isAscii);
        } else if (!generatorName.empty()) {
            std::cout << "Testing generator: " << generatorName << "\n";
            auto generator = GeneratorFactory::createGenerator(generatorName);
            results = suite.testGenerator(*generator, length);
        } else if (testAllGenerators) {
            std::cout << "Testing all available generators\n";
            allResults = suite.testAllGenerators(length);
        } else {
            std::cerr << "Error: No input file or generator specified.\n";
            printUsage(argv[0]);
            return 1;
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        int main(int argc, char* argv[]) {
            if (argc < 2) {
                std::cerr << "Usage: " << argv[0] << " <filename> [iterations]\n";
                return 1;
            }
            
            std::string filename = argv[1];
            int iterations = 5; // default
            
            if (argc >= 3) {
                iterations = std::stoi(argv[2]);
            }
            
            try {
                // Create and run the analyzer
                CAAnalyzer analyzer(filename, iterations);
                analyzer.runAnalysis();
                
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
                return 1;
            }
            
            return 0;
        }        // Output results

        if (testAllGenerators) {
            for (const auto& [name, result] : allResults) {
                std::cout << "\nResults for " << name << ":\n";
                std::string summary = suite.generateSummary(result);
                
                if (outputFile.empty()) {
                    std::cout << summary;
                } else {
                    std::string genOutputFile = name + "_" + outputFile;
                    suite.saveResults(result, genOutputFile);
                    std::cout << "Results saved to " << genOutputFile << std::endl;
                }
            }
        } else {
            std::string summary = suite.generateSummary(results);
            
            if (outputFile.empty()) {
                std::cout << summary;
            } else {
                suite.saveResults(results, outputFile);
                std::cout << "Results saved to " << outputFile << std::endl;
            }
        }
        
        std::cout << "Testing completed in " << duration.count() / 1000.0 
                  << " seconds." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}