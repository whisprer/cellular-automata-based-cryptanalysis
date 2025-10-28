// visualization_generator.cpp
#include "visualization_generator.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace visualization {

VisualizationGenerator::VisualizationGenerator(const std::string& outputDirectory)
    : outputDir(outputDirectory) {
    // Create output directory if it doesn't exist
    std::filesystem::create_directories(outputDirectory);
}

void VisualizationGenerator::addDataset(const std::string& name, const nist_sts::TestResults& results, 
                const std::map<std::string, double>& metadata) {
    TestDataset dataset;
    dataset.name = name;
    dataset.results = results.test_results;
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
    
    // Generate dashboard HTML
    file << generateDashboardHTML(title);
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
    
    file << generatePValueHeatmapHTML();
    file.close();
    
    std::cout << "P-value heatmap generated at: " << filename << std::endl;
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
    
    file << generateRadarChartHTML(datasetIndex);
    file.close();
    
    std::cout << "Radar chart generated at: " << filename << std::endl;
}

void VisualizationGenerator::generateComparativeBarChart() {
    std::string filename = outputDir + "/comparative_chart.html";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create comparative chart file: " << filename << std::endl;
        return;
    }
    
    file << generateComparativeBarChartHTML();
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
    
    file << generateDistributionHistogramHTML();
    file.close();
    
    std::cout << "Distribution histogram generated at: " << filename << std::endl;
}

// Implementation of HTML generation methods from the header file
// These are already included in the visualization_generator.hpp artifact

} // namespace visualization