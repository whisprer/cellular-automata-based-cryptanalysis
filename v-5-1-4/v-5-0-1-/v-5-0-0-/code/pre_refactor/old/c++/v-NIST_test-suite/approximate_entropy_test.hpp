// approximate_entropy_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class ApproximateEntropyTest : public StatisticalTest {
private:
    size_t blockLength;
    
public:
    explicit ApproximateEntropyTest(size_t blockLength = 10);
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Approximate Entropy"; }
    
    void setBlockLength(size_t length) { blockLength = length; }
    size_t getBlockLength() const { return blockLength; }
    
private:
    double calculatePhi(const BitSequence& data, size_t m);
};

} // namespace nist_sts