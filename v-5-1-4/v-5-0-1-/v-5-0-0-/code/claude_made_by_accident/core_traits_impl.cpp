#pragma once

#include <type_traits>
#include <concepts>
#include <cstdint>
#include <vector>
#include <bit>

namespace caca::core::traits {
    // Compile-time type traits and concepts

    // Concept for bit sequence types
    template<typename T>
    concept BitSequenceType = requires(T a) {
        { a.size() } -> std::convertible_to<size_t>;
        { a[0] } -> std::convertible_to<bool>;
    };

    // Concept for random generators
    template<typename T>
    concept RandomGenerator = requires(T generator, size_t length) {
        { generator.generate(length) } -> BitSequenceType;
        { generator.getName() } -> std::convertible_to<std::string>;
    };

    // Concept for statistical tests
    template<typename T>
    concept StatisticalTest = requires(T test, const std::vector<uint8_t>& data) {
        { test.execute(data) } -> std::convertible_to<TestResult>;
        { test.getName() } -> std::convertible_to<std::string>;
    };

    // Type traits for aligned memory
    template<typename T>
    struct is_trivially_relocatable : 
        std::bool_constant<std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>> {};

    // Compile-time endianness detection
    constexpr bool is_little_endian = std::endian::native == std::endian::little;
    constexpr bool is_big_endian = std::endian::native == std::endian::big;

    // Byte swap utility for endian conversion
    template<typename T>
    constexpr T byte_swap(T value) noexcept {
        if constexpr (sizeof(T) == 1) {
            return value;
        } else if constexpr (sizeof(T) == 2) {
            return static_cast<T>((value >> 8) | (value << 8));
        } else if constexpr (sizeof(T) == 4) {
            return (value >> 24) | 
                   ((value >> 8) & 0xFF00) | 
                   ((value << 8) & 0xFF0000) | 
                   (value << 24);
        } else if constexpr (sizeof(T) == 8) {
            return (value >> 56) | 
                   ((value >> 40) & 0xFF00) | 
                   ((value >> 24) & 0xFF0000) | 
                   ((value >> 8) & 0xFF000000) | 
                   ((value << 8) & 0xFF00000000) | 
                   ((value << 24) & 0xFF0000000000) | 
                   ((value << 40) & 0xFF000000000000) | 
                   (value << 56);
        }
    }
}