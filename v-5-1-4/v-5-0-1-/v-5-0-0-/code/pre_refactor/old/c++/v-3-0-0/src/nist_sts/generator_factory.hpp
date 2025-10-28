// generator_factory.hpp
#pragma once
#include "random_number_generator.hpp"
#include <memory>
#include <string>
#include <vector>

namespace nist_sts {

class GeneratorFactory {
public:
    // Get a list of available generator names
    static std::vector<std::string> getAvailableGenerators();
    
    // Create a generator by name
    static std::unique_ptr<RandomNumberGenerator> createGenerator(const std::string& name);
    
    // Create all available generators
    static std::vector<std::unique_ptr<RandomNumberGenerator>> createAllGenerators();
};

} // namespace nist_sts