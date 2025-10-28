// rank_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"

namespace nist_sts {

class RankTest : public StatisticalTest {
public:
    RankTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Rank"; }
    
private:
    // Helper functions for matrix rank calculation
    int computeRank(const std::vector<std::vector<bool>>& matrix);
    void performForwardElimination(std::vector<std::vector<bool>>& matrix);
    void performBackwardElimination(std::vector<std::vector<bool>>& matrix);
};

} // namespace nist_sts