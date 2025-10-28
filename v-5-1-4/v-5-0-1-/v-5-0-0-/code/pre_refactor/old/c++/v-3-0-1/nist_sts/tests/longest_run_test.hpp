// longest_run_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"

namespace nist_sts {

class LongestRunTest : public StatisticalTest {
private:
    int blockLength;  // Added blockLength member
    
public:
    LongestRunTest();  // Constructor
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Longest Run of Ones"; }
};

} // namespace nist_sts