// bitsequence.hpp
#pragma once
#include <vector>
#include <cstddef>
#include <fstream>
#include <string>

namespace nist_sts {

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
    
    // Element access
    bool operator[](size_t index) const;
    bool& operator[](size_t index);
    
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
};

} // namespace nist_sts