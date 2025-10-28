namespace caca::simd {
    // Enhanced feature detection and dispatch
    class RuntimeDispatcher {
    public:
        // Enum capturing most granular SIMD capabilities
        enum class SIMDLevel {
            Scalar,      // Baseline fallback
            SSE2,        // Basic SIMD
            AVX,         // Advanced Vector Extensions
            AVX2,        // Advanced Vector Extensions 2
            AVX512F,     // Foundation
            AVX512DQ,    // Double & Quadword
            AVX512VNNI,  // Vector Neural Network Instructions
            // Future expansion points
            OpenCL,
            CUDA,
            ARM_NEON
        };

        // Compile-time feature detection
        template<SIMDLevel Level>
        struct DispatchTraits {
            using ComputeType = std::conditional_t
                Level == SIMDLevel::AVX512VNNI, __m512i,
                Level == SIMDLevel::AVX2, __m256i,
                Level == SIMDLevel::SSE2, __m128i,
                uint64_t  // Scalar fallback
            >;

            static constexpr size_t VectorWidth = 
                Level == SIMDLevel::AVX512VNNI ? 8 :
                Level == SIMDLevel::AVX2 ? 4 :
                Level == SIMDLevel::SSE2 ? 2 : 1;
        };

        // Runtime strategy selection
        static SIMDLevel detectBestStrategy() {
            auto features = CPUFeatures::detect();
            
            // Prioritized detection logic
            if (features->hasFeature(CPUFeatures::Feature::AVX512VNNI)) 
                return SIMDLevel::AVX512VNNI;
            if (features->hasFeature(CPUFeatures::Feature::AVX512F)) 
                return SIMDLevel::AVX512F;
            if (features->hasFeature(CPUFeatures::Feature::AVX2)) 
                return SIMDLevel::AVX2;
            if (features->hasFeature(CPUFeatures::Feature::SSE2)) 
                return SIMDLevel::SSE2;
            
            return SIMDLevel::Scalar;
        }

        // Polymorphic execution strategy
        template<typename Algorithm>
        static auto execute(Algorithm&& algo) {
            switch(detectBestStrategy()) {
                case SIMDLevel::AVX512VNNI: 
                    return algo.template vectorize<__m512i>();
                case SIMDLevel::AVX2:  
                    return algo.template vectorize<__m256i>();
                case SIMDLevel::SSE2:   
                    return algo.template vectorize<__m128i>();
                default: 
                    return algo.template vectorize<uint64_t>();
            }
        }
    };
}