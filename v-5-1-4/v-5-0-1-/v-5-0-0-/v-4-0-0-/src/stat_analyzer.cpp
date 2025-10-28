/*****************************************************
 * File: stat_analyzer.cpp
 *****************************************************/
#include "stat_analyzer.hpp"
#include <cmath>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <map>

// For a more advanced pipeline, we might want to include "nist_sts.hpp" if needed.

double StatAnalyzer::indexOfCoincidence(const std::vector<uint8_t>& data) {
    if (data.size() < 2) return 0.0;
    std::vector<size_t> frequencies(256, 0);
    for (auto b : data) {
        frequencies[b]++;
    }
    double sum = 0.0;
    for (auto freq : frequencies) {
        sum += freq * (freq - 1);
    }
    return sum / (data.size() * (data.size() - 1));
}

double StatAnalyzer::chiSquare(const std::vector<uint8_t>& data) {
    if (data.empty()) return 0.0;
    double expected = double(data.size()) / 256.0;
    std::vector<size_t> frequencies(256, 0);
    for (auto b : data) {
        frequencies[b]++;
    }
    double chi = 0.0;
    for (auto freq : frequencies) {
        double diff = freq - expected;
        chi += (diff * diff) / expected;
    }
    return chi;
}

double StatAnalyzer::serialCorrelation(const std::vector<uint8_t>& data) {
    if (data.size() < 2) return 0.0;
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double num = 0.0, den1 = 0.0, den2 = 0.0;
    for (size_t i = 1; i < data.size(); i++) {
        double x = data[i - 1] - mean;
        double y = data[i] - mean;
        num  += x * y;
        den1 += x * x;
        den2 += y * y;
    }
    if (den1 == 0.0 || den2 == 0.0) return 0.0;
    return num / std::sqrt(den1 * den2);
}

void StatAnalyzer::byteFrequency(const std::vector<uint8_t>& data, bool showTopSpikes) {
    std::map<uint8_t, size_t> frequencies;
    for (auto b : data) {
        frequencies[b]++;
    }
    if (showTopSpikes) {
        std::vector<std::pair<uint8_t,size_t>> sorted(frequencies.begin(), frequencies.end());
        std::sort(sorted.begin(), sorted.end(), 
                  [](auto &a, auto &b) { return a.second > b.second; });
        std::cout << "Top 10 Most Frequent Bytes:\n";
        for (size_t i = 0; i < std::min<size_t>(10, sorted.size()); i++) {
            std::cout << "Byte 0x" << std::hex << (int)sorted[i].first
                      << ": " << std::dec << sorted[i].second << " times\n";
        }
    }
}

double StatAnalyzer::monobitTestPValue(const std::vector<uint8_t>& data) {
    if (data.empty()) return 0.0;
    size_t ones = 0;
    for (auto b : data) {
        ones += b;  // b is 0 or 1, presumably
    }
    double proportion = (double)ones / data.size();
    double s_obs = std::fabs(proportion - 0.5) * std::sqrt(data.size());
    // 2 * erfc(...) is often used, but let's keep it consistent
    // This next line might require #include <cmath> or <complex> for erfc
    return 2.0 * std::erfc(s_obs / std::sqrt(2.0));
}

void StatAnalyzer::displayStats(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << "Statistics for " << label << ":\n";
    std::cout << "  Size: " << data.size() << " bytes\n";
    std::cout << "  Index of Coincidence: " << indexOfCoincidence(data) << "\n";
    std::cout << "  Chi-Square:           " << chiSquare(data) << "\n";
    std::cout << "  Serial Correlation:   " << serialCorrelation(data) << "\n";
    std::cout << "  Monobit Test p-value: " << monobitTestPValue(data) << "\n";
}
