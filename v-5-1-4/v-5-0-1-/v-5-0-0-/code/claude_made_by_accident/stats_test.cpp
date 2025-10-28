#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <type_traits>
#include <concepts>

namespace caca {
    // Core type definitions
    using BitSequence = std::vector<bool>;
    using ByteSequence = std::vector<uint8_t>;

    // Core configuration traits
    namespace traits {
        // Concept for bit sequence types
        template<typename T>
        concept BitSequenceType = requires(T a) {
            { a.size() } -> std::convertible_to<size_t>;
            { a[0] } -> std::convertible_to<bool>;
        };

        // Concept for encryption/decryption operations
        template<typename T>
        concept CryptographicOperation = requires(T op, const ByteSequence& input) {
            { op.process(input) } -> std::same_as<ByteSequence>;
        };

        // Concept for random number generators
        template<typename T>
        concept RandomGenerator = requires(T gen, size_t length) {
            { gen.generate(length) } -> BitSequenceType;
            { gen.getName() } -> std::convertible_to<std::string>;
        };

        // Concept for statistical tests
        template<typename T>
        concept StatisticalTest = requires(T test, const ByteSequence& data) {
            { test.execute(data) } -> std::convertible_to<bool>;
            { test.getName() } -> std::convertible_to<std::string>;
        };
    }

    // Configuration structure for global settings
    struct CACAConfig {
        // Configurable parameters for the entire framework
        size_t default_block_size = 1024;
        size_t max_iteration_count = 10;
        bool verbose_logging = false;
        
        // Singleton-like access
        static CACAConfig& getInstance() {
            static CACAConfig instance;
            return instance;
        }

    private:
        // Private constructor to enforce singleton pattern
        CACAConfig() = default;
    };

    // Utility function for converting between sequence types
    template<typename SourceType, typename TargetType>
    TargetType convert_sequence(const SourceType& source) {
        TargetType target;
        target.reserve(source.size());
        
        for (auto item : source) {
            target.push_back(static_cast<typename TargetType::value_type>(item));
        }
        
        return target;
    }
}