#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>         // for std::log2
#include <stdexcept>     // for std::runtime_error

// You can adjust NUM_THREADS to your preference for multi-threading
constexpr int NUM_THREADS = 8;

// ============================= CipherDataLoader =============================
class CipherDataLoader {
public:
    static std::vector<uint8_t> loadCipherText(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Error: Cannot open file " + filename);
        }

        // Read all bytes into a buffer
        std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), {});
        return buffer;
    }
};

// ======================= CellularAutomataProcessor (1D) ======================
class CellularAutomataProcessor {
private:
    std::vector<std::bitset<8>> grid;      // Current state
    std::vector<std::bitset<8>> nextGrid;  // Next state
    size_t dataSize;                       // Number of bytes

public:
    // Constructor: allocate the grid to match the file size
    CellularAutomataProcessor(size_t sz) : dataSize(sz) {
        grid.resize(dataSize);
        nextGrid.resize(dataSize);
    }

    // Initialize from the loaded ciphertext
    void initializeFromCiphertext(const std::vector<uint8_t> &cipherData) {
        for (size_t i = 0; i < cipherData.size(); i++) {
            // Store each byte as an 8-bit bitset
            grid[i] = std::bitset<8>(cipherData[i]);
        }
    }

    // Perform one iteration of the CA
    void updateCellularAutomata() {
        // Define our Rule 82 function
        auto rule82 = [](std::bitset<8> leftCell,
                         std::bitset<8> centerCell,
                         std::bitset<8> rightCell) -> std::bitset<8> {
            std::bitset<8> next;
            // Rule 82 in binary (from 111..000 = 7..0):
            //  111 -> 0
            //  110 -> 1
            //  101 -> 0
            //  100 -> 1
            //  011 -> 0
            //  010 -> 0
            //  001 -> 1
            //  000 -> 0
            // which is 0b01010010 in "Wolfram" indexing.
            static const unsigned char RULE_82 = 0b01010010;

            for (int b = 0; b < 8; b++) {
                // Extract bit b from each cell
                int l = (int)leftCell[b];
                int c = (int)centerCell[b];
                int r = (int)rightCell[b];

                // Combine into a 3-bit pattern
                int idx = (l << 2) | (c << 1) | r;  // range 0..7

                // We want the (7 - idx)-th bit in RULE_82
                // because 111 => bit #7, 000 => bit #0.
                int nextBit = (RULE_82 >> (7 - idx)) & 1;

                // Set the b-th bit in the result
                next[b] = nextBit;
            }
            return next;
        };

        // Launch multi-threaded updates
        std::vector<std::thread> workers;
        workers.reserve(NUM_THREADS);

        for (int t = 0; t < NUM_THREADS; ++t) {
            workers.emplace_back([&, t]() {
                for (size_t i = t; i < dataSize; i += NUM_THREADS) {
                    // If no left neighbor, use 0
                    std::bitset<8> leftCell  = (i > 0) ? grid[i - 1] : std::bitset<8>(0);
                    // Current cell
                    std::bitset<8> centerCell = grid[i];
                    // If no right neighbor, use 0
                    std::bitset<8> rightCell = (i < dataSize - 1) ? grid[i + 1] : std::bitset<8>(0);

                    nextGrid[i] = rule82(leftCell, centerCell, rightCell);
                }
            });
        }

        // Wait for all threads
        for (auto &thr : workers) {
            thr.join();
        }

        // Swap next -> current
        grid.swap(nextGrid);
    }

    // Extract the final data as raw bytes
    std::vector<uint8_t> extractProcessedData() {
        std::vector<uint8_t> out;
        out.reserve(dataSize);
        for (auto &bits : grid) {
            out.push_back(static_cast<uint8_t>(bits.to_ulong()));
        }
        return out;
    }
};

// ============================== PatternAnalyzer ==============================
class PatternAnalyzer {
public:
    // Calculate Shannon entropy of the final data
    static double calculateEntropy(const std::vector<uint8_t> &data) {
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

    // Simple threshold check for "possible structure"
    static void detectPatterns(const std::vector<uint8_t> &processedData) {
        double entropy = calculateEntropy(processedData);
        std::cout << "Entropy of processed data: " << entropy << std::endl;

        if (entropy < 6.9) {
            std::cout << "Possible structure detected in encrypted data!\n";
        } else {
            std::cout << "No clear structure detected.\n";
        }
    }
};

// ================================ MainApp ===================================
class MainApp {
public:
    static void run(const std::string &filename) {
        try {
            // 1) Load file data
            auto cipherData = CipherDataLoader::loadCipherText(filename);

            // 2) Set up CA for the entire file size
            CellularAutomataProcessor caProcessor(cipherData.size());
            caProcessor.initializeFromCiphertext(cipherData);

            std::cout << "Processing Cellular Automata...\n";
            auto startTime = std::chrono::high_resolution_clock::now();

            // 3) Perform N iterations (here, 5)
            for (int i = 0; i < 2; i++) {
                caProcessor.updateCellularAutomata();
            }

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = endTime - startTime;
            std::cout << "CA Processing completed in " 
                      << duration.count() << " seconds.\n";

            // 4) Extract final data and analyze
            auto processedData = caProcessor.extractProcessedData();
            PatternAnalyzer::detectPatterns(processedData);

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

// ================================== main() ==================================
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <encrypted_file>\n";
        return 1;
    }

    MainApp::run(argv[1]);
    return 0;
}
