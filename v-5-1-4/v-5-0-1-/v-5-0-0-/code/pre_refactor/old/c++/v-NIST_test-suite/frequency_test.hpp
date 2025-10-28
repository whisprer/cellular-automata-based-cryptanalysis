// frequency_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class FrequencyTest : public StatisticalTest {
public:
    FrequencyTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Frequency"; }
};

} // namespace nist_sts