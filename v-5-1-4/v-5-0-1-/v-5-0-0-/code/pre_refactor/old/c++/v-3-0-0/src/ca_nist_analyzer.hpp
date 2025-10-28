#include "ca_analyzer.hpp"
#include <immintrin.h>
#include <algorithm>

// Constructor implementation
CellularAutomataProcessor::CellularAutomataProcessor(size_t size, int rule) 
    : dataSize(size), ruleNumber(rule), grid(size), nextGrid(size) {}

// Initialize grid from ciphertext
void CellularAutomataProcessor::initializeFromCiphertext(const std::vector<uint8_t>& cipherData) {
    size_t limit = std::min(cipherData.size(), dataSize);
    std::copy_n(cipherData.begin(), limit, grid.begin());
}

// Get rule byte for specific rules
uint8_t CellularAutomataProcessor::getRuleByte() const {
    switch(ruleNumber) {
        case 30:  return 0x1E;
        case 82:  return 0x52;
        case 110: return 0x6E;
        case 150: return 0x96;
        default:  return 0x00;
    }
}

// SIMD-based Cellular Automata update
void CellularAutomataProcessor::updateCA_SIMD() {
    __m256i ruleVec = _mm256_set1_epi8(getRuleByte());
    size_t i = 0;
    
    for (; i + 32 <= dataSize; i += 32) {
        __m256i left = (i == 0) ? _mm256_setzero_si256() : _mm256_loadu_si256((__m256i*)&grid[i - 1]);
        __m256i center = _mm256_loadu_si256((__m256i*)&grid[i]);
        __m256i right = _mm256_loadu_si256((__m256i*)&grid[i + 1]);

        __m256i next = _mm256_xor_si256(_mm256_xor_si256(left, center), right);
        next = _mm256_xor_si256(next, ruleVec);
        _mm256_storeu_si256((__m256i*)&nextGrid[i], next);
    }

    // Remaining bytes processed scalar
    for (; i < dataSize; i++) {
        uint8_t left = (i == 0) ? 0 : grid[i - 1];
        uint8_t right = (i == dataSize - 1) ? 0 : grid[i + 1];
        nextGrid[i] = (left ^ grid[i] ^ right ^ getRuleByte());
    }

    // Swap grids
    grid.swap(nextGrid);
}

// Extract processed data
std::vector<uint8_t> CellularAutomataProcessor::extractProcessedData() const {
    return grid;
}