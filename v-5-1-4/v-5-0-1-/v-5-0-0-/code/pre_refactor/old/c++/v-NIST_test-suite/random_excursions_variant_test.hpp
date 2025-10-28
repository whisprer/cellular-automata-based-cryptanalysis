// random_excursions_variant_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "../bitsequence.hpp"
#include <vector>

namespace nist_sts {

class RandomExcursionsVariantTest : public StatisticalTest {
public:
    RandomExcursionsVariantTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Random Excursions Variant"; }
};

} // namespace nist_sts