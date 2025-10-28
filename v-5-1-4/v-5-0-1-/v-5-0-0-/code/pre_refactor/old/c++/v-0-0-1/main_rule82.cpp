#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <thread>
#include <chrono>
#include <random>
#include <immintrin.h> // For SIMD optimization

constexpr int GRID_SIZE = 256;  // CA grid size
constexpr int NUM_THREADS = 4;  // For multi-threading

// ============================ CipherDataLoader ==============================
class CipherDataLoader {
public:
    static std::vector<uint8_t> loadCipherText(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Error: Cannot open file " + filename);
        }

        std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), {});
        return buffer;
    }
};

// ============================ CellularAutomataProcessor ==============================
class CellularAutomataProcessor {
private:
    std::vector<std::bitset<8>> grid;
    std::vector<std::bitset<8>> nextGrid;
    int width, height;

public:
    CellularAutomataProcessor(int w, int h) : width(w), height(h) {
        grid.resize(width * height);
        nextGrid.resize(width * height);
    }

    void initializeFromCiphertext(const std::vector<uint8_t>& cipherData) {
        for (size_t i = 0; i < cipherData.size() && i < grid.size(); i++) {
            grid[i] = std::bitset<8>(cipherData[i]);  // Store byte as 8-bit CA cell
        }
    }

    void updateCellularAutomata() {
        // Replace your existing rule-lambda with something like this:
    auto rule82 = [](std::bitset<8> leftCell, 
                   std::bitset<8> centerCell, 
                   std::bitset<8> rightCell) -> std::bitset<8> 
    {
      std::bitset<8> next;
    
      // For each bit in the byte:
        for (int b = 0; b < 8; b++) {
            // Extract the b-th bit (0 or 1) from left, center, right
            int l = (int)leftCell[b];    // 0 or 1
            int c = (int)centerCell[b]; 
            int r = (int)rightCell[b];
        
            // Combine into 3-bit pattern: [l,c,r]
            int idx = (l << 2) | (c << 1) | r;  // 0..7
        
            /*
              Rule 82 = 0b01010010 in “Wolfram order” from 111..000:
                111 -> bit 7  ( msb ) 
                110 -> bit 6
                101 -> bit 5
                ...
                000 -> bit 0  ( lsb )
            */
            static const unsigned char RULE_82 = 0b01010010;
        
            // Next state is the (7 - idx)-th bit of RULE_82 if
            // you label bits from left(111) to right(000).
            // Or you can invert that, depending on how you prefer to index.
            // Here we do the usual approach: for pattern i,
            // we want bit (7 - i) in RULE_82:
            int nextBit = (RULE_82 >> (7 - idx)) & 1;
        
            // Set that bit in our "next" output
            next[b] = nextBit;
        }
    
        return next;
    };


        std::vector<std::thread> workers;

        for (int i = t; i < width * height; i += NUM_THREADS) {
          // If there's no left neighbor, treat it as all 0 bits
          std::bitset<8> leftCell = (i > 0) ? grid[i - 1] : std::bitset<8>(0);
          // Current cell
          std::bitset<8> centerCell = grid[i];
          // If there's no right neighbor, treat as all 0 bits
          std::bitset<8> rightCell = (i < (width * height) - 1) ? grid[i + 1] : std::bitset<8>(0);
      
          // Apply Rule 82
          nextGrid[i] = rule82(leftCell, centerCell, rightCell);
      }

        for (auto& w : workers) {
            w.join();
        }

        grid.swap(nextGrid); // Update grid
    }

    std::vector<uint8_t> extractProcessedData() {
        std::vector<uint8_t> processed;
        for (const auto& cell : grid) {
            processed.push_back(static_cast<uint8_t>(cell.to_ulong()));
        }
        return processed;
    }
};

// ============================ PatternAnalyzer ==============================
class PatternAnalyzer {
public:
    static double calculateEntropy(const std::vector<uint8_t>& data) {
        int frequency[256] = {0};
        for (uint8_t byte : data) {
            frequency[byte]++;
        }

        double entropy = 0.0;
        for (int freq : frequency) {
            if (freq > 0) {
                double p = static_cast<double>(freq) / data.size();
                entropy -= p * std::log2(p);
            }
        }
        return entropy;
    }

    static void detectPatterns(const std::vector<uint8_t>& processedData) {
        double entropy = calculateEntropy(processedData);
        std::cout << "Entropy of processed data: " << entropy << std::endl;

        if (entropy < 7.5) {  // If significantly below 8.0, it suggests non-randomness
            std::cout << "Possible structure detected in encrypted data!\n";
        } else {
            std::cout << "No clear structure detected.\n";
        }
    }
};

// ============================ MainApp ==============================
class MainApp {
public:
    static void run(const std::string& filename) {
        try {
            auto cipherData = CipherDataLoader::loadCipherText(filename);

            CellularAutomataProcessor caProcessor(GRID_SIZE, GRID_SIZE);
            caProcessor.initializeFromCiphertext(cipherData);

            std::cout << "Processing Cellular Automata...\n";

            auto startTime = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < 5; i++) {  // Run CA iterations
                caProcessor.updateCellularAutomata();
            }

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = endTime - startTime;

            auto processedData = caProcessor.extractProcessedData();

            std::cout << "CA Processing completed in " << duration.count() << " seconds.\n";

            PatternAnalyzer::detectPatterns(processedData);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

// ============================ Main Function ==============================
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <encrypted_file>\n";
        return 1;
    }

    MainApp::run(argv[1]);

    return 0;
}