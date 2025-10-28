// sha1.hpp
#pragma once
#include "random_number_generator.hpp"
#include <vector>
#include <cstdint>

namespace nist_sts {
namespace generators {

class SHA1Generator : public RandomNumberGenerator {
private:
    std::vector<uint8_t> key;  // The generator state (SHA-1 key)
    
public:
    SHA1Generator();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "SHA-1"; }
    
private:
    // Helper functions for SHA-1
    void sha1Process(const std::vector<uint8_t>& message, std::vector<uint8_t>& digest);
    void hexToBytes(const std::string& hex, std::vector<uint8_t>& bytes);
};

} // namespace generators
} // namespace nist_sts