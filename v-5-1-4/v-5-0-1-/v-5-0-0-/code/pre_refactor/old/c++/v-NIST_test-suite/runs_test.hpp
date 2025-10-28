// runs_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class RunsTest : public StatisticalTest {
public:
    RunsTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Runs"; }
};

} // namespace nist_sts