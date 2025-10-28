/*****************************************************
 * File: visualization_generator.hpp
 *****************************************************/
#pragma once

#include <string>
#include <vector>
#include <map>
#include "nist_sts.hpp"

namespace visualization {

class VisualizationGenerator {
public:
    VisualizationGenerator() = default;
    explicit VisualizationGenerator(const std::string& outDir) : outputDir(outDir) {}

    void addDataset(const std::string& name,
                    const std::vector<nist_sts::TestResult>& results,
                    const std::map<std::string, double>& stats);

    void generateDashboard(const std::string& filename);

private:
    std::string outputDir;
    // We'll store everything in a single container
    struct Dataset {
        std::string name;
        std::vector<nist_sts::TestResult> results;
        std::map<std::string, double> stats;
    };
    std::vector<Dataset> datasets;
};

} // namespace visualization
