/*****************************************************
 * File: main.cpp
 * A unified main entry point that parses CLI options,
 * loads data from file, does CA analysis or generator
 * analysis, etc.
 *****************************************************/

 #include <iostream>
 #include <string>
 #include <vector>
 #include <fstream>
 #include <stdexcept>
 #include <algorithm>
 #include <chrono>
 #include <iomanip>
 #include <map>
 #include <cmath>
 
 // Include your local headers
 #include "bitsequence.hpp"
 #include "nist_sts.hpp"
 #include "stat_analyzer.hpp"
 #include "ca_analyzer.hpp"               // For CellularAutomataProcessor
 #include "visualization_generator.hpp"   // For VisualizationGenerator
 #include "generator_factory.hpp"         // For GeneratorFactory
 #include "test_suite.hpp"                // For TestSuite
 
 // ----------------------------------------------------------------------------
 // 1. Define a struct for command-line options
 // ----------------------------------------------------------------------------
 struct CACACLIOptions {
     std::string inputFile;
     std::string outputFile;
     bool asciiMode         = false;
     bool verbose           = false;
     bool listGenerators    = false;
     bool testAllGenerators = false;
     std::string generatorName;
     int iterations         = 5;
     long sequenceLength    = 1000000;
     std::vector<int> caRules{30, 82, 110, 150};
 };
 
 // ----------------------------------------------------------------------------
 // 2. Print usage and help
 // ----------------------------------------------------------------------------
 static void printUsage(const char* progName) {
     std::cerr << "Usage: " << progName << " [options]\n"
               << "  -f, --file <file>        Input file to analyze\n"
               << "  -a, --ascii              Treat input as ASCII (default: binary)\n"
               << "  -o, --output <file>      Output file prefix\n"
               << "  -g, --generator <name>   Test a specific random number generator\n"
               << "  -G, --all-generators     Test all available generators\n"
               << "  -l, --list               List available generators\n"
               << "  -i, --iterations <n>     Number of CA iterations (default: 5)\n"
               << "  -L, --length <n>         Sequence length for generator tests (default: 1000000)\n"
               << "  -r, --ca-rules <r1,r2>   Comma-separated CA rules (default: 30,82,110,150)\n"
               << "  -v, --verbose            Verbose output\n"
               << "  -h, --help               Show this help\n";
 }
 
 static void printHelp(const char* progName) {
     printUsage(progName);
     std::cout << "\nDescription:\n"
               << "CACA (Cellular Automata Cryptanalysis Analysis) is a tool for analyzing\n"
               << "randomness in encrypted data using Cellular Automata and the NIST STS.\n\n"
               << "Examples:\n"
               << "  " << progName << " -f encrypted.bin\n"
               << "  " << progName << " -f input.txt -a -r 30,110\n"
               << "  " << progName << " -g \"Linear Congruential\" -L 500000\n"
               << "  " << progName << " -G\n";
 }
 
 // ----------------------------------------------------------------------------
 // 3. Command-line parser
 // ----------------------------------------------------------------------------
 static CACACLIOptions parseCommandLineOptions(int argc, char* argv[]) {
     CACACLIOptions options;
 
     for (int i = 1; i < argc; ++i) {
         std::string arg = argv[i];
         if (arg == "-f" || arg == "--file") {
             if (i + 1 < argc) options.inputFile = argv[++i];
         } else if (arg == "-a" || arg == "--ascii") {
             options.asciiMode = true;
         } else if (arg == "-o" || arg == "--output") {
             if (i + 1 < argc) options.outputFile = argv[++i];
         } else if (arg == "-g" || arg == "--generator") {
             if (i + 1 < argc) options.generatorName = argv[++i];
         } else if (arg == "-G" || arg == "--all-generators") {
             options.testAllGenerators = true;
         } else if (arg == "-l" || arg == "--list") {
             options.listGenerators = true;
         } else if (arg == "-i" || arg == "--iterations") {
             if (i + 1 < argc) options.iterations = std::stoi(argv[++i]);
         } else if (arg == "-L" || arg == "--length") {
             if (i + 1 < argc) options.sequenceLength = std::stol(argv[++i]);
         } else if (arg == "-r" || arg == "--ca-rules") {
             if (i + 1 < argc) {
                 options.caRules.clear();
                 std::string ruleStr = argv[++i];
                 size_t pos = 0;
                 while ((pos = ruleStr.find(',')) != std::string::npos) {
                     std::string token = ruleStr.substr(0, pos);
                     options.caRules.push_back(std::stoi(token));
                     ruleStr.erase(0, pos + 1);
                 }
                 if (!ruleStr.empty()) {
                     options.caRules.push_back(std::stoi(ruleStr));
                 }
             }
         } else if (arg == "-v" || arg == "--verbose") {
             options.verbose = true;
         } else if (arg == "-h" || arg == "--help") {
             printHelp(argv[0]);
             exit(0);
         } else {
             std::cerr << "Unknown option: " << arg << std::endl;
             printUsage(argv[0]);
             exit(1);
         }
     }
     return options;
 }
 
 // ----------------------------------------------------------------------------
 // 4. Helper: load data from file
 // ----------------------------------------------------------------------------
 static std::vector<uint8_t> loadDataFromFile(const std::string& filename, bool isAscii) {
     std::ifstream file(filename, (isAscii ? std::ios::in : std::ios::binary));
     if (!file) {
         throw std::runtime_error("Cannot open file: " + filename);
     }
 
     std::vector<uint8_t> data;
     if (isAscii) {
         char ch;
         while (file.get(ch)) {
             if (ch == '0') data.push_back(0);
             else if (ch == '1') data.push_back(1);
         }
     } else {
         // read binary
         data.assign((std::istreambuf_iterator<char>(file)), 
                      std::istreambuf_iterator<char>());
     }
     return data;
 }
 
 // ----------------------------------------------------------------------------
 // 5. Perform CA analysis
 // ----------------------------------------------------------------------------
 static void performCellularAutomataAnalysis(const std::vector<uint8_t>& cipherData,
                                             const CACACLIOptions& options)
 {
     using namespace nist_sts;
     // Print basic info
     std::cout << "\n=== Original Data Analysis (NIST Tests) ===\n";
     NISTTestSuite nistTester;
     // Use the test suite’s convenience method or your own method:
     std::string originalSummary = nistTester.generateSummary(cipherData);
     std::cout << originalSummary << "\n";
 
     // Optional additional stats
     std::cout << "Additional Stats:\n";
     std::cout << "  Index of Coincidence: " << StatAnalyzer::indexOfCoincidence(cipherData) << "\n";
     std::cout << "  Chi-Square:          " << StatAnalyzer::chiSquare(cipherData) << "\n";
     std::cout << "  Serial Correlation:  " << StatAnalyzer::serialCorrelation(cipherData) << "\n";
 
     // Now do CA for each rule
     for (int rule : options.caRules) {
         std::cout << "\n--- Cellular Automata with Rule " << rule << " ---\n";
         CellularAutomataProcessor caProcessor(cipherData.size(), rule);
         caProcessor.initializeFromCiphertext(cipherData);
 
         auto startTime = std::chrono::high_resolution_clock::now();
         for (int i = 0; i < options.iterations; i++) {
             caProcessor.updateCA_SIMD();
         }
         auto endTime = std::chrono::high_resolution_clock::now();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
 
         // Extract processed data
         auto processedData = caProcessor.extractProcessedData();
 
         std::cout << "Processing Time: " << duration.count() << " ms\n";
 
         // Run NIST tests on processed data
         std::string processedSummary = nistTester.generateSummary(processedData);
         std::cout << processedSummary << "\n";
 
         // More stats
         double ioc = StatAnalyzer::indexOfCoincidence(processedData);
         double chi = StatAnalyzer::chiSquare(processedData);
         double corr = StatAnalyzer::serialCorrelation(processedData);
 
         std::cout << "Additional Stats:\n";
         std::cout << "  Index of Coincidence: " << ioc << "\n";
         std::cout << "  Chi-Square:           " << chi << "\n";
         std::cout << "  Serial Correlation:   " << corr << "\n";
 
         // Optionally write processed data out
         if (!options.outputFile.empty()) {
             std::string outName = options.outputFile + "_rule" + std::to_string(rule);
             std::ofstream outFile(outName, std::ios::binary);
             outFile.write(reinterpret_cast<const char*>(processedData.data()), processedData.size());
             std::cout << "Processed data saved to: " << outName << "\n";
         }
     }
 }
 
 // ----------------------------------------------------------------------------
 // 6. Perform generator analysis
 // ----------------------------------------------------------------------------
 static void performGeneratorAnalysis(const CACACLIOptions& options) {
     using namespace nist_sts;
 
     if (options.listGenerators) {
         auto names = GeneratorFactory::getAvailableGenerators();
         std::cout << "Available Generators:\n";
         for (auto& n : names) {
             std::cout << "  " << n << "\n";
         }
         return;
     }
 
     // Create a test suite
     TestSuite suite = TestSuite::createDefaultSuite();
 
     if (options.testAllGenerators) {
         std::cout << "Testing All Available Generators...\n";
         auto gens = GeneratorFactory::createAllGenerators();
         for (auto& gen : gens) {
             auto results = suite.testGenerator(*gen, options.sequenceLength);
             std::cout << suite.generateSummary(results) << "\n";
         }
     } else {
         // Test specific generator
         std::cout << "Testing Generator: " << options.generatorName << "\n";
         auto gen = GeneratorFactory::createGenerator(options.generatorName);
         auto results = suite.testGenerator(*gen, options.sequenceLength);
         std::cout << suite.generateSummary(results) << "\n";
     }
 }
 
 // ----------------------------------------------------------------------------
 // 7. The main function
 // ----------------------------------------------------------------------------
 int main(int argc, char* argv[]) {
     if (argc < 2) {
         printUsage(argv[0]);
         return 1;
     }
 
     // parse CLI
     CACACLIOptions options = parseCommandLineOptions(argc, argv);
 
     // If user just wants to list generators, do so and exit
     if (options.listGenerators && !options.testAllGenerators && options.generatorName.empty()) {
         performGeneratorAnalysis(options);
         return 0;
     }
 
     try {
         if (!options.generatorName.empty() || options.testAllGenerators) {
             // Perform generator analysis
             performGeneratorAnalysis(options);
         } else if (!options.inputFile.empty()) {
             // Perform CA analysis
             auto data = loadDataFromFile(options.inputFile, options.asciiMode);
             if (data.empty()) {
                 std::cerr << "Error: no data read from " << options.inputFile << "\n";
                 return 1;
             }
             performCellularAutomataAnalysis(data, options);
         } else {
             // No input file, no generator -> usage
             printUsage(argv[0]);
         }
     } catch (const std::exception& e) {
         std::cerr << "Exception: " << e.what() << "\n";
         return 1;
     }
 
     return 0;
 }
 