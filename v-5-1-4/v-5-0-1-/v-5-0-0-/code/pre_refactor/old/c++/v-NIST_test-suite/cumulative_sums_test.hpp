// cumulative_sums_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class CumulativeSumsTest : public StatisticalTest {
public:
    CumulativeSumsTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Cumulative Sums"; }
};

} // namespace nist_sts