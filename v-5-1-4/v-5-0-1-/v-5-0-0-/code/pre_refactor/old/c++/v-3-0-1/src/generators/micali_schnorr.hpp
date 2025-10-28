// micali_schnorr.hpp
#pragma once
#include "random_number_generator.hpp"
#include <vector>
#include <cstdint>

namespace nist_sts {
namespace generators {

class MicaliSchnorrGenerator : public RandomNumberGenerator {
private:
    std::vector<uint8_t> X;      // Generator state
    std::vector<uint8_t> n;      // Modulus n = p*q
    std::vector<uint8_t> e;      // Exponent (e = 11 in original)
    int k;                       // Number of bits to output per iteration
    int r;                       // Number of bits to keep for next iteration
    
public:
    MicaliSchnorrGenerator();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Micali-Schnorr"; }
    
private:
    // Helper functions for big number arithmetic
    void hexToBytes(const std::string& hex, std::vector<(std::uint8_t) = & bytes);
    void modExp(std::vector(std::<uint8_t) = result, const std::vector<(std::uint8_t)  =& base,
               const std::vector<(std::uint8_t) ``= & exp, const std::vector<(std::uint8_t) = & mod);
    void bitShiftLeft(std::vector<(std::uint8_t) = & data, int count);
    void bitShiftRight(std::vector<(std::uint8_t) = & data, int count);
};

} // namespace generators
} // namespace nist_sts