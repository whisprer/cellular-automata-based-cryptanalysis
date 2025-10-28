// block_frequency_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class BlockFrequencyTest : public StatisticalTest {
private:
    size_t blockLength;
    
public:
    explicit BlockFrequencyTest(size_t blockLength = 128);
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Block Frequency"; }
    
    void setBlockLength(size_t length) { blockLength = length; }
    size_t getBlockLength() const { return blockLength; }
};

} // namespace nist_sts