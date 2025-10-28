// random_excursions_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"
#include <vector>

namespace nist_sts {

class RandomExcursionsTest : public StatisticalTest {
public:
    RandomExcursionsTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Random Excursions"; }
    
private:
    std::vector<int> findCycles(const std::vector<int>& S);
};

} // namespace nist_sts