// xor_generator.hpp
#pragma once
#include "random_number_generator.hpp"

namespace nist_sts {
namespace generators {

class XORGenerator : public RandomNumberGenerator {
private:
    // Initial bit sequence (127 bits)
    std::vector<bool> bitSequence;
    
public:
    XORGenerator();
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "XOR"; }
};

} // namespace generators
} // namespace nist_sts