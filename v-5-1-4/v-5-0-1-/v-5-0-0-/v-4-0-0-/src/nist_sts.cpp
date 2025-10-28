/*****************************************************
 * File: nist_sts.cpp
 *****************************************************/
#include "nist_sts.hpp"
#include "frequency_test.hpp"
#include "block_frequency_test.hpp"
#include "runs_test.hpp"
#include "dft_test.hpp"
#include "approximate_entropy_test.hpp"
#include "serial_test.hpp"
// ... plus any other test headers you need ...
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

namespace nist_sts {

std::vector<TestResult> NISTTestSuite::runFrequencyTests(const std::vector<uint8_t>& data) {
    std::vector<TestResult> results;
    // Frequency (Monobit)
    {
        FrequencyTest freqTest;
        results.push_back(freqTest.execute(data));
    }
    // Block Frequency
    {
        BlockFrequencyTest blockFreq(128);
        results.push_back(blockFreq.execute(data));
    }
    return results;
}

std::vector<TestResult> NISTTestSuite::runAllTests(const std::vector<uint8_t>& data) {
    std::vector<TestResult> results;

    // 1) Frequency group
    auto freqResults = runFrequencyTests(data);
    results.insert(results.end(), freqResults.begin(), freqResults.end());

    // 2) Example: runs test
    {
        RunsTest runsTest;
        results.push_back(runsTest.execute(data));
    }
    // 3) DFT test
    {
        DiscreteFourierTransformTest dftTest;
        results.push_back(dftTest.execute(data));
    }
    // 4) Approximate Entropy
    {
        ApproximateEntropyTest appEnt(10);
        results.push_back(appEnt.execute(data));
    }
    // 5) Serial test
    {
        SerialTest serialT(16);
        results.push_back(serialT.execute(data));
    }
    // ... add whichever other tests you want ...

    return results;
}

std::string NISTTestSuite::generateSummary(const std::vector<uint8_t>& data) {
    auto allResults = runAllTests(data);
    
    std::stringstream ss;
    ss << "NIST Statistical Test Suite Results\n"
       << "-----------------------------------\n"
       << "Data size: " << data.size() << " bytes (" << data.size()*8 << " bits)\n\n"
       << "Test Name                           p_value      Result\n"
       << "-------------------------------------------------------\n";

    for (auto& r : allResults) {
        ss << std::left << std::setw(35) << r.testName.substr(0,34)
           << " " << std::setw(10) << std::fixed << std::setprecision(6) << r.p_value
           << (r.success ? "  SUCCESS" : "  FAIL") << "\n";
    }
    return ss.str();
}

} // namespace nist_sts
