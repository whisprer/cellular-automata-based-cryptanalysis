/*****************************************************
 * File: core_class_structure.hpp
 *****************************************************/
#pragma once

#include <string>
#include <vector>

namespace nist_sts {
    // Forward declare if needed
    struct TestResult;
}

namespace caca {

struct Options {
    std::string inputFile;
    bool asciiMode = false;
    // Add anything else your "core" might need
};

class Core {
public:
    Core() = default;
    // Or if you have special constructor, define it

    // Example method
    void execute(const Options& options);

private:
    // Any internal fields
};

} // namespace caca
