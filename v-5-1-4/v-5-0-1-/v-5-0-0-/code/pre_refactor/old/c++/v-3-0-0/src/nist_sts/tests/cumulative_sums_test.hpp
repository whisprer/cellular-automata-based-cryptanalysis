// cumulative_sums_test.hpp - Fixed version
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"

namespace nist_sts {

class CumulativeSumsTest : public StatisticalTest {
public:
    CumulativeSumsTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Cumulative Sums"; }

private:
    // Add missing method declarations
    double calculatePValue(size_t n, int z);
    double normal(double x);

};

} // namespace nist_sts