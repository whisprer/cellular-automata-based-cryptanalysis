// quadratic_congruential.cpp
#include "nist_sts/test_suite.hpp"
#include "quadratic_congruential.hpp"
#include <algorithm>

namespace nist_sts {
namespace generators {

// QuadraticCongruentialType1 implementation

QuadraticCongruentialType1::QuadraticCongruentialType1() {
    // Initialize with values from the original implementation
    std::string pHex = "987b6a6bf2c56a97291c445409920032499f9ee7ad128301b5d0254aa1a9633fdbd378d40149f1e23a13849f3d45992f5c4c6b7104099bc301f6005f9d8115e1";
    std::string gHex = "3844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5";
    
    hexToBytes(pHex, p);
    hexToBytes(gHex, g);
}

BitSequence QuadraticCongruentialType1::generate(size_t length) {
    BitSequence sequence(length);
    size_t bitsGenerated = 0;
    
    while (bitsGenerated < length) {
        // Calculate g = g^2 mod p
        std::vector<uint8_t> temp(g.size() * 2);
        modMult(temp, g, g, p);
        
        // Copy result back to g
        g = std::vector<uint8_t>(temp.begin() + p.size(), temp.end());
        
        // Extract bits from g
        for (auto byte : g) {
            for (int bit = 7; bit >= 0 && bitsGenerated < length; bit--) {
                sequence[bitsGenerated++] = (byte >> bit) & 0x01;
            }
        }
    }
    
    return sequence;
}

void QuadraticCongruentialType1::modMult(std::vector<uint8_t>& result, 
                                       const std::vector<uint8_t>& a, 
                                       const std::vector<uint8_t>& b, 
                                       const std::vector<uint8_t>& m) {
    // Simplified implementation of modular multiplication
    // In a production environment, use a proper big integer library
    
    // For now, just fill the result with placeholder values
    std::fill(result.begin(), result.end(), 0);
    
    // This is not a proper implementation, just a placeholder
    for (size_t i = 0; i < std::min(result.size(), a.size()); i++) {
        result[i + result.size() - a.size()] = a[i];
    }
}

// QuadraticCongruentialType2 implementation

QuadraticCongruentialType2::QuadraticCongruentialType2() {
    // Initialize with values from the original implementation
    std::string gHex = "7844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5";
    
    hexToBytes(gHex, g);
}

BitSequence QuadraticCongruentialType2::generate(size_t length) {
    BitSequence sequence(length);
    size_t bitsGenerated = 0;
    
    // Constants
    std::vector<uint8_t> three = {0x03};
    std::vector<uint8_t> one = {0x01};
    
    while (bitsGenerated < length) {
        // Calculate t1 = 2*g
        std::vector<uint8_t> t1(g.size() + 1, 0);
        smult(t1, 2, g);
        
        // Add 3: t1 = 2*g + 3
        add(t1, three);
        
        // Multiply: x = g * (2*g + 3)
        std::vector<uint8_t> x(t1.size() + g.size(), 0);
        multiply(x, g, t1);
        
        // Add 1: x = g*(2*g + 3) + 1
        add(x, one);
        
        // Update g with the result (modulo 2^512 by taking lowest 64 bytes)
        g = std::vector<uint8_t>(x.end() - 64, x.end());
        
        // Extract bits from g
        for (auto byte : g) {
            for (int bit = 7; bit >= 0 && bitsGenerated < length; bit--) {
                sequence[bitsGenerated++] = (byte >> bit) & 0x01;
            }
        }
    }
    
    return sequence;
}

void QuadraticCongruentialType2::smult(std::vector<uint8_t>& result, 
                                     uint8_t scalar, 
                                     const std::vector<uint8_t>& x) {
    // Simple scalar multiplication implementation
    std::fill(result.begin(), result.end(), 0);
    
    uint16_t carry = 0;
    for (size_t i = x.size(); i > 0; i--) {
        uint16_t temp = x[i-1] * scalar + carry;
        result[i] = temp & 0xFF;
        carry = temp >> 8;
    }
    result[0] = carry;
}

void QuadraticCongruentialType2::add(std::vector<uint8_t>& a, 
                                   const std::vector<uint8_t>& b) {
    // Add b to a in place
    uint16_t carry = 0;
    for (size_t i = a.size(); i > 0 && (i > a.size() - b.size() || carry); i--) {
        uint16_t temp = a[i-1] + carry;
        if (i > a.size() - b.size()) {
            temp += b[i - (a.size() - b.size()) - 1];
        }
        a[i-1] = temp & 0xFF;
        carry = temp >> 8;
    }
}

void QuadraticCongruentialType2::multiply(std::vector<uint8_t>& result, 
                                        const std::vector<uint8_t>& a, 
                                        const std::vector<uint8_t>& b) {
    // Simple multiplication implementation
    std::fill(result.begin(), result.end(), 0);
    
    for (size_t i = 0; i < a.size(); i++) {
        uint16_t carry = 0;
        for (size_t j = 0; j < b.size(); j++) {
            size_t k = i + j + 1;
            uint16_t temp = result[k] + ((uint16_t)a[i] * b[j]) + carry;
            result[k] = temp & 0xFF;
            carry = temp >> 8;
        }
        result[i] = carry;
    }
}

// Common methods for QuadraticCongruentialGenerator

void QuadraticCongruentialGenerator::hexToBytes(const std::string& hex, 
                                              std::vector<uint8_t>& bytes) {
    bytes.resize(hex.length() / 2);
    
    for (size_t i = 0; i < bytes.size(); i++) {
        std::string byteString = hex.substr(i * 2, 2);
        bytes[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }
}

} // namespace generators
} // namespace nist_sts