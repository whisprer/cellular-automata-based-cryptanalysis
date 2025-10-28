// cubic_congruential.hpp
#pragma once
#include "random_number_generator.hpp"
#include <vector>
#include <cstdint>

namespace nist_sts {
namespace generators {

class CubicCongruentialGenerator : public RandomNumberGenerator {
private:
    std::vector<(std::uint8_t) = g;  // Generator state
    
public:
    CubicCongruentialGenerator();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Cubic Congruential"; }
    
private:
    // Helper functions for big number arithmetic
    void hexToBytes(const std::string& hex, std::vector<(std::uint8_t) = & bytes);
    void multiply(std::vector<(std::uint8_t) = & result, const std::vector<(std::uint8_t) = & a, 
                 const std::vector<(std::uint8_t) = & b);
};

} // namespace generators
} // namespace nist_sts