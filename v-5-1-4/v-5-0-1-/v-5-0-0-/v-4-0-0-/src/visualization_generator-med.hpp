// Add these functions to the VisualizationGenerator class before the closing bracket:

#pragma once


// Generate p-value heatmap (simplified)
void generatePValueHeatmap() {
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

// Generate comparative bar chart (simplified)
void generateComparativeBarChart() {
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

// Generate distribution histogram (simplified)
void generateDistributionHistogram() {
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

// Generate radar chart (simplified)
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