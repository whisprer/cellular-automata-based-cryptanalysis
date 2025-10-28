// overlapping_template_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"

namespace nist_sts {

class OverlappingTemplateTest : public StatisticalTest {
private:
    size_t blockLength;
    std::vector<bool> template_; // Using template_ to avoid keyword conflict
    
public:
    explicit OverlappingTemplateTest(size_t blockLength = 9);
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Overlapping Template Matching"; }
    
    void setBlockLength(size_t length) { blockLength = length; }
    size_t getBlockLength() const { return blockLength; }
    
    // By default, use template of all ones
    void setTemplate(const std::vector<bool>& templ) { template_ = templ; }
    
private:
    // Helper function to calculate probability
    double calculateProbability(int u, double eta);
};

} // namespace nist_sts