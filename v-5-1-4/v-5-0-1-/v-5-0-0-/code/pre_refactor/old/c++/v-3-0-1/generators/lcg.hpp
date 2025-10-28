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

// lcg.cpp
#include "lcg.hpp"
#include <cmath>

namespace nist_sts {
namespace generators {

LinearCongruentialGenerator::LinearCongruentialGenerator(double initialSeed)
    : seed(initialSeed) {}

BitSequence LinearCongruentialGenerator::generate(size_t length) {
    BitSequence sequence(length);
    std::vector<double> dunif(length);
    
    // LCG algorithm implementation
    const double DTWO31 = 2147483648.0;
    const double DMDLS = 2147483647.0;
    const double DA1 = 41160.0;
    const double DA2 = 950665216.0;
    
    double dz = seed;
    
    for (size_t i = 0; i < length; i++) {
        dz = std::floor(dz);
        double dz1 = dz * DA1;
        double dz2 = dz * DA2;
        double dover1 = std::floor(dz1 / DTWO31);
        double dover2 = std::floor(dz2 / DTWO31);
        dz1 = dz1 - dover1 * DTWO31;
        dz2 = dz2 - dover2 * DTWO31;
        dz = dz1 + dz2 + dover1 + dover2;
        double dover = std::floor(dz / DMDLS);
        dz = dz - dover * DMDLS;
        dunif[i] = dz / DMDLS;
    }
    
    // Convert uniform values to bits
    for (size_t i = 0; i < length; i++) {
        sequence[i] = (dunif[i] < 0.5) ? false : true;
    }
    
    seed = dz;
    return sequence;
}

} // namespace generators
} // namespace nist_sts