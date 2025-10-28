// serial_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class SerialTest : public StatisticalTest {
private:
    size_t blockLength;
    
public:
    explicit SerialTest(size_t blockLength = 16);
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Serial"; }
    
    void setBlockLength(size_t length) { blockLength = length; }
    size_t getBlockLength() const { return blockLength; }
    
private:
    double psi2(const BitSequence& data, size_t m);
};

} // namespace nist_sts