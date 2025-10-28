// simple_ca_test.cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

// Simple CA implementation without SIMD
class SimpleCellularAutomata {
private:
    std::vector<uint8_t> grid;
    std::vector<uint8_t> nextGrid;
    int ruleNumber;
    
public:
    SimpleCellularAutomata(const std::vector<uint8_t>& initialData, int rule) 
        : grid(initialData), nextGrid(initialData.size()), ruleNumber(rule) {}
    
    uint8_t getRuleByte() const {
        switch(ruleNumber) {
            case 30: return 0x1E;
            case 82: return 0x52;
            case 110: return 0x6E;
            case 150: return 0x96;
            default: return 0x00;
        }
    }
    
    void update() {
        uint8_t ruleByte = getRuleByte();
        
        for (size_t i = 0; i < grid.size(); i++) {
            uint8_t left = (i == 0) ? 0 : grid[i - 1];
            uint8_t center = grid[i];
            uint8_t right = (i == grid.size() - 1) ? 0 : grid[i + 1];
            
            // Apply CA rule (XOR with rule byte)
            nextGrid[i] = (left ^ center ^ right ^ ruleByte);
        }
        
        // Swap grids
        grid.swap(nextGrid);
    }
    
    const std::vector<uint8_t>& getData() const {
        return grid;
    }
};

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

// Save binary data to file
void saveData(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Helper to display byte histogram 
void displayHistogram(const std::vector<uint8_t>& data, int maxBins = 16) {
    std::vector<int> histogram(256, 0);
    
    for (auto byte : data) {
        histogram[byte]++;
    }
    
    // Show distribution
    std::cout << "Byte value distribution (in 16 bins):\n";
    for (int bin = 0; bin < maxBins; bin++) {
        int count = 0;
        int start = bin * 16;
        int end = start + 15;
        
        for (int i = start; i <= end; i++) {
            count += histogram[i];
        }
        
        std::cout << "[" << start << "-" << end << "]: " << count << " bytes\n";
    }
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <encrypted_file> [output_prefix]\n";
            return 1;
        }
        
        std::string inputFile = argv[1];
        std::string outputPrefix = (argc > 2) ? argv[2] : "ca_results";
        
        // Load encrypted data
        std::cout << "Loading data from " << inputFile << "...\n";
        auto data = loadData(inputFile);
        std::cout << "Loaded " << data.size() << " bytes\n";
        
        // Display initial histogram
        std::cout << "\nOriginal data histogram:\n";
        displayHistogram(data);
        
        // Define CA rules to test
        std::vector<int> rules = {30, 82, 110, 150};
        
        // Run CA with different rules and iterations
        for (int rule : rules) {
            for (int iterations : {1, 3, 5, 10}) {
                std::cout << "\nApplying CA Rule " << rule << " with " << iterations << " iterations...\n";
                
                // Initialize CA processor
                SimpleCellularAutomata ca(data, rule);
                
                // Apply CA iterations
                for (int i = 0; i < iterations; i++) {
                    ca.update();
                }
                
                // Get processed data
                const auto& processedData = ca.getData();
                
                // Display histogram 
                std::cout << "Processed data histogram:\n";
                displayHistogram(processedData);
                
                // Save processed data
                std::string outputFile = outputPrefix + "_rule" + std::to_string(rule) + 
                                        "_iter" + std::to_string(iterations) + ".bin";
                saveData(outputFile, processedData);
                std::cout << "Saved processed data to " << outputFile << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}