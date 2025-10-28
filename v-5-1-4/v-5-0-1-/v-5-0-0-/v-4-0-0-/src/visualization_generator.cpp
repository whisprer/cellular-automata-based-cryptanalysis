/*****************************************************
 * File: visualization_generator.cpp
 *****************************************************/
#include "visualization_generator.hpp"
#include <fstream>
#include <stdexcept>
#include <iomanip>

namespace visualization {

void VisualizationGenerator::addDataset(const std::string& name,
                                        const std::vector<nist_sts::TestResult>& results,
                                        const std::map<std::string, double>& stats)
{
    Dataset ds;
    ds.name    = name;
    ds.results = results;
    ds.stats   = stats;
    datasets.push_back(ds);
}

void VisualizationGenerator::generateDashboard(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("Error opening output file: " + filename);
    }

    outFile << "<html><head><title>CA Cryptanalysis Results</title></head><body>\n";
    outFile << "<h1>Visualization Dashboard</h1>\n";

    for (auto& ds : datasets) {
        outFile << "<h2>Dataset: " << ds.name << "</h2>\n";
        outFile << "<p>Statistics:</p><ul>\n";
        for (auto& kv : ds.stats) {
            outFile << "<li>" << kv.first << " : " << kv.second << "</li>\n";
        }
        outFile << "</ul>\n";
        outFile << "<table border='1' cellspacing='0' cellpadding='5'>\n";
        outFile << "<tr><th>Test Name</th><th>p-value</th><th>Success?</th></tr>\n";
        for (auto& r : ds.results) {
            outFile << "<tr>"
                    << "<td>" << r.testName << "</td>"
                    << "<td>" << r.p_value << "</td>"
                    << "<td>" << (r.success ? "Yes" : "No") << "</td>"
                    << "</tr>\n";
        }
        outFile << "</table>\n";
    }

    outFile << "</body></html>\n";
}

} // namespace visualization
