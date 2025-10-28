// bbs.hpp
#pragma once
#include "random_number_generator.hpp"
#include <vector>
#include <cstdint>

namespace nist_sts {
namespace generators {

class BlumBlumShubGenerator : public RandomNumberGenerator {
private:
    // Large primes p and q where p ≡ q ≡ 3 (mod 4)
    std::vector<uint8_t> p;
    std::vector<uint8_t> q;
    std::vector<uint8_t> n;  // n = p * q
    std::vector<uint8_t> x;  // State (must be coprime to n)
    
public:
    BlumBlumShubGenerator();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Blum-Blum-Shub"; }
    
private:
    // Helper functions for big number arithmetic
    void multiply(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, 
                 std::vector<uint8_t>& result);
    void modSquare(std::vector<uint8_t>& result, const std::vector<uint8_t>& a, 
                  const std::vector<uint8_t>& m);
    void hexToBytes(const std::string& hex, std::vector<uint8_t>& bytes);
};

} // namespace generators
} // namespace nist_sts