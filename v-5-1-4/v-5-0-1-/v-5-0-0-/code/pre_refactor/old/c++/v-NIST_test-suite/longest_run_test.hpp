// longest_run_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class LongestRunTest : public StatisticalTest {
public:
    LongestRunTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Longest Run of Ones"; }
};

} // namespace nist_sts