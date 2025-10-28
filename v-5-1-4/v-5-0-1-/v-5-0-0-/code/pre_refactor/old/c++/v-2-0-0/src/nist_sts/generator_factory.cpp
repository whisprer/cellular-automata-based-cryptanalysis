// generator_factory.cpp
#include "nist_sts/test_suite.hpp" 
#include "generator_factory.hpp"
#include "generators/lcg.hpp"
#include "generators/bbs.hpp"
#include "generators/sha1.hpp"
#include "generators/xor_generator.hpp"
#include "generators/quadratic_congruential.hpp"
#include "generators/cubic_congruential.hpp"
#include "generators/micali_schnorr.hpp"
#include "generators/modular_exponentiation.hpp"
#include <stdexcept>

namespace nist_sts {

std::vector<std::string> GeneratorFactory::getAvailableGenerators() {
    return {
        "Linear Congruential",
        "Blum-Blum-Shub",
        "SHA-1",
        "XOR",
        "Quadratic Congruential Type 1",
        "Quadratic Congruential Type 2",
        "Cubic Congruential",
        "Micali-Schnorr",
        "Modular Exponentiation"
    };
}

std::unique_ptr<RandomNumberGenerator> GeneratorFactory::createGenerator(const std::string& name) {
    if (name == "Linear Congruential") {
        return std::make_unique<generators::LinearCongruentialGenerator>();
    } else if (name == "Blum-Blum-Shub") {
        return std::make_unique<generators::BlumBlumShubGenerator>();
    } else if (name == "SHA-1") {
        return std::make_unique<generators::SHA1Generator>();
    } else if (name == "XOR") {
        return std::make_unique<generators::XORGenerator>();
    } else if (name == "Quadratic Congruential Type 1") {
        return std::make_unique<generators::QuadraticCongruentialType1>();
    } else if (name == "Quadratic Congruential Type 2") {
        return std::make_unique<generators::QuadraticCongruentialType2>();
    } else if (name == "Cubic Congruential") {
        return std::make_unique<generators::CubicCongruentialGenerator>();
    } else if (name == "Micali-Schnorr") {
        return std::make_unique<generators::MicaliSchnorrGenerator>();
    } else if (name == "Modular Exponentiation") {
        return std::make_unique<generators::ModularExponentiationGenerator>();
    } else {
        throw std::invalid_argument("Unknown generator: " + name);
    }
}

std::vector<std::unique_ptr<RandomNumberGenerator>> GeneratorFactory::createAllGenerators() {
    std::vector<std::unique_ptr<RandomNumberGenerator>> generators;
    
    for (const auto& name : getAvailableGenerators()) {
        generators.push_back(createGenerator(name));
    }
    
    return generators;
}

} // namespace nist_sts