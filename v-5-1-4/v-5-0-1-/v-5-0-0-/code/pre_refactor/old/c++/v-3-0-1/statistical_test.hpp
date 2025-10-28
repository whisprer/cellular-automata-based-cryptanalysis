#ifndef STAT_ANALYZER_HPP
#define STAT_ANALYZER_HPP

#include <vector>
#include <map>
#include <string>
#include "nist_sts.hpp"

class StatAnalyzer {
public:
    static std::map<std::string, double> calculateAllStats(const std::vector<uint8_t>& data);
    static std::vector<nist_sts::TestResult> runAllNISTTests(const std::vector<uint8_t>& data);

private:
    static double calculateEntropy(const std::vector<uint8_t>& data);
    static double calculateChiSquare(const std::vector<uint8_t>& data);
    static double calculateSerialCorrelation(const std::vector<uint8_t>& data);
    static double calculateIndexOfCoincidence(const std::vector<uint8_t>& data);
    static std::map<uint8_t, int> calculateFrequency(const std::vector<uint8_t>& data);
};

#endif
