// random_number_generator.hpp
#pragma once
#include "bitsequence.hpp"
#include <string>

namespace nist_sts {

class RandomNumberGenerator {
public:
    virtual ~RandomNumberGenerator() = default;
    
    // Generate a bit sequence of the specified length
    virtual BitSequence generate(size_t length) = 0;
    
    // Get the name of the generator
    virtual std::string getName() const = 0;
};

} // namespace nist_sts