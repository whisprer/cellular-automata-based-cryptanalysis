// lcg.hpp
#pragma once
#include "random_number_generator.hpp"

namespace nist_sts {
namespace generators {

class LinearCongruentialGenerator : public RandomNumberGenerator {
private:
    double seed;
    
public:
    explicit LinearCongruentialGenerator(double initialSeed = 23482349.0);
    BitSequence generate(size_t length) override;
    std::string getName() const override { return "Linear Congruential"; }
};

} // namespace generators
} // namespace nist_sts