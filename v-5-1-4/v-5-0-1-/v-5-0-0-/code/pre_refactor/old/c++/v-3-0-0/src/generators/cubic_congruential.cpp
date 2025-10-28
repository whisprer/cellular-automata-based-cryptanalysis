// cubic_congruential.cpp
#include "../test_suite.hpp"
#include "cubic_congruential.hpp"
#include <algorithm>

namespace nist_sts {
namespace generators {

CubicCongruentialGenerator::CubicCongruentialGenerator() {
    // Initialize with values from the original implementation
    std::string gHex = "7844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5";
    
    hexToBytes(gHex, g);
}

// cubic_congruential.cpp (continued)
BitSequence CubicCongruentialGenerator::generate(size_t length) {
    BitSequence sequence(length);
    size_t bitsGenerated = 0;
    
    while (bitsGenerated < length) {
        // Calculate g^2
        std::vector<uint8_t> tmp(g.size() * 2, 0);
        multiply(tmp, g, g);
        
        // Calculate g^3 = g^2 * g
        std::vector<uint8_t> x(tmp.size() + g.size(), 0);
        multiply(x, tmp, g);
        
        // Take lowest 64 bytes (modulo 2^512)
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

void CubicCongruentialGenerator::multiply(std::vector<uint8_t>& result,
                                        const std::vector<uint8_t>& a,
                                        const std::vector<uint8_t>& b) {
    // Simple multiplication implementation
    std::fill(result.begin(), result.end(), 0);
    
    for (size_t i = 0; i < a.size(); i++) {
        uint16_t carry = 0;
        for (size_t j = 0; j < b.size(); j++) {
            size_t k = i + j;
            uint16_t temp = result[k] + ((uint16_t)a[i] * b[j]) + carry;
            result[k] = temp & 0xFF;
            carry = temp >> 8;
        }
        if (carry && i + b.size() < result.size()) {
            result[i + b.size()] = carry;
        }
    }
}

void CubicCongruentialGenerator::hexToBytes(const std::string& hex,
                                          std::vector<uint8_t>& bytes) {
    bytes.resize(hex.length() / 2);
    
    for (size_t i = 0; i < bytes.size(); i++) {
        std::string byteString = hex.substr(i * 2, 2);
        bytes[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }
}

} // namespace generators
} // namespace nist_sts