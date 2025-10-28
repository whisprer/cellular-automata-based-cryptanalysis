// main.cpp
#include "ca_analyzer.hpp"
#include "nist_sts.hpp"
#include "stat_analyzer.hpp"
#include "nist_sts/generator_factory.hpp"
#include "test_suite.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <string>

// Configuration constants
const int DEFAULT_ITERATIONS = 5;
const std::vector<int> DEFAULT_CA_RULES = {30, 82, 110, 150};

// Struct to hold command-line options
struct CACACLIOptions {
    std::string inputFile;
    std::string outputFile;
    std::string generatorName;
    std::vector<int> caRules = DEFAULT_CA_RULES;
    int iterations = DEFAULT_ITERATIONS;
    bool testAllGenerators = false;
    bool listGenerators = false;
    bool asciiMode = false;
    long sequenceLength = 1000000;
    bool verbose = false;
};

// Function prototypes
void printUsage(const char* programName);
void printHelp(const char* programName);
CACACLIOptions parseCommandLineOptions(int argc, char* argv[]);
std::vector<uint8_t> loadCipherText(const std::string& filename, bool isAscii = false);
void performCellularAutomataAnalysis(const std::vector<uint8_t>& cipherData, 
                                     const CACACLIOptions& options);
void performGeneratorAnalysis(const CACACLIOptions& options);

// Custom command-line parsing function
CACACLIOptions parseCommandLineOptions(int argc, char* argv[]) {
    CACACLIOptions options;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) options.inputFile = argv[++i];
        }
        else if (arg == "-a" || arg == "--ascii") {
            options.asciiMode = true;
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) options.outputFile = argv[++i];
        }
        else if (arg == "-g" || arg == "--generator") {
            if (i + 1 < argc) options.generatorName = argv[++i];
        }
        else if (arg == "-G" || arg == "--all-generators") {
            options.testAllGenerators = true;
        }
        else if (arg == "-l" || arg == "--list") {
            options.listGenerators = true;
        }
        else if (arg == "-i" || arg == "--iterations") {
            if (i + 1 < argc) options.iterations = std::stoi(argv[++i]);
        }
        else if (arg == "-L" || arg == "--length") {
            if (i + 1 < argc) options.sequenceLength = std::stol(argv[++i]);
        }
        else if (arg == "-r" || arg == "--ca-rules") {
            if (i + 1 < argc) {
                // Parse comma-separated rules
                options.caRules.clear();
                std::string ruleStr = argv[++i];
                size_t pos = 0;
                std::string token;
                while ((pos = ruleStr.find(',')) != std::string::npos) {
                    token = ruleStr.substr(0, pos);
                    options.caRules.push_back(std::stoi(token));
                    ruleStr.erase(0, pos + 1);
                }
                // Add last rule
                if (!ruleStr.empty()) {
                    options.caRules.push_back(std::stoi(ruleStr));
                }
            }
        }
        else if (arg == "-v" || arg == "--verbose") {
            options.verbose = true;
        }
        else if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            exit(0);
        }
        else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            exit(1);
        }
    }

    return options;
}

// Helper function to load cipher text
std::vector<uint8_t> loadCipherText(const std::string& filename, bool isAscii) {
    std::ifstream file(filename, isAscii ? std::ios::in : std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    // Read file contents
    std::vector<uint8_t> data;
    
    if (isAscii) {
        // Read ASCII file (only 0 and 1 characters)
        char ch;
        while (file.get(ch)) {
            if (ch == '0' || ch == '1') {
                data.push_back(ch == '1' ? 1 : 0);
            }
        }
    }
    else {
        // Read binary file
        data = std::vector<uint8_t>(
            (std::istreambuf_iterator<char>(file)), 
            std::istreambuf_iterator<char>()
        );
    }
    
    return data;
}

// Perform Cellular Automata Analysis
void performCellularAutomataAnalysis(const std::vector<uint8_t>& cipherData, 
                                     const CACACLIOptions& options) {
    // NIST Statistical Test Suite
    nist_sts::NISTTestSuite nistTester;
    
    // Base analysis of original data
    std::cout << "\n=== Original Data Analysis ===\n";
    std::string originalSummary = nistTester.generateSummary(cipherData);
    std::cout << originalSummary << "\n";
    
    // Cellular Automata processing
    for (int rule : options.caRules) {
        std::cout << "\n--- Cellular Automata Rule " << rule << " ---\n";
        
        // Create CA processor
        CellularAutomataProcessor caProcessor(cipherData.size(), rule);
        caProcessor.initializeFromCiphertext(cipherData);
        
        // Track processing time
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Run iterations
        for (int iter = 0; iter < options.iterations; ++iter) {
            caProcessor.updateCA_SIMD();
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        // Extract processed data
        auto processedData = caProcessor.extractProcessedData();
        
        // Statistical analysis
        std::cout << "Processing Time: " << duration.count() << " ms\n";
        
        // Run NIST tests on processed data
        std::string processedSummary = nistTester.generateSummary(processedData);
        std::cout << processedSummary << "\n";
        
        // Additional statistical checks
        double entropy = StatAnalyzer::indexOfCoincidence(processedData);
        double chiSquare = StatAnalyzer::chiSquare(processedData);
        double serialCorr = StatAnalyzer::serialCorrelation(processedData);
        
        std::cout << "Additional Metrics:\n";
        std::cout << "  Index of Coincidence: " << std::fixed << std::setprecision(4) << entropy << "\n";
        std::cout << "  Chi-Square Statistic: " << chiSquare << "\n";
        std::cout << "  Serial Correlation: " << serialCorr << "\n";
        
        // Optional output file
        if (!options.outputFile.empty()) {
            std::string outputFilename = options.outputFile + "_rule" + std::to_string(rule);
            std::ofstream outFile(outputFilename, std::ios::binary);
            outFile.write(reinterpret_cast<const char*>(processedData.data()), processedData.size());
            std::cout << "Processed data saved to: " << outputFilename << "\n";
        }
    }
}

// Perform Generator Analysis
void performGeneratorAnalysis(const CACACLIOptions& options) {
    // Create test suite
    nist_sts::TestSuite testSuite = nist_sts::TestSuite::createDefaultSuite();
    
    if (options.testAllGenerators) {
        // Test all available generators
        std::cout << "Testing All Available Generators\n";
        auto generators = nist_sts::GeneratorFactory::createAllGenerators();
        
        for (auto& generator : generators) {
            std::cout << "\n=== Generator: " << generator->getName() << " ===\n";
            auto results = testSuite.testGenerator(*generator, options.sequenceLength);
            std::cout << testSuite.generateSummary(results) << "\n";
        }
    }
    else {
        // Test specific generator
        std::cout << "Testing Generator: " << options.generatorName << "\n";
        auto generator = nist_sts::GeneratorFactory::createGenerator(options.generatorName);
        auto results = testSuite.testGenerator(*generator, options.sequenceLength);
        std::cout << testSuite.generateSummary(results) << "\n";
    }
}

// Print usage instructions
void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " [options]\n";
    std::cerr << "Options:\n";
    std::cerr << "  -f, --file <file>        Input file to analyze\n";
    std::cerr << "  -a, --ascii              Treat input as ASCII (default: binary)\n";
    std::cerr << "  -o, --output <file>      Output file prefix\n";
    std::cerr << "  -g, --generator <n>     Test specific random number generator\n";
    std::cerr << "  -G, --all-generators     Test all available generators\n";
    std::cerr << "  -l, --list              List available generators\n";
    std::cerr << "  -i, --iterations <n>    Number of CA iterations (default: 5)\n";
    std::cerr << "  -L, --length <n>        Sequence length for generator tests (default: 1000000)\n";
    std::cerr << "  -r, --ca-rules <r1,r2>  Comma-separated CA rules (default: 30,82,110,150)\n";
    std::cerr << "  -v, --verbose           Verbose output\n";
    std::cerr << "  -h, --help             Show this help message\n";
}

// Print detailed help
void printHelp(const char* programName) {
    printUsage(programName);
    std::cout << "\nDescription:\n";
    std::cout << "CACA (Cellular Automata Cryptanalysis Analysis) is a tool for analyzing\n";
    std::cout << "randomness in encrypted data using Cellular Automata and statistical tests.\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " -f encrypted.bin\n";
    std::cout << "  " << programName << " -f input.txt -a -r 30,110\n";
    std::cout << "  " << programName << " -g 'Linear Congruential' -L 500000\n";
    std::cout << "  " << programName << " -G\n";
}

// Main function
int main(int argc, char* argv[]) {
    try {
        // Parse command-line options
        CACACLIOptions options = parseCommandLineOptions(argc, argv);
        
        // Print banner
        std::cout << "CACA: Cellular Automata Cryptanalysis Analysis Tool\n";
        std::cout << "-----------------------------------------------\n";
        
        // Dispatch based on options
        if (options.listGenerators) {
            // List available generators
            std::cout << "Available Random Number Generators:\n";
            for (const auto& name : nist_sts::GeneratorFactory::getAvailableGenerators()) {
                std::cout << "  - " << name << "\n";
            }
            return 0;
        }
        
        if (!options.generatorName.empty() || options.testAllGenerators) {
            // Generator testing mode
            performGeneratorAnalysis(options);
            return 0;
        }
        
        if (!options.inputFile.empty()) {
            // Load and analyze input file
            std::vector<uint8_t> cipherData = loadCipherText(
                options.inputFile, 
                options.asciiMode
            );
            
            // Print basic file info
            std::cout << "Input File: " << options.inputFile << "\n";
            std::cout << "File Size: " << cipherData.size() << " bytes\n";
            std::cout << "Mode: " << (options.asciiMode ? "ASCII" : "Binary") << "\n";
            
            // Perform Cellular Automata analysis
            performCellularAutomataAnalysis(cipherData, options);
            
            return 0;
        }
        
        // If no specific action, show help
        printHelp(argv[0]);
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}