// caca_optimized.cpp - High-performance version with parallelization
// Compile with: g++ -std=c++17 -O3 -march=native -fopenmp -o caca_fast caca_optimized.cpp -lm -pthread

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <complex>
#include <random>
#include <chrono>
#include <iomanip>
#include <bitset>
#include <numeric>
#include <array>
#include <immintrin.h>
#include <omp.h>
#include <thread>
#include <atomic>
#include <execution>

// Define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nist_sts {

// ============================================================================
// Performance Monitoring
// ============================================================================

class PerfTimer {
    std::chrono::high_resolution_clock::time_point start;
    std::string name;
public:
    PerfTimer(const std::string& n) : name(n), start(std::chrono::high_resolution_clock::now()) {}
    ~PerfTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "[PERF] " << name << ": " << duration.count() / 1000.0 << " ms\n";
    }
};

// ============================================================================
// Optimized BitSequence with SIMD operations
// ============================================================================

class BitSequence {
private:
    std::vector<uint64_t> blocks;  // Store as 64-bit blocks for faster processing
    size_t bit_count;
    
public:
    BitSequence(size_t size = 0) : bit_count(size) {
        blocks.resize((size + 63) / 64, 0);
    }
    
    static BitSequence fromBytes(const std::vector<uint8_t>& bytes) {
        BitSequence seq(bytes.size() * 8);
        
        // Parallel conversion using OpenMP
        #pragma omp parallel for
        for (size_t i = 0; i < bytes.size(); i += 8) {
            size_t block_idx = i / 8;
            if (block_idx < seq.blocks.size()) {
                uint64_t block = 0;
                for (size_t j = 0; j < 8 && i + j < bytes.size(); ++j) {
                    block |= (static_cast<uint64_t>(bytes[i + j]) << (j * 8));
                }
                seq.blocks[block_idx] = block;
            }
        }
        
        return seq;
    }
    
    bool operator[](size_t index) const {
        size_t block_idx = index / 64;
        size_t bit_idx = index % 64;
        return (blocks[block_idx] >> bit_idx) & 1;
    }
    
    size_t size() const { return bit_count; }
    
    // SIMD-optimized bit counting
    size_t countOnes() const {
        size_t count = 0;
        
        #pragma omp parallel for reduction(+:count)
        for (size_t i = 0; i < blocks.size(); ++i) {
            count += __builtin_popcountll(blocks[i]);
        }
        
        return count;
    }
    
    size_t countZeros() const {
        return bit_count - countOnes();
    }
};

// ============================================================================
// Parallel Statistical Tests
// ============================================================================

struct TestResult {
    std::string testName;
    double p_value = 0.0;
    bool success = false;
    std::map<std::string, double> statistics;
};

class StatisticalTest {
public:
    virtual ~StatisticalTest() = default;
    virtual TestResult execute(const BitSequence& data) = 0;
    virtual std::string getName() const = 0;
    
protected:
    bool isSuccess(double p_value) const {
        return p_value >= 0.01;
    }
};

// Optimized Frequency Test with SIMD
class FrequencyTest : public StatisticalTest {
public:
    TestResult execute(const BitSequence& data) override {
        PerfTimer timer("FrequencyTest");
        TestResult result;
        result.testName = getName();
        
        if (data.size() == 0) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        size_t ones = data.countOnes();
        double proportion = ones / static_cast<double>(data.size());
        double sum = 2.0 * ones - data.size();
        double s_obs = std::abs(sum) / std::sqrt(data.size());
        
        result.p_value = std::erfc(s_obs / std::sqrt(2.0));
        result.success = isSuccess(result.p_value);
        result.statistics["proportion"] = proportion;
        
        return result;
    }
    
    std::string getName() const override { return "Frequency (Monobit)"; }
};

// Parallel Block Frequency Test
class BlockFrequencyTest : public StatisticalTest {
private:
    size_t blockSize;
    
public:
    BlockFrequencyTest(size_t blockSize = 128) : blockSize(blockSize) {}
    
    TestResult execute(const BitSequence& data) override {
        PerfTimer timer("BlockFrequencyTest");
        TestResult result;
        result.testName = getName();
        
        if (data.size() < blockSize) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        size_t numBlocks = data.size() / blockSize;
        std::atomic<double> chi_squared(0.0);
        
        #pragma omp parallel for
        for (size_t block = 0; block < numBlocks; ++block) {
            size_t ones = 0;
            for (size_t i = 0; i < blockSize; ++i) {
                if (data[block * blockSize + i]) ones++;
            }
            
            double pi = ones / static_cast<double>(blockSize);
            double v = pi - 0.5;
            double local_chi = 4.0 * blockSize * v * v;
            
            // Atomic add for thread safety
            double expected = chi_squared.load();
            while (!chi_squared.compare_exchange_weak(expected, expected + local_chi));
        }
        
        result.p_value = std::exp(-chi_squared.load() / 2.0);
        result.success = isSuccess(result.p_value);
        result.statistics["chi_squared"] = chi_squared.load();
        
        return result;
    }
    
    std::string getName() const override { return "Block Frequency"; }
};

// Test Suite with parallel execution
class NISTTestSuite {
private:
    std::vector<std::unique_ptr<StatisticalTest>> tests;
    
public:
    NISTTestSuite() {
        tests.push_back(std::make_unique<FrequencyTest>());
        tests.push_back(std::make_unique<BlockFrequencyTest>());
        // Add more tests as needed
    }
    
    std::vector<TestResult> runTests(const std::vector<uint8_t>& data) {
        PerfTimer timer("NISTTestSuite::runTests");
        BitSequence bits = BitSequence::fromBytes(data);
        std::vector<TestResult> results(tests.size());
        
        // Run tests in parallel
        #pragma omp parallel for
        for (size_t i = 0; i < tests.size(); ++i) {
            results[i] = tests[i]->execute(bits);
        }
        
        return results;
    }
    
    std::string generateSummary(const std::vector<uint8_t>& data) {
        auto results = runTests(data);
        std::stringstream ss;
        
        ss << "NIST Statistical Test Suite Results\n";
        ss << "=====================================\n";
        ss << "Data size: " << data.size() << " bytes\n";
        ss << "Threads: " << omp_get_max_threads() << "\n\n";
        
        for (const auto& result : results) {
            ss << std::left << std::setw(25) << result.testName
               << std::fixed << std::setprecision(6) 
               << std::setw(12) << result.p_value
               << (result.success ? "PASS" : "FAIL") << "\n";
        }
        
        return ss.str();
    }
};

} // namespace nist_sts

// ============================================================================
// Optimized Cellular Automata with SIMD and parallel processing
// ============================================================================

using nist_sts::PerfTimer;  // Bring PerfTimer into scope

class CellularAutomataProcessor {
private:
    size_t dataSize;
    int ruleNumber;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> nextGrid;
    
public:
    CellularAutomataProcessor(size_t size, int rule) 
        : dataSize(size), ruleNumber(rule), grid(size), nextGrid(size) {}
    
    void initializeFromCiphertext(const std::vector<uint8_t>& cipherData) {
        size_t limit = std::min(cipherData.size(), dataSize);
        std::copy_n(cipherData.begin(), limit, grid.begin());
    }
    
    // Highly optimized SIMD CA update
    void updateCA_SIMD() {
        PerfTimer timer("CA_SIMD_Update");
        
        const size_t simd_width = 32;  // AVX2 processes 32 bytes at once
        const size_t num_chunks = (dataSize + simd_width - 1) / simd_width;
        
        #pragma omp parallel for schedule(static)
        for (size_t chunk = 0; chunk < num_chunks; ++chunk) {
            size_t start = chunk * simd_width;
            size_t end = std::min(start + simd_width, dataSize);
            
            if (end - start == simd_width && start > 0 && end < dataSize) {
                // Full SIMD block
                __m256i left = _mm256_loadu_si256((__m256i*)&grid[start - 1]);
                __m256i center = _mm256_loadu_si256((__m256i*)&grid[start]);
                __m256i right = _mm256_loadu_si256((__m256i*)&grid[start + 1]);
                
                // Apply CA rule using SIMD
                __m256i result;
                switch(ruleNumber) {
                    case 30:
                        result = _mm256_xor_si256(left, _mm256_or_si256(center, right));
                        break;
                    case 110:
                        result = _mm256_xor_si256(center, _mm256_or_si256(left, right));
                        break;
                    case 150:
                        result = _mm256_xor_si256(_mm256_xor_si256(left, center), right);
                        break;
                    default:
                        result = center;  // Identity for unknown rules
                }
                
                _mm256_storeu_si256((__m256i*)&nextGrid[start], result);
            } else {
                // Edge cases - process scalar
                for (size_t i = start; i < end; ++i) {
                    uint8_t left = (i == 0) ? 0 : grid[i - 1];
                    uint8_t center = grid[i];
                    uint8_t right = (i == dataSize - 1) ? 0 : grid[i + 1];
                    
                    // Apply elementary CA rule
                    uint8_t newByte = 0;
                    for (int bit = 0; bit < 8; ++bit) {
                        int l = (left >> bit) & 1;
                        int c = (center >> bit) & 1;
                        int r = (right >> bit) & 1;
                        int neighborhood = (l << 2) | (c << 1) | r;
                        newByte |= ((ruleNumber >> neighborhood) & 1) << bit;
                    }
                    nextGrid[i] = newByte;
                }
            }
        }
        
        grid.swap(nextGrid);
    }
    
    std::vector<uint8_t> extractProcessedData() const {
        return grid;
    }
};

// ============================================================================
// Parallel Statistical Analyzer
// ============================================================================

class StatAnalyzer {
public:
    static double indexOfCoincidence(const std::vector<uint8_t>& data) {
        PerfTimer timer("IndexOfCoincidence");
        std::array<std::atomic<size_t>, 256> freq{};
        
        // Parallel frequency counting
        #pragma omp parallel for
        for (size_t i = 0; i < data.size(); ++i) {
            freq[data[i]]++;
        }
        
        double ic = 0.0;
        size_t n = data.size();
        
        #pragma omp parallel for reduction(+:ic)
        for (size_t i = 0; i < 256; ++i) {
            size_t count = freq[i].load();
            if (count > 1) {
                ic += count * (count - 1);
            }
        }
        
        return ic / (n * (n - 1) / 256.0);
    }
    
    static double chiSquare(const std::vector<uint8_t>& data) {
        PerfTimer timer("ChiSquare");
        std::array<std::atomic<size_t>, 256> observed{};
        
        #pragma omp parallel for
        for (size_t i = 0; i < data.size(); ++i) {
            observed[data[i]]++;
        }
        
        double expected = data.size() / 256.0;
        double chi2 = 0.0;
        
        #pragma omp parallel for reduction(+:chi2)
        for (size_t i = 0; i < 256; ++i) {
            double diff = observed[i].load() - expected;
            chi2 += (diff * diff) / expected;
        }
        
        return chi2;
    }
    
    static double entropy(const std::vector<uint8_t>& data) {
        PerfTimer timer("Entropy");
        std::array<std::atomic<size_t>, 256> freq{};
        
        #pragma omp parallel for
        for (size_t i = 0; i < data.size(); ++i) {
            freq[data[i]]++;
        }
        
        double ent = 0.0;
        size_t n = data.size();
        
        for (size_t i = 0; i < 256; ++i) {
            size_t count = freq[i].load();
            if (count > 0) {
                double p = count / static_cast<double>(n);
                ent -= p * std::log2(p);
            }
        }
        
        return ent;
    }
};

// ============================================================================
// Main program with performance options
// ============================================================================

struct Options {
    std::string inputFile;
    std::string outputFile = "caca_results";
    bool asciiMode = false;
    bool verbose = false;
    int iterations = 5;
    int threads = 0;  // 0 = auto-detect
    std::vector<int> caRules = {30, 110, 150};
};

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << "  -f, --file <file>        Input file to analyze\n"
              << "  -a, --ascii              Treat input as ASCII\n"
              << "  -o, --output <file>      Output file prefix\n"
              << "  -i, --iterations <n>     Number of CA iterations (default: 5)\n"
              << "  -t, --threads <n>        Number of threads (0=auto, default: 0)\n"
              << "  -r, --ca-rules <r1,r2>   CA rules (default: 30,110,150)\n"
              << "  -v, --verbose            Verbose output with performance metrics\n"
              << "  -h, --help               Show this help\n";
}

Options parseArgs(int argc, char** argv) {
    Options opts;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) opts.inputFile = argv[++i];
        } else if (arg == "-t" || arg == "--threads") {
            if (i + 1 < argc) opts.threads = std::stoi(argv[++i]);
        } else if (arg == "-a" || arg == "--ascii") {
            opts.asciiMode = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) opts.outputFile = argv[++i];
        } else if (arg == "-i" || arg == "--iterations") {
            if (i + 1 < argc) opts.iterations = std::stoi(argv[++i]);
        } else if (arg == "-r" || arg == "--ca-rules") {
            if (i + 1 < argc) {
                opts.caRules.clear();
                std::string rules = argv[++i];
                std::stringstream ss(rules);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    opts.caRules.push_back(std::stoi(token));
                }
            }
        } else if (arg == "-v" || arg == "--verbose") {
            opts.verbose = true;
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            exit(0);
        }
    }
    
    return opts;
}

std::vector<uint8_t> loadFile(const std::string& filename, bool ascii) {
    std::ifstream file(filename, ascii ? std::ios::in : std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::vector<uint8_t> data;
    
    if (ascii) {
        char ch;
        while (file >> ch) {
            if (ch == '0' || ch == '1') {
                data.push_back(ch - '0');
            }
        }
    } else {
        data.assign(std::istreambuf_iterator<char>(file), 
                   std::istreambuf_iterator<char>());
    }
    
    return data;
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            printUsage(argv[0]);
            return 1;
        }
        
        Options opts = parseArgs(argc, argv);
        
        // Set number of threads
        if (opts.threads > 0) {
            omp_set_num_threads(opts.threads);
        }
        
        std::cout << "\n=== CACA Optimized Edition ===\n";
        std::cout << "Threads: " << omp_get_max_threads() << "\n";
        std::cout << "SIMD: AVX2 enabled\n\n";
        
        if (opts.inputFile.empty()) {
            std::cerr << "Error: No input file specified\n";
            return 1;
        }
        
        // Load data
        std::vector<uint8_t> data = loadFile(opts.inputFile, opts.asciiMode);
        std::cout << "Loaded " << data.size() << " bytes from " << opts.inputFile << "\n\n";
        
        // Run baseline analysis
        {
            PerfTimer timer("Baseline Analysis");
            nist_sts::NISTTestSuite nist;
            std::cout << nist.generateSummary(data) << "\n";
            
            std::cout << "Statistical Measures:\n";
            std::cout << "  Index of Coincidence: " << StatAnalyzer::indexOfCoincidence(data) << "\n";
            std::cout << "  Chi-Square: " << StatAnalyzer::chiSquare(data) << "\n";
            std::cout << "  Entropy: " << StatAnalyzer::entropy(data) << " bits/byte\n\n";
        }
        
        // CA Analysis
        for (int rule : opts.caRules) {
            std::cout << "=== CA Rule " << rule << " (" << opts.iterations << " iterations) ===\n";
            
            CellularAutomataProcessor ca(data.size(), rule);
            ca.initializeFromCiphertext(data);
            
            {
                PerfTimer timer("CA Processing Total");
                for (int i = 0; i < opts.iterations; ++i) {
                    ca.updateCA_SIMD();
                }
            }
            
            auto processed = ca.extractProcessedData();
            
            // Analysis on processed data
            {
                PerfTimer timer("Post-CA Analysis");
                nist_sts::NISTTestSuite nist;
                std::cout << nist.generateSummary(processed) << "\n";
                
                std::cout << "Statistical Measures:\n";
                std::cout << "  Index of Coincidence: " << StatAnalyzer::indexOfCoincidence(processed) << "\n";
                std::cout << "  Chi-Square: " << StatAnalyzer::chiSquare(processed) << "\n";
                std::cout << "  Entropy: " << StatAnalyzer::entropy(processed) << " bits/byte\n\n";
            }
            
            // Save if requested
            if (!opts.outputFile.empty()) {
                std::string filename = opts.outputFile + "_rule" + std::to_string(rule) + ".bin";
                std::ofstream out(filename, std::ios::binary);
                out.write(reinterpret_cast<const char*>(processed.data()), processed.size());
                std::cout << "Saved to: " << filename << "\n\n";
            }
        }
        
        std::cout << "Analysis complete.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

// ============================================================================
// Compilation:
//   g++ -std=c++17 -O3 -march=native -fopenmp -o caca_fast caca_optimized.cpp -lm -pthread
//
// For maximum performance:
//   g++ -std=c++17 -O3 -march=native -mavx2 -mfma -fopenmp -funroll-loops -o caca_fast caca_optimized.cpp -lm -pthread
//
// Run with thread control:
//   ./caca_fast -f encrypted.bin -t 8 -v  # Use 8 threads with verbose output
// ============================================================================