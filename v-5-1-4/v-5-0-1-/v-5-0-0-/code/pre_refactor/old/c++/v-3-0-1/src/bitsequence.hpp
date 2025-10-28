// bitsequence.hpp
#pragma once
#include <vector>
#include <cstddef>
#include <fstream>
#include <string>

namespace nist_sts {

// Forward declaration of our custom reference class
class BitReference;

class BitSequence {
private:
    std::vector<bool> bits;
    
public:
    // Constructors
    BitSequence(size_t size = 0);
    BitSequence(const std::vector<bool>& data);
    
    // Create from file
    static BitSequence fromBinaryFile(const std::string& filename);
    static BitSequence fromAsciiFile(const std::string& filename);
    
    // Element access - const version returns bool
    bool operator[](size_t index) const;
    
    // Non-const version returns our custom reference
    BitReference operator[](size_t index);
    
    // Capacity
    size_t size() const;
    void resize(size_t newSize);
    
    // Modifiers
    void push_back(bool bit);
    
    // Statistics
    size_t countOnes() const;
    size_t countZeros() const;
    
    // Iterator support
    using iterator = std::vector<bool>::iterator;
    using const_iterator = std::vector<bool>::const_iterator;
    
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    
    // Give BitReference access to private members
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

} // namespace nist_sts