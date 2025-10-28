// visualization_generator.cpp
#include "visualization_generator.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace visualization {

VisualizationGenerator::VisualizationGenerator(const std::string& outputDirectory)
    : outputDir(outputDirectory) {
    // Create output directory if it doesn't exist
    std::filesystem::create_directories(outputDirectory);
}

void VisualizationGenerator::addDataset(const std::string& name, 
                const nist_sts::TestResults& results, 
                const std::map<std::string, double>& metadata) {
    TestDataset dataset;
    dataset.name = name;
    dataset.results = results.test_results;
    dataset.metadata = metadata;
    datasets.push_back(dataset);
}

void VisualizationGenerator::addDataset(const std::string& name, 
                const std::map<std::string, double>& pValues,
                const std::map<std::string, double>& metadata) {
    TestDataset dataset;
    dataset.name = name;
    
    // Convert p-values to test results
    for (const auto& pair : pValues) {
        nist_sts::TestResult result;
        result.testName = pair.first;
        result.p_value = pair.second;
        result.success = (pair.second >= 0.01); // Standard alpha value
        dataset.results.push_back(result);
    }
    
    dataset.metadata = metadata;
    datasets.push_back(dataset);
}

void VisualizationGenerator::generateDashboard(const std::string& title) {
    std::string filename = outputDir + "/dashboard.html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create dashboard file: " << filename << std::endl;
        return;
    }
    
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>" << title << "</title>\n"
         << "    <style>\n"
         << "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
         << "        table { border-collapse: collapse; width: 100%; }\n"
         << "        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n"
         << "        th { background-color: #f2f2f2; }\n"
         << "        .pass { color: green; }\n"
         << "        .fail { color: red; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>" << title << "</h1>\n"
         << "    <h2>Test Results</h2>\n"
         << "    <table>\n"
         << "        <tr>\n"
         << "            <th>Test Name</th>\n";
    
    // Add column headers for each dataset
    for (const auto& dataset : datasets) {
        file << "            <th>" << dataset.name << " (p-value)</th>\n";
    }
    
    file << "        </tr>\n";
    
    // Get all unique test names
    std::set<std::string> testNames;
    for (const auto& dataset : datasets) {
        for (const auto& result : dataset.results) {
            testNames.insert(result.testName);
        }
    }
    
    // Add rows for each test
    for (const auto& testName : testNames) {
        file << "        <tr>\n"
             << "            <td>" << testName << "</td>\n";
        
        // Add p-values for each dataset
        for (const auto& dataset : datasets) {
            bool found = false;
            for (const auto& result : dataset.results) {
                if (result.testName == testName) {
                    std::string className = result.success ? "pass" : "fail";
                    file << "            <td class=\"" << className << "\">" 
                         << std::fixed << std::setprecision(6) << result.p_value << "</td>\n";
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                file << "            <td>N/A</td>\n";
            }
        }
        
        file << "        </tr>\n";
    }
    
    file << "    </table>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    std::cout << "Dashboard generated at: " << filename << std::endl;
}

void VisualizationGenerator::generatePValueHeatmap() {
    std::string filename = outputDir + "/pvalue_heatmap.html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create heatmap file: " << filename << std::endl;
        return;
    }
    
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>P-Value Heatmap</title>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>P-Value Heatmap</h1>\n"
         << "    <p>Simplified visualization - upgrade later</p>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    std::cout << "P-value heatmap generated at: " << filename << std::endl;
}

void VisualizationGenerator::generateComparativeBarChart() {
    std::string filename = outputDir + "/comparative_chart.html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create comparative chart file: " << filename << std::endl;
        return;
    }
    
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Comparative Bar Chart</title>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Comparative Bar Chart</h1>\n"
         << "    <p>Simplified visualization - upgrade later</p>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    std::cout << "Comparative chart generated at: " << filename << std::endl;
}

void VisualizationGenerator::generateDistributionHistogram() {
    std::string filename = outputDir + "/distribution_histogram.html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create histogram file: " << filename << std::endl;
        return;
    }
    
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Distribution Histogram</title>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Distribution Histogram</h1>\n"
         << "    <p>Simplified visualization - upgrade later</p>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    std::cout << "Distribution histogram generated at: " << filename << std::endl;
}

void VisualizationGenerator::generateRadarChart(size_t datasetIndex) {
    if (datasetIndex >= datasets.size()) {
        std::cerr << "Error: Dataset index out of range" << std::endl;
        return;
    }
    
    std::string filename = outputDir + "/radar_chart_" + 
                           std::to_string(datasetIndex) + ".html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create radar chart file: " << filename << std::endl;
        return;
    }
    
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Radar Chart</title>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Radar Chart for " << datasets[datasetIndex].name << "</h1>\n"
         << "    <p>Simplified visualization - upgrade later</p>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    std::cout << "Radar chart generated at: " << filename << std::endl;
}

} // namespace visualization