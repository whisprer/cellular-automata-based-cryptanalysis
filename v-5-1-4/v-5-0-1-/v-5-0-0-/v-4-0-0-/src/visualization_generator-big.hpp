// visualization_generator.hpp
#pragma once
#include "common.hpp"
#include "test_suite.hpp"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace visualization {

class VisualizationGenerator {
private:
    // Store data from multiple test runs for comparison
    struct TestDataset {
        std::string name;
        std::vector<nist_sts::TestResult> results;
        std::map<std::string, double> metadata;
    };
    
    std::vector<TestDataset> datasets;
    std::string outputDir;

public:
    VisualizationGenerator(const std::string& outputDirectory = "visualizations") 
        : outputDir(outputDirectory) {
        // Create output directory if it doesn't exist
        std::filesystem::create_directories(outputDirectory);
    }

    // Add a dataset from test results
    void addDataset(const std::string& name, const nist_sts::TestResults& results, 
                    const std::map<std::string, double>& metadata = {}) {
        TestDataset dataset;
        dataset.name = name;
        dataset.results = results.test_results;
        dataset.metadata = metadata;
        datasets.push_back(dataset);
    }

    // Generate a dashboard with all visualizations
    void generateDashboard(const std::string& title = "CACA Statistical Analysis Dashboard") {
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

    // Generate heatmap of p-values for all tests
    void generatePValueHeatmap() {
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

    // Generate radar chart for a specific dataset
    void generateRadarChart(size_t datasetIndex = 0) {
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

    // Generate comparative bar chart for all datasets
    void generateComparativeBarChart() {
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

    // Generate a distribution histogram for frequency-related data
    void generateDistributionHistogram() {
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

private:
    // HTML generation functions
    std::string generateDashboardHTML(const std::string& title);
    std::string generatePValueHeatmapHTML();
    std::string generateRadarChartHTML(size_t datasetIndex);
    std::string generateComparativeBarChartHTML();
    std::string generateDistributionHistogramHTML();
    
    // Helper functions for data formatting
    std::string formatDataForCharts();
    std::string getTestNames();
    std::string getDatasetNames();
};

// Implementation of HTML generation methods

std::string VisualizationGenerator::generateDashboardHTML(const std::string& title) {
    std::stringstream ss;
    
    ss << "<!DOCTYPE html>\n"
       << "<html lang=\"en\">\n"
       << "<head>\n"
       << "    <meta charset=\"UTF-8\">\n"
       << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
       << "    <title>" << title << "</title>\n"
       << "    <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n"
       << "    <script src=\"https://cdn.jsdelivr.net/npm/chartjs-plugin-datalabels\"></script>\n"
       << "    <style>\n"
       << "        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n"
       << "        .dashboard { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; }\n"
       << "        .card { background: white; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); padding: 20px; }\n"
       << "        .full-width { grid-column: 1 / span 2; }\n"
       << "        h1, h2 { color: #333; }\n"
       << "        .chart-container { position: relative; height: 350px; margin-top: 20px; }\n"
       << "        table { width: 100%; border-collapse: collapse; margin-top: 20px; }\n"
       << "        th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n"
       << "        th { background-color: #f2f2f2; }\n"
       << "        tr:hover { background-color: #f5f5f5; }\n"
       << "        .pass { color: green; }\n"
       << "        .fail { color: red; }\n"
       << "    </style>\n"
       << "</head>\n"
       << "<body>\n"
       << "    <h1>" << title << "</h1>\n"
       << "    <div class=\"dashboard\">\n"
       << "        <div class=\"card full-width\">\n"
       << "            <h2>NIST Test Suite Results Overview</h2>\n"
       << "            <div class=\"chart-container\">\n"
       << "                <canvas id=\"pvalueChart\"></canvas>\n"
       << "            </div>\n"
       << "        </div>\n"
       << "        <div class=\"card\">\n"
       << "            <h2>P-value Distribution</h2>\n"
       << "            <div class=\"chart-container\">\n"
       << "                <canvas id=\"pvalueDistribution\"></canvas>\n"
       << "            </div>\n"
       << "        </div>\n"
       << "        <div class=\"card\">\n"
       << "            <h2>Test Success Rate</h2>\n"
       << "            <div class=\"chart-container\">\n"
       << "                <canvas id=\"successRate\"></canvas>\n"
       << "            </div>\n"
       << "        </div>\n"
       << "        <div class=\"card full-width\">\n"
       << "            <h2>Detailed Results</h2>\n"
       << "            <table id=\"resultsTable\">\n"
       << "                <thead>\n"
       << "                    <tr>\n"
       << "                        <th>Test</th>\n";

    // Add column headers for each dataset
    for (const auto& dataset : datasets) {
        ss << "                        <th>" << dataset.name << " (p-value)</th>\n";
    }
    
    ss << "                    </tr>\n"
       << "                </thead>\n"
       << "                <tbody>\n";

    // Get all unique test names across all datasets
    std::set<std::string> testNames;
    for (const auto& dataset : datasets) {
        for (const auto& result : dataset.results) {
            testNames.insert(result.testName);
        }
    }
    
    // Add rows for each test
    for (const auto& testName : testNames) {
        ss << "                    <tr>\n"
           << "                        <td>" << testName << "</td>\n";
        
        // Add p-values for each dataset
        for (const auto& dataset : datasets) {
            bool found = false;
            for (const auto& result : dataset.results) {
                if (result.testName == testName) {
                    std::string className = result.success ? "pass" : "fail";
                    ss << "                        <td class=\"" << className << "\">" 
                       << std::fixed << std::setprecision(6) << result.p_value << "</td>\n";
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                ss << "                        <td>N/A</td>\n";
            }
        }
        
        ss << "                    </tr>\n";
    }
    
    ss << "                </tbody>\n"
       << "            </table>\n"
       << "        </div>\n"
       << "    </div>\n"
       << "    <script>\n"
       << "        // P-value Bar Chart\n"
       << "        const pvalueCtx = document.getElementById('pvalueChart').getContext('2d');\n"
       << "        const pvalueData = {\n"
       << "            labels: [";
    
    // Add test names
    bool first = true;
    for (const auto& testName : testNames) {
        if (!first) ss << ", ";
        ss << "'" << testName << "'";
        first = false;
    }
    
    ss << "],\n"
       << "            datasets: [\n";
    
    // Add dataset values
    for (size_t i = 0; i < datasets.size(); i++) {
        const auto& dataset = datasets[i];
        
        ss << "                {\n"
           << "                    label: '" << dataset.name << "',\n"
           << "                    data: [";
        
        // Add p-values for each test
        first = true;
        for (const auto& testName : testNames) {
            if (!first) ss << ", ";
            
            bool found = false;
            for (const auto& result : dataset.results) {
                if (result.testName == testName) {
                    ss << result.p_value;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                ss << "null";
            }
            
            first = false;
        }
        
        // Generate a color based on the dataset index
        int hue = (130 * i) % 360;
        ss << "],\n"
           << "                    backgroundColor: 'hsla(" << hue << ", 70%, 60%, 0.7)',\n"
           << "                    borderColor: 'hsla(" << hue << ", 70%, 40%, 1)',\n"
           << "                    borderWidth: 1\n"
           << "                }";
        
        if (i < datasets.size() - 1) {
            ss << ",";
        }
        ss << "\n";
    }
    
    ss << "            ]\n"
       << "        };\n"
       << "        \n"
       << "        new Chart(pvalueCtx, {\n"
       << "            type: 'bar',\n"
       << "            data: pvalueData,\n"
       << "            options: {\n"
       << "                responsive: true,\n"
       << "                maintainAspectRatio: false,\n"
       << "                scales: {\n"
       << "                    y: {\n"
       << "                        beginAtZero: true,\n"
       << "                        max: 1.0,\n"
       << "                        title: {\n"
       << "                            display: true,\n"
       << "                            text: 'P-Value'\n"
       << "                        }\n"
       << "                    }\n"
       << "                },\n"
       << "                plugins: {\n"
       << "                    legend: {\n"
       << "                        position: 'top',\n"
       << "                    },\n"
       << "                    title: {\n"
       << "                        display: true,\n"
       << "                        text: 'NIST Test Results Comparison'\n"
       << "                    }\n"
       << "                }\n"
       << "            }\n"
       << "        });\n";
    
    // P-value distribution histogram
    ss << "        // P-value Distribution Histogram\n"
       << "        const pDistCtx = document.getElementById('pvalueDistribution').getContext('2d');\n"
       << "        \n"
       << "        // Collect all p-values\n"
       << "        const allPValues = [];\n";
    
    for (size_t i = 0; i < datasets.size(); i++) {
        ss << "        const dataset" << i << "PValues = [";
        
        first = true;
        for (const auto& result : datasets[i].results) {
            if (!first) ss << ", ";
            ss << result.p_value;
            first = false;
        }
        
        ss << "];\n"
           << "        allPValues.push(...dataset" << i << "PValues);\n";
    }
    
    ss << "        \n"
       << "        // Count p-values in intervals\n"
       << "        const intervals = 10;\n"
       << "        const histogramData = Array(intervals).fill(0);\n"
       << "        const histogramLabels = [];\n"
       << "        \n"
       << "        for (let i = 0; i < intervals; i++) {\n"
       << "            const lowerBound = i / intervals;\n"
       << "            const upperBound = (i + 1) / intervals;\n"
       << "            histogramLabels.push(`${lowerBound.toFixed(1)}-${upperBound.toFixed(1)}`);\n"
       << "        }\n"
       << "        \n"
       << "        allPValues.forEach(p => {\n"
       << "            if (p >= 0 && p <= 1) {\n"
       << "                const index = Math.min(Math.floor(p * intervals), intervals - 1);\n"
       << "                histogramData[index]++;\n"
       << "            }\n"
       << "        });\n"
       << "        \n"
       << "        new Chart(pDistCtx, {\n"
       << "            type: 'bar',\n"
       << "            data: {\n"
       << "                labels: histogramLabels,\n"
       << "                datasets: [{\n"
       << "                    label: 'P-Value Distribution',\n"
       << "                    data: histogramData,\n"
       << "                    backgroundColor: 'rgba(75, 192, 192, 0.7)',\n"
       << "                    borderColor: 'rgba(75, 192, 192, 1)',\n"
       << "                    borderWidth: 1\n"
       << "                }]\n"
       << "            },\n"
       << "            options: {\n"
       << "                responsive: true,\n"
       << "                maintainAspectRatio: false,\n"
       << "                scales: {\n"
       << "                    y: {\n"
       << "                        beginAtZero: true,\n"
       << "                        title: {\n"
       << "                            display: true,\n"
       << "                            text: 'Count'\n"
       << "                        }\n"
       << "                    },\n"
       << "                    x: {\n"
       << "                        title: {\n"
       << "                            display: true,\n"
       << "                            text: 'P-Value Range'\n"
       << "                        }\n"
       << "                    }\n"
       << "                },\n"
       << "                plugins: {\n"
       << "                    legend: {\n"
       << "                        display: false\n"
       << "                    },\n"
       << "                    title: {\n"
       << "                        display: true,\n"
       << "                        text: 'Distribution of P-Values'\n"
       << "                    }\n"
       << "                }\n"
       << "            }\n"
       << "        });\n";
    
    // Success rate pie chart
    ss << "        // Success Rate Pie Chart\n"
       << "        const successCtx = document.getElementById('successRate').getContext('2d');\n"
       << "        \n"
       << "        // Calculate success rates for each dataset\n"
       << "        const datasetNames = [";
    
    first = true;
    for (const auto& dataset : datasets) {
        if (!first) ss << ", ";
        ss << "'" << dataset.name << "'";
        first = false;
    }
    
    ss << "];\n"
       << "        const successCounts = [];\n"
       << "        const failureCounts = [];\n"
       << "        \n";
    
    for (size_t i = 0; i < datasets.size(); i++) {
        ss << "        {\n"
           << "            let successes = 0;\n"
           << "            let failures = 0;\n"
           << "            dataset" << i << "PValues.forEach(p => {\n"
           << "                if (p >= 0.01) successes++;\n"
           << "                else failures++;\n"
           << "            });\n"
           << "            successCounts.push(successes);\n"
           << "            failureCounts.push(failures);\n"
           << "        }\n";
    }
    
    ss << "        \n"
       << "        new Chart(successCtx, {\n"
       << "            type: 'pie',\n"
       << "            data: {\n"
       << "                labels: datasetNames,\n"
       << "                datasets: [{\n"
       << "                    label: 'Success Rate',\n"
       << "                    data: successCounts.map((success, i) => success / (success + failureCounts[i]) * 100),\n"
       << "                    backgroundColor: [";
    
    for (size_t i = 0; i < datasets.size(); i++) {
        int hue = (130 * i) % 360;
        if (i > 0) ss << ", ";
        ss << "'hsla(" << hue << ", 70%, 60%, 0.7)'";
    }
    
    ss << "],\n"
       << "                    borderColor: [";
    
    for (size_t i = 0; i < datasets.size(); i++) {
        int hue = (130 * i) % 360;
        if (i > 0) ss << ", ";
        ss << "'hsla(" << hue << ", 70%, 40%, 1)'";
    }
    
    ss << "],\n"
       << "                    borderWidth: 1\n"
       << "                }]\n"
       << "            },\n"
       << "            options: {\n"
       << "                responsive: true,\n"
       << "                maintainAspectRatio: false,\n"
       << "                plugins: {\n"
       << "                    legend: {\n"
       << "                        position: 'top',\n"
       << "                    },\n"
       << "                    title: {\n"
       << "                        display: true,\n"
       << "                        text: 'Test Success Rate (%)'\n"
       << "                    },\n"
       << "                    tooltip: {\n"
       << "                        callbacks: {\n"
       << "                            label: function(context) {\n"
       << "                                const label = context.label || '';\n"
       << "                                const value = context.raw.toFixed(1);\n"
       << "                                const total = successCounts[context.dataIndex] + failureCounts[context.dataIndex];\n"
       << "                                const success = successCounts[context.dataIndex];\n"
       << "                                return `${label}: ${value}% (${success}/${total} tests passed)`;\n"
       << "                            }\n"
       << "                        }\n"
       << "                    }\n"
       << "                }\n"
       << "            }\n"
       << "        });\n"
       << "    </script>\n"
       << "</body>\n"
       << "</html>\n";
    
    return ss.str();
}

std::string VisualizationGenerator::generatePValueHeatmapHTML() {
    std::stringstream ss;
    
    ss << "<!DOCTYPE html>\n"
       << "<html lang=\"en\">\n"
       << "<head>\n"
       << "    <meta charset=\"UTF-8\">\n"
       << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
       << "    <title>P-Value Heatmap</title>\n"
       << "    <script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>\n"
       << "    <style>\n"
       << "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
       << "        .chart-container { height: 600px; }\n"
       << "    </style>\n"
       << "</head>\n"
       << "<body>\n"
       << "    <h1>P-Value Heatmap</h1>\n"
       << "    <div class=\"chart-container\" id=\"heatmap\"></div>\n"
       << "    <script>\n"
       << "        // Collect data for heatmap\n"
       << "        const testNames = [";
    
    // Get all unique test names
    std::set<std::string> testNames;
    for (const auto& dataset : datasets) {
        for (const auto& result : dataset.results) {
            testNames.insert(result.testName);
        }
    }
    
    bool first = true;
    for (const auto& testName : testNames) {
        if (!first) ss << ", ";
        ss << "'" << testName << "'";
        first = false;
    }
    
    ss << "];\n"
       << "        const datasetNames = [";
    
    first = true;
    for (const auto& dataset : datasets) {
        if (!first) ss << ", ";
        ss << "'" << dataset.name << "'";
        first = false;
    }
    
    ss << "];\n"
       << "        const pValues = [\n";
    
    // Populate p-values for each test and dataset
    for (const auto& testName : testNames) {
        ss << "            [";
        
        first = true;
        for (const auto& dataset : datasets) {
            if (!first) ss << ", ";
            
            bool found = false;
            for (const auto& result : dataset.results) {
                if (result.testName == testName) {
                    ss << result.p_value;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                ss << "null";
            }
            
            first = false;
        }
        
        ss << "],\n";
    }
    
    ss << "        ];\n"
       << "        \n"
       << "        // Create heatmap\n"
       << "        const data = {\n"
       << "            z: pValues,\n"
       << "            x: datasetNames,\n"
       << "            y: testNames,\n"
       << "            type: 'heatmap',\n"
       << "            colorscale: [\n"
       << "                [0, 'rgb(255, 0, 0)'],\n"    // Red for low p-values (failures)
       << "                [0.01, 'rgb(255, 255, 0)'],\n"  // Yellow at threshold
       << "                [1, 'rgb(0, 128, 0)']\n"     // Green for high p-values (successes)
       << "            ],\n"
       << "            colorbar: {\n"
       << "                title: 'P-Value',\n"
       << "                tickvals: [0, 0.01, 0.25, 0.5, 0.75, 1],\n"
       << "                ticktext: ['0', '0.01', '0.25', '0.5', '0.75', '1']\n"
       << "            },\n"
       << "            hovertemplate: 'Test: %{y}<br>Dataset: %{x}<br>P-value: %{z}<extra></extra>'\n"
       << "        };\n"
       << "        \n"
       << "        const layout = {\n"
       << "            title: 'P-Value Heatmap Across Tests and Datasets',\n"
       << "            xaxis: {\n"
       << "                title: 'Dataset'\n"
       << "            },\n"
       << "            yaxis: {\n"
       << "                title: 'Statistical Test'\n"
       << "            },\n"
       << "            margin: { l: 200 }\n"
       << "        };\n"
       << "        \n"
       << "        Plotly.newPlot('heatmap', [data], layout, {responsive: true});\n"
       << "    </script>\n"
       << "</body>\n"
       << "</html>\n";
    
    return ss.str();
}

std::string VisualizationGenerator::generateRadarChartHTML(size_t datasetIndex) {
    if (datasetIndex >= datasets.size()) {
        return "<html><body><h1>Error: Dataset index out of range</h1></body></html>";
    }
    
    const auto& dataset = datasets[datasetIndex];
    
    std::stringstream ss;
    
    ss << "<!DOCTYPE html>\n"
       << "<html lang=\"en\">\n"
       << "<head>\n"
       << "    <meta charset=\"UTF-8\">\n"
       << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
       << "    <title>Radar Chart - " << dataset.name << "</title>\n"
       << "    <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n"
       << "    <script src=\"https://cdn.jsdelivr.net/npm/chartjs-chart-radial-gauge\"></script>\n"
       << "    <style>\n"
       << "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
       << "        .chart-container { height: 600px; }\n"
       << "    </style>\n"
       << "</head>\n"
       << "<body>\n"
       << "    <h1>Radar Chart for " << dataset.name << "</h1>\n"
       << "    <div class=\"chart-container\">\n"
       << "        <canvas id=\"radarChart\"></canvas>\n"
       << "    </div>\n"
       << "    <script>\n"
       << "        const radarCtx = document.getElementById('radarChart').getContext('2d');\n"
       << "        \n"
       << "        const labels = [";
    
    // Add test names
    bool first = true;
    for (const auto& result : dataset.results) {
        if (!first) ss << ", ";
        ss << "'" << result.testName << "'";
        first = false;
    }
    
    ss << "];\n"
       << "        const pValues = [";
    
    // Add p-values
    first = true;
    for (const auto& result : dataset.results) {
        if (!first) ss << ", ";
        ss << result.p_value;
        first = false;
    }
    
 ss << "];\n"
    << "        \n"
    << "        new Chart(radarCtx, {\n"
    << "            type: 'radar',\n"
    << "            data: {\n"
    << "                labels: labels,\n"
    << "                datasets: [{\n"
    << "                    label: 'P-Values',\n"
    << "                    data: pValues,\n"
    << "                    backgroundColor: 'rgba(75, 192, 192, 0.2)',\n"
    << "                    borderColor: 'rgba(75, 192, 192, 1)',\n"
    << "                    pointBackgroundColor: 'rgba(75, 192, 192, 1)',\n"
    << "                    pointBorderColor: '#fff',\n"
    << "                    pointHoverBackgroundColor: '#fff',\n"
    << "                    pointHoverBorderColor: 'rgba(75, 192, 192, 1)'\n"
    << "                }]\n"
    << "            },\n"
    << "            options: {\n"
    << "                elements: {\n"
    << "                    line: {\n"
    << "                        tension: 0,\n"
    << "                        borderWidth: 3\n"
    << "                    }\n"
    << "                },\n"
    << "                scales: {\n"
    << "                    r: {\n"
    << "                        angleLines: {\n"
    << "                            display: true\n"
    << "                        },\n"
    << "                        suggestedMin: 0,\n"
    << "                        suggestedMax: 1\n"
    << "                    }\n"
    << "                },\n"
    << "                plugins: {\n"
    << "                    legend: {\n"
    << "                        position: 'top',\n"
    << "                    },\n"
    << "                    title: {\n"
    << "                        display: true,\n"
    << "                        text: 'Statistical Test P-Values'\n"
    << "                    }\n"
    << "                }\n"
    << "            }\n"
    << "        });\n"
    << "    </script>\n"
    << "</body>\n"
    << "</html>";\n"
    << "\n"
    << "    return ss.str();\n"
 }