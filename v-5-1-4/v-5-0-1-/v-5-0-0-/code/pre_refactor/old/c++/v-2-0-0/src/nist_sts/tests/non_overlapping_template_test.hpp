// non_overlapping_template_test.hpp
#pragma once
#include "statistical_test.hpp"
#include "bitsequence.hpp"
#include <vector>
#include <string>

namespace nist_sts {

class NonOverlappingTemplateTest : public StatisticalTest {
private:
    size_t blockLength;
    std::vector<std::vector<bool>> templates;
    
public:
    explicit NonOverlappingTemplateTest(size_t blockLength = 9);
    TestResult execute(const BitSequence& data) override;
    std::string getName() const override { return "Non-overlapping Template Matching"; }
    
    void setBlockLength(size_t length) { blockLength = length; }
    size_t getBlockLength() const { return blockLength; }
    
    // Load templates from file or generate them
    void loadTemplates(const std::string& filename);
    void generateTemplates();
    
private:
    // Helper function to check if a template is aperiodic
    bool isAperiodic(const std::vector<bool>& templ);
};

} // namespace nist_sts