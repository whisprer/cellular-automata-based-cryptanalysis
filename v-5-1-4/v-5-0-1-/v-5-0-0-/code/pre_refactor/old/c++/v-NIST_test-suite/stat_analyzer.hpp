// stat_analyzer.hpp
#pragma once
#include <vector>
#include <string>

class StatAnalyzer {
public:
    // Original methods
    static void byteFrequency(const std::vector<uint8_t>& data, bool showTopSpikes = true);
    static double indexOfCoincidence(const std::vector<uint8_t>& data);
    static double chiSquare(const std::vector<uint8_t>& data);
    static double serialCorrelation(const std::vector<uint8_t>& data);
    static double monobitTestPValue(const std::vector<uint8_t>& data);
    static void displayStats(const std::string& label, const std::vector<uint8_t>& data);
    
    // New comparison methods
    static double correlationBetweenDatasets(const std::vector<uint8_t>& data1, 
                                           const std::vector<uint8_t>& data2);
    static double hammingDistanceBetweenDatasets(const std::vector<uint8_t>& data1, 
                                               const std::vector<uint8_t>& data2);

// Calculate correlation between two datasets
double StatAnalyzer::correlationBetweenDatasets(const std::vector<uint8_t>& data1, 
    const std::vector<uint8_t>& data2) {
if (data1.size() != data2.size() || data1.empty()) {
return 0.0;
}

// Calculate means
double mean1 = 0.0, mean2 = 0.0;
for (size_t i = 0; i < data1.size(); i++) {
mean1 += data1[i];
mean2 += data2[i];
}
mean1 /= data1.size();
mean2 /= data2.size();

// Calculate correlation
double num = 0.0, den1 = 0.0, den2 = 0.0;
for (size_t i = 0; i < data1.size(); i++) {
double x = data1[i] - mean1;
double y = data2[i] - mean2;
num += (x * y);
den1 += (x * x);
den2 += (y * y);
}

if (den1 == 0.0 || den2 == 0.0) {
return 0.0;
}

return num / std::sqrt(den1 * den2);
}

// Calculate normalized Hamming distance (as a percentage of differing bits)
double StatAnalyzer::hammingDistanceBetweenDatasets(const std::vector<uint8_t>& data1, 
        const std::vector<uint8_t>& data2) {
if (data1.size() != data2.size() || data1.empty()) {
return 1.0; // Maximum distance for incomparable data
}

size_t totalBits = data1.size() * 8;
size_t differentBits = 0;

for (size_t i = 0; i < data1.size(); i++) {
uint8_t diff = data1[i] ^ data2[i]; // XOR shows bit differences

// Count bits set in diff (popcount)
for (int j = 0; j < 8; j++) {
if ((diff >> j) & 1) {
differentBits++;
}
}
}

return static_cast<double>(differentBits) / static_cast<double>(totalBits);
}

};