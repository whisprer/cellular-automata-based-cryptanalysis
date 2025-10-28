// ca_analyzer.cpp
#include "ca_analyzer.hpp"
#include "nist_sts.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <algorithm>  // Add this for partial_sort
#include <cmath>     // Add this for log2

// Placeholder implementations for dependencies
class CellularAutomataProcessor {
public:
    CellularAutomataProcessor(size_t size) {}
    void initializeFromCiphertext(const std::vector<uint8_t>&) {}
    void updateAllRules() {}
    std::vector<uint8_t> extractDataRule30() { return {}; }
    std::vector<uint8_t> extractDataRule82() { return {}; }
    std::vector<uint8_t> extractDataRule110() { return {}; }
    std::vector<uint8_t> extractDataRule150() { return {}; }
};

std::vector<uint8_t> DataLoader::loadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Cannot open file " + filename);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), {});
}

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
    std::cout << "Running analysis on file: " << inputFilename 
              << " with " << iterations << " iterations.\n";
    
    // Execute CA iterations
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
    
    // Basic statistical analysis
    class StatAnalyzer {
    public:
        static void displayStats(const std::string& label, const std::vector<uint8_t>& data) {
            if (data.empty()) {
                std::cout << "No data to analyze for " << label << std::endl;
                return;
            }

            // Basic statistical analysis
            size_t totalBytes = data.size();
            size_t zeros = 0, ones = 0;
            double entropy = 0.0;
            std::vector<size_t> byteFrequency(256, 0);

            // Calculate byte frequencies and count zeros/ones
            for (uint8_t byte : data) {
                byteFrequency[byte]++;
                for (int i = 0; i < 8; ++i) {
                    if (byte & (1 << i)) {
                        ones++;
                    } else {
                        zeros++;
                    }
                }
            }

            // Calculate entropy
            for (size_t freq : byteFrequency) {
                if (freq > 0) {
                    double p = static_cast<double>(freq) / totalBytes;
                    entropy -= p * std::log2(p);
                }
            }

            // Display results
            std::cout << "Stats for " << label << ":\n";
            std::cout << "  Total bytes: " << totalBytes << std::endl;
            std::cout << "  Bit distribution:\n";
            std::cout << "    Zeros: " << zeros << " (" << (zeros * 100.0 / (zeros + ones)) << "%)\n";
            std::cout << "    Ones: " << ones << " (" << (ones * 100.0 / (zeros + ones)) << "%)\n";
            std::cout << "  Entropy: " << entropy << std::endl;
            
            // Top 5 most frequent bytes
            std::cout << "  Top 5 most frequent bytes:\n";
            std::vector<std::pair<uint8_t, size_t>> sortedFrequencies;
            for (int i = 0; i < 256; ++i) {
                if (byteFrequency[i] > 0) {
                    sortedFrequencies.emplace_back(i, byteFrequency[i]);
                }
            }
            
            std::partial_sort(sortedFrequencies.begin(), 
                               sortedFrequencies.begin() + std::min(sortedFrequencies.size(), size_t(5)), 
                               sortedFrequencies.end(),
                               [](const auto& a, const auto& b) { return a.second > b.second; });
            
            for (size_t i = 0; i < std::min(sortedFrequencies.size(), size_t(5)); ++i) {
                std::cout << "    Byte 0x" << std::hex << static_cast<int>(sortedFrequencies[i].first) 
                          << std::dec << ": " << sortedFrequencies[i].second << " times\n";
            }
        }
    };

    // Display stats for each rule
    StatAnalyzer::displayStats("Rule 30", data30);
    StatAnalyzer::displayStats("Rule 82", data82);
    StatAnalyzer::displayStats("Rule 110", data110);
    StatAnalyzer::displayStats("Rule 150", data150);
}