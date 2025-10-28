// xor_generator.cpp
#include "xor_generator.hpp"

namespace nist_sts {
namespace generators {

XORGenerator::XORGenerator() {
    // Initialize with the default seed from the original implementation
    std::string seed = "0001011011011001000101111001001010011011101101000100000010101111111010100100001010110110000000000100110000101110011111111100111";
    
    bitSequence.resize(127);
    for (size_t i = 0; i < 127; i++) {
        bitSequence[i] = (seed[i] == '1');
    }
}

BitSequence XORGenerator::generate(size_t length) {
    BitSequence sequence(length);
    
    // Copy the initial 127 bits (if length is that large)
    for (size_t i = 0; i < std::min(length, bitSequence.size()); i++) {
        sequence[i] = bitSequence[i];
    }
    
    // Generate remaining bits using XOR of previous bits
    for (size_t i = 127; i < length; i++) {
        sequence[i] = sequence[i-1] != sequence[i-127];  // XOR operation
        
        // Update the circular buffer
        bitSequence[i % 127] = sequence[i];
    }
    
    return sequence;
}

} // namespace generators
} // namespace nist_sts