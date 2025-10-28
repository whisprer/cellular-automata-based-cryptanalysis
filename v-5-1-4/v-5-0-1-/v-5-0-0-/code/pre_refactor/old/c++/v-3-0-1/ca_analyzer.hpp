#ifndef CA_ANALYZER_HPP
#define CA_ANALYZER_HPP

#include <vector>
#include <cstdint>
#include <immintrin.h>

class CellularAutomataProcessor {
private:
    size_t dataSize;
    int ruleNumber;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> nextGrid;

public:
    // Constructor
    CellularAutomataProcessor(size_t size, int rule);

    // Initialize the grid from ciphertext
    void initializeFromCiphertext(const std::vector<uint8_t>& cipherData);

    // Update the cellular automata using SIMD
    void updateCA_SIMD();

    // Get rule byte for specific rules
    uint8_t getRuleByte() const;

    // Extract processed data
    std::vector<uint8_t> extractProcessedData() const;
};

#endif // CA_ANALYZER_HPP