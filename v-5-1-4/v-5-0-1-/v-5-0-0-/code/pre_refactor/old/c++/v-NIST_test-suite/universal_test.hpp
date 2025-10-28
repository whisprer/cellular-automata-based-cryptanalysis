// universal_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class UniversalTest : public StatisticalTest {
public:
    UniversalTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Universal"; }
};

} // namespace nist_sts