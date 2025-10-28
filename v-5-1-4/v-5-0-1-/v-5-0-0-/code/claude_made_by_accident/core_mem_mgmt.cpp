#pragma once

#include <memory>
#include <type_traits>
#include <cstddef>
#include <immintrin.h>

namespace caca::core::memory {
    // RAII aligned memory allocator
    template<typename T, size_t Alignment = 64>  // 64-byte alignment for AVX-512
    class AlignedAllocator {
    private:
        T* m_ptr = nullptr;
        size_t m_size = 0;

    public:
        // Constructors
        AlignedAllocator() = default;

        explicit AlignedAllocator(size_t size) : m_size(size) {
            // Use aligned allocation with AVX-512 compatibility
            m_ptr = static_cast<T*>(_mm_malloc(size * sizeof(T), Alignment));
            if (!m_ptr) {
                throw std::bad_alloc();
            }
        }

        // Rule of five
        ~AlignedAllocator() {
            if (m_ptr) {
                _mm_free(m_ptr);
            }
        }

        // Move constructor
        AlignedAllocator(AlignedAllocator&& other) noexcept 
            : m_ptr(other.m_ptr), m_size(other.m_size) {
            other.m_ptr = nullptr;
            other.m_size = 0;
        }

        // Move assignment
        AlignedAllocator& operator=(AlignedAllocator&& other) noexcept {
            if (this != &other) {
                if (m_ptr) {
                    _mm_free(m_ptr);
                }
                m_ptr = other.m_ptr;
                m_size = other.m_size;
                other.m_ptr = nullptr;
                other.m_size = 0;
            }
            return *this;
        }

        // Disable copy construction and assignment
        AlignedAllocator(const AlignedAllocator&) = delete;
        AlignedAllocator& operator=(const AlignedAllocator&) = delete;

        // Accessor methods
        T* get() noexcept { return m_ptr; }
        const T* get() const noexcept { return m_ptr; }
        size_t size() const noexcept { return m_size; }

        // Element access
        T& operator[](size_t index) {
            return m_ptr[index];
        }

        const T& operator[](size_t index) const {
            return m_ptr[index];
        }

        // Explicit conversion to pointer
        explicit operator T*() noexcept { return m_ptr; }
        explicit operator const T*() const noexcept { return m_ptr; }
    };

    // RAII wrapper for temporary SIMD context management
    class SIMDContextGuard {
    public:
        // Save and potentially modify SIMD control/status registers
        SIMDContextGuard() {
            // Save current SIMD state
            _mm_save_state();
            
            // Optionally set specific SIMD modes
            #ifdef __AVX512F__
                _mm512_setcsr(_MM_ROUND_NEAREST | _MM_FLUSH_ZERO_ON | _MM_EXCEPT_MASK);
            #elif defined(__AVX2__)
                _mm256_setcsr(_MM_ROUND_NEAREST | _MM_FLUSH_ZERO_ON | _MM_EXCEPT_MASK);
            #endif
        }

        // Restore previous SIMD state
        ~SIMDContextGuard() {
            // Restore saved SIMD state
            _mm_restore_state();
        }

        // Prevent copying
        SIMDContextGuard(const SIMDContextGuard&) = delete;
        SIMDContextGuard& operator=(const SIMDContextGuard&) = delete;
    };
}