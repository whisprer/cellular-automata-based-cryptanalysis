// stat_analyzer.hpp
#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <map>
#include <iostream>

class StatAnalyzer {
public:
    // Index of Coincidence
    static double indexOfCoincidence(const std::vector<uint8_t>& data) {
        if (data.empty()) return 0.0;

        // Count frequencies of each byte
        std::vector<size_t> frequencies(256, 0);
        for (auto byte : data) {
            frequencies[byte]++;
        }

        // Calculate index of coincidence
        double sum = 0.0;
        for (auto freq : frequencies) {
            sum += freq * (freq - 1);
        }

        // Total combinations divided by total possible combinations
        return sum / (data.size() * (data.size() - 1));
    }

    // Chi-Square Statistic
    static double chiSquare(const std::vector<uint8_t>& data) {
        if (data.empty()) return 0.0;

        // Calculate expected frequency
        double expectedFreq = data.size() / 256.0;

        // Count frequencies of each byte
        std::vector<size_t> frequencies(256, 0);
        for (auto byte : data) {
            frequencies[byte]++;
        }

        // Calculate chi-square statistic
        double chiSquare = 0.0;
        for (auto freq : frequencies) {
            double diff = freq - expectedFreq;
            chiSquare += (diff * diff) / expectedFreq;
        }

        return chiSquare;
    }

    // Serial Correlation
    static double serialCorrelation(const std::vector<uint8_t>& data) {
        if (data.size() < 2) return 0.0;

        // Calculate means
        double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

        // Calculate correlation
        double num = 0.0, den1 = 0.0, den2 = 0.0;
        for (size_t i = 1; i < data.size(); ++i) {
            double x = data[i-1] - mean;
            double y = data[i] - mean;
            num += x * y;
            den1 += x * x;
            den2 += y * y;
        }

        // Avoid division by zero
        if (den1 == 0.0 || den2 == 0.0) return 0.0;

        return num / std::sqrt(den1 * den2);
    }

    // Byte Frequency
    static void byteFrequency(const std::vector<uint8_t>& data, bool showTopSpikes = true) {
        // Count frequency of each byte value
        std::map<uint8_t, size_t> frequencies;
        for (auto byte : data) {
            frequencies[byte]++;
        }

        // If showing top spikes, sort and display most frequent bytes
        if (showTopSpikes) {
            std::vector<std::pair<uint8_t, size_t>> sortedFreq(frequencies.begin(), frequencies.end());
            std::sort(sortedFreq.begin(), sortedFreq.end(), 
                [](const auto& a, const auto& b) { return a.second > b.second; });

            std::cout << "Top 10 Most Frequent Bytes:\n";
            for (size_t i = 0; i < std::min(size_t(10), sortedFreq.size()); ++i) {
                std::cout << "Byte 0x" << std::hex << int(sortedFreq[i].first) 
                          << ": " << std::dec << sortedFreq[i].second << " times\n";
            }
        }
    }

    // Monobit Test p-value
    static double monobitTestPValue(const std::vector<uint8_t>& data) {
        if (data.empty()) return 0.0;

        // Count ones
        size_t ones = 0;
        for (auto byte : data) {
            ones += byte;
        }

        // Calculate proportion of ones
        double proportion = static_cast<double>(ones) / data.size();
        
        // Calculate test statistic
        double s_obs = std::abs(proportion - 0.5) * std::sqrt(data.size());
        
        // Calculate p-value (using complementary error function)
        return 2.0 * std::erfc(s_obs / std::sqrt(2.0));
    }

    // Display Statistics
    static void displayStats(const std::string& label, const std::vector<uint8_t>& data) {
        std::cout << "Statistics for " << label << ":\n";
        std::cout << "  Size: " << data.size() << " bytes\n";
        std::cout << "  Index of Coincidence: " << indexOfCoincidence(data) << "\n";
        std::cout << "  Chi-Square: " << chiSquare(data) << "\n";
        std::cout << "  Serial Correlation: " << serialCorrelation(data) << "\n";
        std::cout << "  Monobit Test p-value: " << monobitTestPValue(data) << "\n";
    }
};