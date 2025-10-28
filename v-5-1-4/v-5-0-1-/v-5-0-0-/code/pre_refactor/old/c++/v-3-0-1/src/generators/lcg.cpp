// lcg.cpp
#include "../test_suite.hpp"
#include "lcg.hpp"
#include <cmath>
#include <vector>

namespace nist_sts {
namespace generators {



LinearCongruentialGenerator::LinearCongruentialGenerator(double initialSeed) 
    : seed(initialSeed) {}

BitSequence LinearCongruentialGenerator::generate(size_t length) {
    BitSequence sequence(length);
    std::vector<double> dunif(length);
    
    // LCG algorithm implementation
    const double DTWO31 = 2147483648.0;  // 2^31
    const double DMDLS = 2147483647.0;   // 2^31-1
    const double DA1 = 41160.0;          // 950706376 MOD 2^16
    const double DA2 = 950665216.0;      // 950706376-DA1
    
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
    
    seed = dz;  // Save the state for next call
    return sequence;
}

} // namespace generators
} // namespace nist_sts