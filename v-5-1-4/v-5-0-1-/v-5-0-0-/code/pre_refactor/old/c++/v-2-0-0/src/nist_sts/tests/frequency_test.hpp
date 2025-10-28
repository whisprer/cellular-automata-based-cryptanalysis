#pragma once
#include "statistical_test.hpp"  // Path needs to be correct relative to file location

namespace nist_sts {

class FrequencyTest : public StatisticalTest {
public:
    FrequencyTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Frequency"; }
};

} // namespace nist_sts