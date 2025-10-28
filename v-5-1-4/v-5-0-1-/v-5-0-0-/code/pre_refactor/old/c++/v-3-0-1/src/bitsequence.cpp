// bitsequence.cpp
#include "test_suite.hpp"
#include "bitsequence.hpp"
#include <stdexcept>
#include <algorithm>

namespace nist_sts {

BitSequence::BitSequence(size_t size) : bits(size) {}

BitSequence::BitSequence(const std::vector<bool>& data) : bits(data) {}

BitSequence BitSequence::fromBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    BitSequence result;
    char byte;
    while (file.get(byte)) {
        for (int i = 7; i >= 0; --i) {
            result.push_back((byte >> i) & 1);
        }
    }
    
    return result;
}

BitSequence BitSequence::fromAsciiFile(const std::string& filename) {
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
        // Ignore other characters (whitespace, etc.)
    }
    
    return result;
}

bool BitSequence::operator[](size_t index) const {
    return bits[index];
}

BitReference BitSequence::operator[](size_t index) {
    return BitReference(bits, index);
}

size_t BitSequence::size() const {
    return bits.size();
}

void BitSequence::resize(size_t newSize) {
    bits.resize(newSize);
}

void BitSequence::push_back(bool bit) {
    bits.push_back(bit);
}

size_t BitSequence::countOnes() const {
    return std::count(bits.begin(), bits.end(), true);
}

size_t BitSequence::countZeros() const {
    return std::count(bits.begin(), bits.end(), false);
}

BitSequence::iterator BitSequence::begin() {
    return bits.begin();
}

BitSequence::iterator BitSequence::end() {
    return bits.end();
}

BitSequence::const_iterator BitSequence::begin() const {
    return bits.begin();
}

BitSequence::const_iterator BitSequence::end() const {
    return bits.end();
}

BitSequence::const_iterator BitSequence::cbegin() const {
    return bits.cbegin();
}

BitSequence::const_iterator BitSequence::cend() const {
    return bits.cend();
}

} // namespace nist_sts