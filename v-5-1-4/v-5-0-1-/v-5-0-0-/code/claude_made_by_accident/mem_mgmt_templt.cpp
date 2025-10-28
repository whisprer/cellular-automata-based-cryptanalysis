namespace caca::core::memory {
    // RAII aligned memory allocator
    template<typename T, size_t Alignment = 64>  // 64-byte alignment for AVX-512
    class AlignedAllocator {
        T* ptr = nullptr;
    public:
        AlignedAllocator(size_t size) {
            ptr = static_cast<T*>(_mm_malloc(size * sizeof(T), Alignment));
        }
        ~AlignedAllocator() {
            _mm_free(ptr);
        }
        // Rule of five implementations...
    };
}