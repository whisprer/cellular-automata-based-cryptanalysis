// caca_main.cpp - Complete unified implementation
// Compile with: g++ -std=c++17 -O3 -march=native -o caca caca_main.cpp -lm -pthread

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

// Define M_PI if not defined (Windows compatibility)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nist_sts {

// ============================================================================
// Core Data Structures
// ============================================================================

constexpr double ALPHA = 0.01;  // Significance level for NIST tests

struct TestResult {
    std::string testName;
    double p_value = 0.0;
    bool success = false;
    std::map<std::string, double> statistics;
};

// ============================================================================
// BitSequence - Core bit manipulation class
// ============================================================================

class BitSequence {
private:
    std::vector<bool> bits;
    
public:
    BitSequence(size_t size = 0) : bits(size) {}
    BitSequence(const std::vector<bool>& data) : bits(data) {}
    
    static BitSequence fromBytes(const std::vector<uint8_t>& bytes) {
        BitSequence seq;
        seq.bits.reserve(bytes.size() * 8);
        for (uint8_t byte : bytes) {
            for (int i = 7; i >= 0; --i) {
                seq.bits.push_back((byte >> i) & 1);
            }
        }
        return seq;
    }
    
    bool operator[](size_t index) const { return bits[index]; }
    size_t size() const { return bits.size(); }
    void push_back(bool bit) { bits.push_back(bit); }
    
    size_t countOnes() const {
        return std::count(bits.begin(), bits.end(), true);
    }
    
    size_t countZeros() const {
        return bits.size() - countOnes();
    }
    
    std::vector<uint8_t> toBytes() const {
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < bits.size(); i += 8) {
            uint8_t byte = 0;
            for (size_t j = 0; j < 8 && i + j < bits.size(); ++j) {
                if (bits[i + j]) byte |= (1 << (7 - j));
            }
            bytes.push_back(byte);
        }
        return bytes;
    }
};

// ============================================================================
// Statistical Test Base Class
// ============================================================================

class StatisticalTest {
public:
    virtual ~StatisticalTest() = default;
    virtual TestResult execute(const BitSequence& data) = 0;
    virtual std::string getName() const = 0;
    
protected:
    bool isSuccess(double p_value) const {
        return p_value >= ALPHA;
    }
};

// ============================================================================
// NIST Statistical Tests Implementation
// ============================================================================

// 1. Frequency (Monobit) Test
class FrequencyTest : public StatisticalTest {
public:
    TestResult execute(const BitSequence& data) override {
        TestResult result;
        result.testName = getName();
        
        if (data.size() == 0) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        double sum = 0.0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum += data[i] ? 1.0 : -1.0;
        }
        
        double s_obs = std::abs(sum) / std::sqrt(data.size());
        result.p_value = std::erfc(s_obs / std::sqrt(2.0));
        result.success = isSuccess(result.p_value);
        
        result.statistics["sum"] = sum;
        result.statistics["s_obs"] = s_obs;
        result.statistics["proportion"] = (data.countOnes() / static_cast<double>(data.size()));
        
        return result;
    }
    
    std::string getName() const override { return "Frequency (Monobit)"; }
};

// 2. Block Frequency Test
class BlockFrequencyTest : public StatisticalTest {
private:
    size_t blockSize;
    
public:
    BlockFrequencyTest(size_t blockSize = 128) : blockSize(blockSize) {}
    
    TestResult execute(const BitSequence& data) override {
        TestResult result;
        result.testName = getName();
        
        if (data.size() < blockSize) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        size_t numBlocks = data.size() / blockSize;
        double chi_squared = 0.0;
        
        for (size_t block = 0; block < numBlocks; ++block) {
            size_t ones = 0;
            for (size_t i = 0; i < blockSize; ++i) {
                if (data[block * blockSize + i]) ones++;
            }
            
            double pi = ones / static_cast<double>(blockSize);
            double v = pi - 0.5;
            chi_squared += 4.0 * blockSize * v * v;
        }
        
        result.p_value = std::exp(-chi_squared / 2.0);
        result.success = isSuccess(result.p_value);
        result.statistics["chi_squared"] = chi_squared;
        result.statistics["num_blocks"] = numBlocks;
        
        return result;
    }
    
    std::string getName() const override { return "Block Frequency"; }
};

// 3. Runs Test
class RunsTest : public StatisticalTest {
public:
    TestResult execute(const BitSequence& data) override {
        TestResult result;
        result.testName = getName();
        
        if (data.size() < 100) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        double pi = data.countOnes() / static_cast<double>(data.size());
        
        // Pre-test check
        if (std::abs(pi - 0.5) >= 2.0 / std::sqrt(data.size())) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        size_t runs = 1;
        for (size_t i = 1; i < data.size(); ++i) {
            if (data[i] != data[i-1]) runs++;
        }
        
        double pi_term = 2.0 * pi * (1.0 - pi);
        double numerator = std::abs(runs - 2.0 * data.size() * pi * (1.0 - pi));
        double denominator = 2.0 * std::sqrt(2.0 * data.size()) * pi * (1.0 - pi);
        
        result.p_value = std::erfc(numerator / denominator);
        result.success = isSuccess(result.p_value);
        result.statistics["runs"] = runs;
        result.statistics["proportion"] = pi;
        
        return result;
    }
    
    std::string getName() const override { return "Runs"; }
};

// 4. DFT (Spectral) Test - Simplified
class DFTTest : public StatisticalTest {
public:
    TestResult execute(const BitSequence& data) override {
        TestResult result;
        result.testName = getName();
        
        size_t n = data.size();
        if (n < 1000) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        // Convert to +1/-1
        std::vector<double> X(n);
        for (size_t i = 0; i < n; ++i) {
            X[i] = data[i] ? 1.0 : -1.0;
        }
        
        // Simple DFT (first half only)
        size_t halfN = n / 2;
        std::vector<double> magnitudes(halfN);
        
        for (size_t k = 0; k < halfN; ++k) {
            double real = 0.0, imag = 0.0;
            for (size_t j = 0; j < n; ++j) {
                double angle = -2.0 * M_PI * k * j / n;
                real += X[j] * std::cos(angle);
                imag += X[j] * std::sin(angle);
            }
            magnitudes[k] = std::sqrt(real * real + imag * imag);
        }
        
        double T = std::sqrt(std::log(1.0 / 0.05) * n);
        size_t N0 = 0;
        for (double m : magnitudes) {
            if (m < T) N0++;
        }
        
        double N1 = halfN - N0;
        double d = (N1 - 0.95 * halfN / 2.0) / std::sqrt(n * 0.95 * 0.05 / 4.0);
        result.p_value = std::erfc(std::abs(d) / std::sqrt(2.0));
        result.success = isSuccess(result.p_value);
        result.statistics["peaks_above_threshold"] = N1;
        
        return result;
    }
    
    std::string getName() const override { return "DFT (Spectral)"; }
};

// 5. Approximate Entropy Test
class ApproximateEntropyTest : public StatisticalTest {
private:
    size_t m;
    
public:
    ApproximateEntropyTest(size_t blockLength = 10) : m(blockLength) {}
    
    TestResult execute(const BitSequence& data) override {
        TestResult result;
        result.testName = getName();
        
        size_t n = data.size();
        if (n < 100) {
            result.p_value = 0.0;
            result.success = false;
            return result;
        }
        
        auto phi = [&](size_t length) -> double {
            std::map<std::string, int> patterns;
            
            for (size_t i = 0; i < n; ++i) {
                std::string pattern;
                for (size_t j = 0; j < length; ++j) {
                    pattern += data[(i + j) % n] ? '1' : '0';
                }
                patterns[pattern]++;
            }
            
            double sum = 0.0;
            for (const auto& [pattern, count] : patterns) {
                double pi = count / static_cast<double>(n);
                if (pi > 0) sum += pi * std::log(pi);
            }
            return sum;
        };
        
        double phi_m = phi(m);
        double phi_m1 = phi(m + 1);
        double ApEn = phi_m - phi_m1;
        double chi_squared = 2.0 * n * (std::log(2.0) - ApEn);
        
        result.p_value = 1.0 - std::exp(-chi_squared / 2.0);
        result.success = isSuccess(result.p_value);
        result.statistics["ApEn"] = ApEn;
        result.statistics["chi_squared"] = chi_squared;
        
        return result;
    }
    
    std::string getName() const override { return "Approximate Entropy"; }
};

// ============================================================================
// NIST Test Suite
// ============================================================================

class NISTTestSuite {
private:
    std::vector<std::unique_ptr<StatisticalTest>> tests;
    
public:
    NISTTestSuite() {
        tests.push_back(std::make_unique<FrequencyTest>());
        tests.push_back(std::make_unique<BlockFrequencyTest>());
        tests.push_back(std::make_unique<RunsTest>());
        tests.push_back(std::make_unique<DFTTest>());
        tests.push_back(std::make_unique<ApproximateEntropyTest>());
    }
    
    std::vector<TestResult> runTests(const std::vector<uint8_t>& data) {
        BitSequence bits = BitSequence::fromBytes(data);
        std::vector<TestResult> results;
        
        for (const auto& test : tests) {
            results.push_back(test->execute(bits));
        }
        
        return results;
    }
    
    std::string generateSummary(const std::vector<uint8_t>& data) {
        auto results = runTests(data);
        std::stringstream ss;
        
        ss << "NIST Statistical Test Suite Results\n";
        ss << "=====================================\n";
        ss << "Data size: " << data.size() << " bytes (" << data.size() * 8 << " bits)\n\n";
        ss << std::left << std::setw(25) << "Test Name" 
           << std::setw(12) << "P-Value" 
           << "Result\n";
        ss << std::string(45, '-') << "\n";
        
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
// Cellular Automata Processor
// ============================================================================

class CellularAutomataProcessor {
private:
    size_t dataSize;
    int ruleNumber;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> nextGrid;
    
    uint8_t applyRule(uint8_t left, uint8_t center, uint8_t right) const {
        // Elementary CA rule application
        int neighborhood = (left << 2) | (center << 1) | right;
        return (ruleNumber >> neighborhood) & 1;
    }
    
public:
    CellularAutomataProcessor(size_t size, int rule) 
        : dataSize(size), ruleNumber(rule), grid(size), nextGrid(size) {}
    
    void initializeFromCiphertext(const std::vector<uint8_t>& cipherData) {
        size_t limit = std::min(cipherData.size(), dataSize);
        std::copy_n(cipherData.begin(), limit, grid.begin());
    }
    
    void updateCA() {
        // Process each byte
        for (size_t i = 0; i < dataSize; ++i) {
            uint8_t newByte = 0;
            
            for (int bit = 0; bit < 8; ++bit) {
                // Get neighborhood for this bit
                int leftBit, centerBit, rightBit;
                
                // Handle bit positions with wrapping
                if (bit == 0) {
                    leftBit = (i == 0) ? 0 : ((grid[i-1] >> 7) & 1);
                    centerBit = (grid[i] >> bit) & 1;
                    rightBit = (grid[i] >> (bit + 1)) & 1;
                } else if (bit == 7) {
                    leftBit = (grid[i] >> (bit - 1)) & 1;
                    centerBit = (grid[i] >> bit) & 1;
                    rightBit = (i == dataSize - 1) ? 0 : (grid[i+1] & 1);
                } else {
                    leftBit = (grid[i] >> (bit - 1)) & 1;
                    centerBit = (grid[i] >> bit) & 1;
                    rightBit = (grid[i] >> (bit + 1)) & 1;
                }
                
                // Apply rule
                int newBit = applyRule(leftBit, centerBit, rightBit);
                newByte |= (newBit << bit);
            }
            
            nextGrid[i] = newByte;
        }
        
        grid.swap(nextGrid);
    }
    
    void updateCA_SIMD() {
        // SIMD optimized version for supported rules
        if (ruleNumber == 30 || ruleNumber == 110 || ruleNumber == 150) {
            const __m256i rule = _mm256_set1_epi8(ruleNumber);
            
            size_t i = 0;
            for (; i + 32 <= dataSize; i += 32) {
                __m256i left = (i == 0) ? _mm256_setzero_si256() 
                                        : _mm256_loadu_si256((__m256i*)&grid[i - 1]);
                __m256i center = _mm256_loadu_si256((__m256i*)&grid[i]);
                __m256i right = (i + 32 >= dataSize) ? _mm256_setzero_si256()
                                                     : _mm256_loadu_si256((__m256i*)&grid[i + 1]);
                
                // Simple XOR-based CA operation
                __m256i result = _mm256_xor_si256(left, center);
                result = _mm256_xor_si256(result, right);
                
                _mm256_storeu_si256((__m256i*)&nextGrid[i], result);
            }
            
            // Handle remaining bytes
            for (; i < dataSize; ++i) {
                updateCA();  // Fallback to scalar for remainder
                break;
            }
            
            grid.swap(nextGrid);
        } else {
            updateCA();  // Use scalar version for other rules
        }
    }
    
    std::vector<uint8_t> extractProcessedData() const {
        return grid;
    }
};

// ============================================================================
// Statistical Analyzer
// ============================================================================

class StatAnalyzer {
public:
    static double indexOfCoincidence(const std::vector<uint8_t>& data) {
        std::array<size_t, 256> freq = {0};
        for (uint8_t byte : data) {
            freq[byte]++;
        }
        
        double ic = 0.0;
        size_t n = data.size();
        for (size_t count : freq) {
            if (count > 1) {
                ic += count * (count - 1);
            }
        }
        
        return ic / (n * (n - 1) / 256.0);
    }
    
    static double chiSquare(const std::vector<uint8_t>& data) {
        std::array<size_t, 256> observed = {0};
        for (uint8_t byte : data) {
            observed[byte]++;
        }
        
        double expected = data.size() / 256.0;
        double chi2 = 0.0;
        
        for (size_t count : observed) {
            double diff = count - expected;
            chi2 += (diff * diff) / expected;
        }
        
        return chi2;
    }
    
    static double serialCorrelation(const std::vector<uint8_t>& data) {
        if (data.size() < 2) return 0.0;
        
        double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
        size_t n = data.size() - 1;
        
        for (size_t i = 0; i < n; ++i) {
            sum1 += data[i] * data[i + 1];
            sum2 += data[i];
            sum3 += data[i] * data[i];
        }
        sum2 += data[n];
        sum3 += data[n] * data[n];
        
        double correlation = (n * sum1 - sum2 * sum2) / 
                           (n * sum3 - sum2 * sum2);
        
        return correlation;
    }
    
    static double entropy(const std::vector<uint8_t>& data) {
        std::array<size_t, 256> freq = {0};
        for (uint8_t byte : data) {
            freq[byte]++;
        }
        
        double ent = 0.0;
        size_t n = data.size();
        
        for (size_t count : freq) {
            if (count > 0) {
                double p = count / static_cast<double>(n);
                ent -= p * std::log2(p);
            }
        }
        
        return ent;
    }
};

// ============================================================================
// Command Line Options and Main Program
// ============================================================================

struct Options {
    std::string inputFile;
    std::string outputFile = "caca_results";
    bool asciiMode = false;
    bool verbose = false;
    int iterations = 5;
    std::vector<int> caRules = {30, 110, 150};
};

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << "  -f, --file <file>        Input file to analyze\n"
              << "  -a, --ascii              Treat input as ASCII (default: binary)\n"
              << "  -o, --output <file>      Output file prefix\n"
              << "  -i, --iterations <n>     Number of CA iterations (default: 5)\n"
              << "  -r, --ca-rules <r1,r2>   Comma-separated CA rules (default: 30,110,150)\n"
              << "  -v, --verbose            Verbose output\n"
              << "  -h, --help               Show this help message\n\n"
              << "Examples:\n"
              << "  " << programName << " -f encrypted.bin -i 10\n"
              << "  " << programName << " -f input.txt -a -r 30,82,110,150\n";
}

Options parseArgs(int argc, char** argv) {
    Options opts;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) opts.inputFile = argv[++i];
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

void analyzeData(const std::vector<uint8_t>& data, const Options& opts) {
    using namespace nist_sts;
    
    std::cout << "\n========================================\n";
    std::cout << "CACA - Cellular Automata Cryptanalysis\n";
    std::cout << "========================================\n\n";
    
    std::cout << "Input file: " << opts.inputFile << "\n";
    std::cout << "Data size: " << data.size() << " bytes\n";
    std::cout << "CA iterations: " << opts.iterations << "\n";
    std::cout << "CA rules to test: ";
    for (int rule : opts.caRules) {
        std::cout << rule << " ";
    }
    std::cout << "\n\n";
    
    // Baseline analysis
    std::cout << "=== BASELINE ANALYSIS ===\n\n";
    
    NISTTestSuite nist;
    std::cout << nist.generateSummary(data) << "\n";
    
    std::cout << "Statistical Measures:\n";
    std::cout << "  Index of Coincidence: " << std::fixed << std::setprecision(6) 
              << StatAnalyzer::indexOfCoincidence(data) << "\n";
    std::cout << "  Chi-Square: " << StatAnalyzer::chiSquare(data) << "\n";
    std::cout << "  Serial Correlation: " << StatAnalyzer::serialCorrelation(data) << "\n";
    std::cout << "  Entropy: " << StatAnalyzer::entropy(data) << " bits/byte\n\n";
    
    // Store results for comparison
    std::map<std::string, std::map<std::string, double>> allResults;
    
    // CA Analysis
    for (int rule : opts.caRules) {
        std::cout << "=== CELLULAR AUTOMATA RULE " << rule << " ===\n\n";
        
        CellularAutomataProcessor ca(data.size(), rule);
        ca.initializeFromCiphertext(data);
        
        // Apply CA iterations
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < opts.iterations; ++i) {
            if (opts.verbose) {
                std::cout << "  Iteration " << (i + 1) << "/" << opts.iterations << "\r" << std::flush;
            }
            ca.updateCA_SIMD();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        if (opts.verbose) {
            std::cout << "\n";
        }
        
        std::cout << "Processing time: " << duration.count() << " ms\n\n";
        
        // Get processed data
        auto processed = ca.extractProcessedData();
        
        // Run NIST tests
        std::cout << nist.generateSummary(processed) << "\n";
        
        // Calculate statistics
        double ioc = StatAnalyzer::indexOfCoincidence(processed);
        double chi2 = StatAnalyzer::chiSquare(processed);
        double corr = StatAnalyzer::serialCorrelation(processed);
        double ent = StatAnalyzer::entropy(processed);
        
        std::cout << "Statistical Measures:\n";
        std::cout << "  Index of Coincidence: " << std::fixed << std::setprecision(6) << ioc << "\n";
        std::cout << "  Chi-Square: " << chi2 << "\n";
        std::cout << "  Serial Correlation: " << corr << "\n";
        std::cout << "  Entropy: " << ent << " bits/byte\n\n";
        
        // Store results
        std::string key = "Rule" + std::to_string(rule) + "_Iter" + std::to_string(opts.iterations);
        allResults[key]["IoC"] = ioc;
        allResults[key]["ChiSquare"] = chi2;
        allResults[key]["Correlation"] = corr;
        allResults[key]["Entropy"] = ent;
        
        // Save processed data if requested
        if (!opts.outputFile.empty()) {
            std::string filename = opts.outputFile + "_rule" + std::to_string(rule) + ".bin";
            std::ofstream out(filename, std::ios::binary);
            out.write(reinterpret_cast<const char*>(processed.data()), processed.size());
            std::cout << "Processed data saved to: " << filename << "\n\n";
        }
    }
    
    // Summary comparison
    std::cout << "=== COMPARATIVE ANALYSIS ===\n\n";
    std::cout << std::left << std::setw(20) << "Configuration" 
              << std::setw(12) << "IoC"
              << std::setw(12) << "Chi-Square"
              << std::setw(12) << "Correlation"
              << std::setw(12) << "Entropy" << "\n";
    std::cout << std::string(68, '-') << "\n";
    
    // Original data
    std::cout << std::left << std::setw(20) << "Original"
              << std::setw(12) << std::fixed << std::setprecision(6) 
              << StatAnalyzer::indexOfCoincidence(data)
              << std::setw(12) << std::fixed << std::setprecision(2)
              << StatAnalyzer::chiSquare(data)
              << std::setw(12) << std::fixed << std::setprecision(6)
              << StatAnalyzer::serialCorrelation(data)
              << std::setw(12) << StatAnalyzer::entropy(data) << "\n";
    
    // CA processed data
    for (const auto& [config, stats] : allResults) {
        std::cout << std::left << std::setw(20) << config
              << std::setw(12) << std::fixed << std::setprecision(6) << stats.at("IoC")
              << std::setw(12) << std::fixed << std::setprecision(2) << stats.at("ChiSquare")
              << std::setw(12) << std::fixed << std::setprecision(6) << stats.at("Correlation")
              << std::setw(12) << stats.at("Entropy") << "\n";
    }
    
    std::cout << "\n";
    
    // Pattern detection analysis
    std::cout << "=== PATTERN DETECTION ANALYSIS ===\n\n";
    
    // Check for significant changes in statistics
    bool patternDetected = false;
    for (const auto& [config, stats] : allResults) {
        double iocChange = std::abs(stats.at("IoC") - StatAnalyzer::indexOfCoincidence(data));
        double chiChange = std::abs(stats.at("ChiSquare") - StatAnalyzer::chiSquare(data));
        
        if (iocChange > 0.5 || chiChange > 100) {
            patternDetected = true;
            std::cout << "Significant change detected with " << config << ":\n";
            std::cout << "  IoC deviation: " << iocChange << "\n";
            std::cout << "  Chi-square deviation: " << chiChange << "\n\n";
        }
    }
    
    if (!patternDetected) {
        std::cout << "No significant patterns detected. The encryption appears robust against CA analysis.\n";
    } else {
        std::cout << "Patterns detected! The encryption may have weaknesses that CA reveals.\n";
        std::cout << "Consider examining the processed outputs for further cryptanalysis.\n";
    }
}

// ============================================================================
// Random Number Generators for Testing
// ============================================================================

class RandomNumberGenerator {
public:
    virtual ~RandomNumberGenerator() = default;
    virtual std::vector<uint8_t> generate(size_t length) = 0;
    virtual std::string getName() const = 0;
};

class LinearCongruentialGenerator : public RandomNumberGenerator {
private:
    uint64_t seed;
    static constexpr uint64_t a = 1664525;
    static constexpr uint64_t c = 1013904223;
    static constexpr uint64_t m = 4294967296;
    
public:
    LinearCongruentialGenerator(uint64_t seed = 12345) : seed(seed) {}
    
    std::vector<uint8_t> generate(size_t length) override {
        std::vector<uint8_t> data(length);
        for (size_t i = 0; i < length; ++i) {
            seed = (a * seed + c) % m;
            data[i] = static_cast<uint8_t>(seed & 0xFF);
        }
        return data;
    }
    
    std::string getName() const override { return "Linear Congruential Generator"; }
};

class MersenneTwisterGenerator : public RandomNumberGenerator {
private:
    std::mt19937 mt;
    
public:
    MersenneTwisterGenerator(uint32_t seed = 12345) : mt(seed) {}
    
    std::vector<uint8_t> generate(size_t length) override {
        std::vector<uint8_t> data(length);
        std::uniform_int_distribution<int> dist(0, 255);
        for (size_t i = 0; i < length; ++i) {
            data[i] = static_cast<uint8_t>(dist(mt));
        }
        return data;
    }
    
    std::string getName() const override { return "Mersenne Twister"; }
};

void testGenerators() {
    using namespace nist_sts;
    
    std::cout << "\n=== RANDOM NUMBER GENERATOR TESTING ===\n\n";
    
    std::vector<std::unique_ptr<RandomNumberGenerator>> generators;
    generators.push_back(std::make_unique<LinearCongruentialGenerator>());
    generators.push_back(std::make_unique<MersenneTwisterGenerator>());
    
    NISTTestSuite nist;
    
    for (const auto& gen : generators) {
        std::cout << "Testing: " << gen->getName() << "\n";
        std::cout << std::string(50, '-') << "\n";
        
        // Generate test data
        auto data = gen->generate(100000);  // 100KB of random data
        
        // Run NIST tests
        std::cout << nist.generateSummary(data) << "\n";
        
        // Show statistics
        std::cout << "Statistical Measures:\n";
        std::cout << "  Index of Coincidence: " << StatAnalyzer::indexOfCoincidence(data) << "\n";
        std::cout << "  Chi-Square: " << StatAnalyzer::chiSquare(data) << "\n";
        std::cout << "  Serial Correlation: " << StatAnalyzer::serialCorrelation(data) << "\n";
        std::cout << "  Entropy: " << StatAnalyzer::entropy(data) << " bits/byte\n\n";
    }
}

// ============================================================================
// Main Function
// ============================================================================

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            printUsage(argv[0]);
            return 1;
        }
        
        Options opts = parseArgs(argc, argv);
        
        // Special mode: test generators
        if (opts.inputFile == "--test-generators") {
            testGenerators();
            return 0;
        }
        
        // Check if input file was provided
        if (opts.inputFile.empty()) {
            std::cerr << "Error: No input file specified\n\n";
            printUsage(argv[0]);
            return 1;
        }
        
        // Load and analyze data
        std::vector<uint8_t> data = loadFile(opts.inputFile, opts.asciiMode);
        
        if (data.empty()) {
            std::cerr << "Error: No data loaded from file\n";
            return 1;
        }
        
        if (data.size() < 1000) {
            std::cerr << "Warning: Data size is very small (" << data.size() 
                     << " bytes). Results may not be statistically significant.\n\n";
        }
        
        analyzeData(data, opts);
        
        std::cout << "\nAnalysis complete.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

// ============================================================================
// Compilation Instructions:
// ============================================================================
// 
// For GCC/G++:
//   g++ -std=c++17 -O3 -march=native -o caca caca_main.cpp -lm -pthread
//
// For Clang:
//   clang++ -std=c++17 -O3 -march=native -o caca caca_main.cpp -lm -pthread
//
// For MSVC (Windows):
//   cl /std:c++17 /O2 /EHsc caca_main.cpp
//
// Optional flags:
//   -Wall -Wextra           : Enable all warnings
//   -g                      : Include debug symbols
//   -DNDEBUG               : Disable assertions for release
//   -fopenmp               : Enable OpenMP parallelization
//   -mavx2                 : Enable AVX2 instructions explicitly
//
// Example with all optimizations:
//   g++ -std=c++17 -O3 -march=native -mavx2 -fopenmp -DNDEBUG -o caca caca_main.cpp -lm -pthread
//
// ============================================================================