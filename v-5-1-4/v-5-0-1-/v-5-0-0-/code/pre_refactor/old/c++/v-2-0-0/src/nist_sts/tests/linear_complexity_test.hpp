// linear_complexity_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"

namespace nist_sts {

class LinearComplexityTest : public StatisticalTest {
private:
    size_t sequenceLength;
    
public:
    explicit LinearComplexityTest(size_t sequenceLength = 500);
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Linear Complexity"; }
    
    void setSequenceLength(size_t length) { sequenceLength = length; }
    size_t getSequenceLength() const { return sequenceLength; }
    
private:
    // Berlekamp-Massey algorithm to find linear complexity
    int berlekampMassey(const std::vector<bool>& sequence);
};

} // namespace nist_sts