// dft_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"

namespace nist_sts {

class DiscreteFourierTransformTest : public StatisticalTest {
public:
    DiscreteFourierTransformTest() = default;
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Discrete Fourier Transform"; }
};

} // namespace nist_sts