// nist_tests.hpp
#pragma once
#include "statistical_test.hpp"
#include <vector>
#include <memory>
#include <cmath>

namespace nist_sts {

// Frequency (Monobit) Test
class FrequencyTest : public StatisticalTest {
public:
    FrequencyTest() = default;
    
    TestResult execute(const std::vector<uint8_t>& data) override {
        TestResult result;
        result.testName = getName();
        
        // Count total bits and ones
        long long totalBits = data.size() * 8LL;
        long long onesCount = 0;
        
        for (auto byte : data) {
            for (int bit = 0; bit < 8; bit++) {
                if ((byte >> bit) & 1) {
                    onesCount++;
                }
            }
        }
        
        // Calculate test statistic
        double proportion = static_cast<double>(onesCount) / static_cast<double>(totalBits);
        double s_obs = std::abs(proportion - 0.5) * std::sqrt(totalBits * 2.0);
        double p_value = std::erfc(s_obs / std::sqrt(2.0));
        
        // Store results
        result.p_value = p_value;
        result.success = isSuccess(p_value);
        result.statistics["proportion"] = proportion;
        result.statistics["ones_count"] = onesCount;
        result.statistics["total_bits"] = totalBits;
        
        return result;
    }
    
    std::string getName() const override {
        return "Frequency (Monobit) Test";
    }
};

// Runs Test
class RunsTest : public StatisticalTest {
public:
    RunsTest() = default;
    
    TestResult execute(const std::vector<uint8_t>& data) override {
        TestResult result;
        result.testName = getName();
        
        // Convert to bits and count runs
        std::vector<bool> bits;
        bits.reserve(data.size() * 8);
        
        for (auto byte : data) {
            for (int i = 7; i >= 0; i--) {
                bits.push_back((byte >> i) & 1);
            }
        }
        
        // Count ones for proportion
        size_t ones = 0;
        for (bool bit : bits) {
            if (bit) ones++;
        }
        double pi = static_cast<double>(ones) / static_cast<double>(bits.size());
        
        // Pre-test: check if proportion is suitable
        if (std::abs(pi - 0.5) > (2.0 / std::sqrt(bits.size()))) {
            result.p_value = 0.0;
            result.success = false;
            result.statistics["proportion"] = pi;
            result.statistics["error"] = 1.0;
            return result;
        }
        
        // Count runs
        size_t runs = 1; // Start with 1 run
        for (size_t i = 1; i < bits.size(); i++) {
            if (bits[i] != bits[i-1]) {
                runs++;
            }
        }
        
        // Calculate test statistic
        double r_obs = static_cast<double>(runs);
        double mean = 2.0 * bits.size() * pi * (1.0 - pi);
        double std_dev = std::sqrt(2.0 * bits.size() * pi * (1.0 - pi));
        double z = std::abs(r_obs - mean) / std_dev;
        double p_value = std::erfc(z / std::sqrt(2.0));
        
        // Store results
        result.p_value = p_value;
        result.success = isSuccess(p_value);
        result.statistics["proportion"] = pi;
        result.statistics["runs"] = runs;
        result.statistics["expected_runs"] = mean;
        
        return result;
    }
    
    std::string getName() const override {
        return "Runs Test";
    }
};

// Block Frequency Test
class BlockFrequencyTest : public StatisticalTest {
public:
    BlockFrequencyTest(size_t blockSize = 128) : blockSize(blockSize) {}
    
    TestResult execute(const std::vector<uint8_t>& data) override {
        TestResult result;
        result.testName = getName();
        
        // Check if we have enough data
        if (data.size() * 8 < blockSize) {
            result.p_value = 0.0;
            result.success = false;
            result.statistics["error"] = 1.0;
            return result;
        }
        
        // Convert to bits
        std::vector<bool> bits;
        bits.reserve(data.size() * 8);
        
        for (auto byte : data) {
            for (int i = 7; i >= 0; i--) {
                bits.push_back((byte >> i) & 1);
            }
        }
        
        // Number of complete blocks
        size_t numBlocks = bits.size() / blockSize;
        
        // Process each block
        double chi_squared = 0.0;
        for (size_t block = 0; block < numBlocks; block++) {
            // Count ones in this block
            size_t ones = 0;
            for (size_t i = 0; i < blockSize; i++) {
                if (bits[block * blockSize + i]) {
                    ones++;
                }
            }
            
            // Calculate proportion and contribution to chi-squared
            double pi = static_cast<double>(ones) / static_cast<double>(blockSize);
            double v = pi - 0.5;
            chi_squared += 4.0 * blockSize * v * v;
        }
        
        // Calculate p-value
        double p_value = std::exp(-chi_squared / 2.0); // Simplified approximation
        
        // Store results
        result.p_value = p_value;
        result.success = isSuccess(p_value);
        result.statistics["chi_squared"] = chi_squared;
        result.statistics["num_blocks"] = numBlocks;
        
        return result;
    }
    
    std::string getName() const override {
        return "Block Frequency Test";
    }
    
private:
    size_t blockSize;
};

// DFT (Spectral) Test
class DFTTest : public StatisticalTest {
public:
    DFTTest() = default;
    
    TestResult execute(const std::vector<uint8_t>& data) override {
        TestResult result;
        result.testName = getName();
        
        // Convert to +1/-1 series
        std::vector<double> X;
        X.reserve(data.size() * 8);
        
        for (auto byte : data) {
            for (int i = 7; i >= 0; i--) {
                X.push_back(((byte >> i) & 1) ? 1.0 : -1.0);
            }
        }
        
        // Perform DFT (simple implementation)
        size_t n = X.size();
        std::vector<double> magnitudes(n/2);
        
        for (size_t k = 0; k < n/2; k++) {
            double real = 0.0, imag = 0.0;
            for (size_t i = 0; i < n; i++) {
                double angle = -2.0 * M_PI * k * i / n;
                real += X[i] * std::cos(angle);
                imag += X[i] * std::sin(angle);
            }
            magnitudes[k] = std::sqrt(real*real + imag*imag);
        }
        
        // Calculate threshold
        double T = std::sqrt(2.995732274 * n);
        
        // Count peaks below threshold
        size_t N0 = 0;
        for (auto m : magnitudes) {
            if (m < T) N0++;
        }
        
        // Calculate statistics
        double N_obs = static_cast<double>(N0);
        double N_exp = 0.95 * n / 2.0;
        double d = (N_obs - N_exp) / std::sqrt(n * 0.95 * 0.05 / 4.0);
        double p_value = std::erfc(std::abs(d) / std::sqrt(2.0));
        
        // Store results
        result.p_value = p_value;
        result.success = isSuccess(p_value);
        result.statistics["below_threshold"] = N0;
        result.statistics["expected_below"] = N_exp;
        
        return result;
    }
    
    std::string getName() const override {
        return "DFT (Spectral) Test";
    }
};

// NIST Test Suite - simplified version
class NISTTestSuite {
private:
    std::vector<std::unique_ptr<StatisticalTest>> tests;
    
public:
    NISTTestSuite() {
        // Add the basic tests
        tests.push_back(std::make_unique<FrequencyTest>());
        tests.push_back(std::make_unique<RunsTest>());
        tests.push_back(std::make_unique<BlockFrequencyTest>());
        tests.push_back(std::make_unique<DFTTest>());
        // Add more tests as needed
    }
    
    // Run all tests and return results
    std::vector<TestResult> runTests(const std::vector<uint8_t>& data) {
        std::vector<TestResult> results;
        results.reserve(tests.size());
        
        for (const auto& test : tests) {
            results.push_back(test->execute(data));
        }
        
        return results;
    }
    
    // Get a map of test names to p-values
    std::map<std::string, double> getPValues(const std::vector<uint8_t>& data) {
        std::map<std::string, double> pValues;
        
        for (const auto& test : tests) {
            auto result = test->execute(data);
            pValues[result.testName] = result.p_value;
        }
        
        return pValues;
    }
    
    // Generate a summary report
    std::string generateSummary(const std::vector<uint8_t>& data) {
        auto results = runTests(data);
        std::stringstream ss;
        
        ss << "NIST Statistical Test Suite Results\n";
        ss << "-----------------------------------\n";
        ss << "Data size: " << data.size() << " bytes (" << data.size() * 8 << " bits)\n\n";
        
        ss << "Test Name                         P-Value    Result\n";
        ss << "---------------------------------------------------\n";
        
        for (const auto& result : results) {
            ss << std::left << std::setw(35) << result.testName.substr(0, 34) 
               << std::fixed << std::setprecision(6) << std::setw(10) << result.p_value
               << (result.success ? "SUCCESS" : "FAILURE") << "\n";
        }
        
        return ss.str();
    }
};

} // namespace nist_sts