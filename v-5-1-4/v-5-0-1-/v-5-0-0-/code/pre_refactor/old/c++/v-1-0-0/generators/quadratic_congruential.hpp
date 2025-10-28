// quadratic_congruential.hpp
#pragma once
#include "random_number_generator.hpp"
#include <vector>
#include <cstdint>

namespace nist_sts {
namespace generators {

// Base class for both quadratic congruential generators
class QuadraticCongruentialGenerator : public RandomNumberGenerator {
protected:
    std::vector<uint8_t> g;  // Generator state
    
    // Helper functions for big number arithmetic
    void hexToBytes(const std::string& hex, std::vector<uint8_t>& bytes);
};

// Type I: x_{n+1} = x_n^2 mod p
class QuadraticCongruentialType1 : public QuadraticCongruentialGenerator {
private:
    std::vector<uint8_t> p;  // Prime modulus
    
public:
    QuadraticCongruentialType1();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Quadratic Congruential Type 1"; }
    
private:
    void modMult(std::vector<uint8_t>& result, const std::vector<uint8_t>& a, 
                const std::vector<uint8_t>& b, const std::vector<uint8_t>& m);
};

// Type II: x_{n+1} = x_n(2x_n + 3) + 1 mod 2^L
class QuadraticCongruentialType2 : public QuadraticCongruentialGenerator {
public:
    QuadraticCongruentialType2();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Quadratic Congruential Type 2"; }
    
private:
    void smult(std::vector<uint8_t>& result, uint8_t scalar, const std::vector<uint8_t>& x);
    void add(std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
    void multiply(std::vector<uint8_t>& result, const std::vector<uint8_t>& a, 
                 const std::vector<uint8_t>& b);
};

} // namespace generators
} // namespace nist_sts