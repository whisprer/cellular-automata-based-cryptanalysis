#pragma once
#include <vector>
#include <cstddef>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>

namespace nist_sts {

class BitReference;

class BitSequence {
private:
    std::vector<bool> bits;
    
public:
    // Constructors
    BitSequence(size_t size = 0) : bits(size) {}
    BitSequence(const std::vector<bool>& data) : bits(data) {}
    
    // Conversion constructors from uint8_t vector
    BitSequence(const std::vector<uint8_t>& data) {
        bits.clear();
        for (auto byte : data) {
            for (int bit = 7; bit >= 0; bit--) {
                bits.push_back((byte >> bit) & 0x01);
            }
        }
    }
    
    // Element access
    bool operator[](size_t index) const { return bits[index]; }
    BitReference operator[](size_t index);
    
    // Capacity
    size_t size() const { return bits.size(); }
    void resize(size_t newSize) { bits.resize(newSize); }
    void push_back(bool bit) { bits.push_back(bit); }
    
    // Statistics
    size_t countOnes() const { 
        return std::count(bits.begin(), bits.end(), true); 
    }
    
    size_t countZeros() const { 
        return std::count(bits.begin(), bits.end(), false); 
    }
    
    // Iterator support
    using iterator = std::vector<bool>::iterator;
    using const_iterator = std::vector<bool>::const_iterator;
    
    iterator begin() { return bits.begin(); }
    iterator end() { return bits.end(); }
    const_iterator begin() const { return bits.begin(); }
    const_iterator end() const { return bits.end(); }
    const_iterator cbegin() const { return bits.cbegin(); }
    const_iterator cend() const { return bits.cend(); }
    
    // Additional utility methods
    std::vector<bool>& getData() { return bits; }
    const std::vector<bool>& getData() const { return bits; }
    
    // File loading methods
    static BitSequence fromBinaryFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        std::vector<uint8_t> rawData(
            (std::istreambuf_iterator<char>(file)), 
            std::istreambuf_iterator<char>()
        );
        
        return BitSequence(rawData);
    }
    
    static BitSequence fromAsciiFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        BitSequence result;
        char bit;
        while (file.get(bit)) {
            if (bit == '0') {
                result.push_back(false);
            } else if (bit == '1') {
                result.push_back(true);
            }
        }
        
        return result;
    }
    
    // Friend for access to private data
    friend class BitReference;
};

// Custom reference class for non-const operator[]
class BitReference {
private:
    std::vector<bool>& bits;
    size_t index;
    
public:
    BitReference(std::vector<bool>& bits, size_t index) : bits(bits), index(index) {}
    
    // Conversion operator to bool
    operator bool() const {
        return bits[index];
    }
    
    // Assignment operator
    BitReference& operator=(bool value) {
        bits[index] = value;
        return *this;
    }
    
    // Assignment operator for BitReference
    BitReference& operator=(const BitReference& other) {
        bits[index] = other.bits[other.index];
        return *this;
    }
};

inline BitReference BitSequence::operator[](size_t index) {
    return BitReference(bits, index);
}

} // namespace nist_sts