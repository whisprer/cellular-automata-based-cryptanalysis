// rank_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"

namespace nist_sts {

class RankTest : public StatisticalTest {
public:
    RankTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Rank"; }
    
private:
    // Helper function to compute rank of a binary matrix
    int computeRank(const std::vector<std::vector<bool>>& matrix);
    
    // Forward elimination step in Gaussian elimination
    void performForwardElimination(std::vector<std::vector<bool>>& matrix);
    
    // Backward elimination step in Gaussian elimination
    void performBackwardElimination(std::vector<std::vector<bool>>& matrix);
};

} // namespace nist_sts