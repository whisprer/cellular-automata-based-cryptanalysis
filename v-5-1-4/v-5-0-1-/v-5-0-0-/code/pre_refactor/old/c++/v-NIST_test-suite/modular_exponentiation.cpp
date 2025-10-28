// modular_exponentiation.cpp
#include "modular_exponentiation.hpp"
#include <algorithm>

namespace nist_sts {
namespace generators {

ModularExponentiationGenerator::ModularExponentiationGenerator() {
    // Initialize with values from the original implementation
    std::string pHex = "987b6a6bf2c56a97291c445409920032499f9ee7ad128301b5d0254aa1a9633fdbd378d40149f1e23a13849f3d45992f5c4c6b7104099bc301f6005f9d8115e1";
    std::string gHex = "3844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5";
    std::string yHex = "7AB36982CE1ADF832019CDFEB2393CABDF0214EC";
    
    hexToBytes(pHex, p);
    hexToBytes(gHex, g);
    hexToBytes(yHex, y);
}

BitSequence ModularExponentiationGenerator::generate(size_t length) {
    BitSequence sequence(length);
    size_t bitsGenerated = 0;
    
    while (bitsGenerated < length) {
        // Calculate x = g^y mod p
        std::vector<uint8_t> x(p.size() * 2, 0);
        modExp(x, g, y, p);
        
        // Extract bits from x to generate output
        for (size_t i = 0; i < x.size() && bitsGenerated < length; i++) {
            for (int bit = 7; bit >= 0 && bitsGenerated < length; bit--) {
                sequence[bitsGenerated++] = (x[i] >> bit) & 0x01;
            }
        }
        
        // Update y for next iteration (using the last 20 bytes of x)
        std::copy(x.begin() + 44, x.begin() + 64, y.begin());
    }
    
    return sequence;
}

void ModularExponentiationGenerator::modExp(std::vector<uint8_t>& result,
                                          const std::vector<uint8_t>& base,
                                          const std::vector<uint8_t>& exp,
                                          const std::vector<uint8_t>& mod) {
    // Simplified placeholder for modular exponentiation
    // In a real implementation, use proper modular exponentiation algorithm
    
    // For demonstration, just fill result with placeholder values
    std::fill(result.begin(), result.end(), 0);
    
    // Copy some values from base to result
    // This is NOT a proper modular exponentiation!
    std::copy(base.begin(), base.end(), result.begin() + result.size() - base.size());
}

void ModularExponentiationGenerator::hexToBytes(const std::string& hex,
                                              std::vector<uint8_t>& bytes) {
    bytes.resize(hex.length() / 2);
    
    for (size_t i = 0; i < bytes.size(); i++) {
        std::string byteString = hex.substr(i * 2, 2);
        bytes[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }
}

} // namespace generators
} // namespace nist_sts