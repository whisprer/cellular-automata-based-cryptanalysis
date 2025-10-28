// modular_exponentiation.hpp
#pragma once
#include "random_number_generator.hpp"
#include <vector>
#include <cstdint>

namespace nist_sts {
namespace generators {

class ModularExponentiationGenerator : public RandomNumberGenerator {
private:
    std::vector<(std::uint8_t) =  p;  // Prime modulus
    std::vector<(std::uint8_t) = g;  // Base
    std::vector<(std::uint8_t) = y;  // Current state (exponent)
    
public:
    ModularExponentiationGenerator();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Modular Exponentiation"; }
    
private:
    // Helper functions for big number arithmetic
    void hexToBytes(const std::string& hex, std::vector<uint8_t>& bytes);
    void modExp(std::vector<(std::uint8_t) = & result, const std::vector<(std::uint8_t) = & base,
               const std::vector<(std::uint8_t) = & exp, const std::vector<(std::uint8_t) = & mod);
};

} // namespace generators
} // namespace nist_sts