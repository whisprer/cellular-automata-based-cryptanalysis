#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <thread>
#include <chrono>
#include <cmath>        // for log2, sqrt, pow, etc.
#include <stdexcept>    // for runtime_error
#include <algorithm>    // for sort

// -------------------------- Utility / Loader --------------------------

class CipherDataLoader {
public:
    static std::vector<uint8_t> loadCipherText(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Error: Cannot open file " + filename);
        }
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), {});
    }
};

// ------------------- Cellular Automata with multiple rules -------------------
class CellularAutomataProcessor {
private:
    // We'll store 4 separate grids, each one uses a different rule.
    std::vector<std::bitset<8>> gridRule30;
    std::vector<std::bitset<8>> gridRule82;
    std::vector<std::bitset<8>> gridRule110;
    std::vector<std::bitset<8>> gridRule150;

    std::vector<std::bitset<8>> nextRule30;
    std::vector<std::bitset<8>> nextRule82;
    std::vector<std::bitset<8>> nextRule110;
    std::vector<std::bitset<8>> nextRule150;

    size_t dataSize;
    int numThreads = 4;

    // Define lambdas for each rule
    static std::bitset<8> applyRule30(std::bitset<8> left,
                                      std::bitset<8> center,
                                      std::bitset<8> right)
    {
        // Rule 30 in binary is 00011110 (from 111..000 indexing)
        //   111->0, 110->0, 101->0, 100->1,
        //   011->1, 010->1, 001->1, 000->0
        static const unsigned char RULE_30 = 0b00011110;
        std::bitset<8> next;
        for (int b = 0; b < 8; b++) {
            int l = (int)left[b];
            int c = (int)center[b];
            int r = (int)right[b];
            int idx = (l << 2) | (c << 1) | r;
            // (7 - idx) to match Wolfram ordering 111->bit7..000->bit0
            int nextBit = (RULE_30 >> (7 - idx)) & 1;
            next[b] = nextBit;
        }
        return next;
    }

    static std::bitset<8> applyRule82(std::bitset<8> left,
                                      std::bitset<8> center,
                                      std::bitset<8> right)
    {
        //  Rule 82 = 01010010 in binary
        //    111->0, 110->1, 101->0, 100->1,
        //    011->0, 010->0, 001->1, 000->0
        static const unsigned char RULE_82 = 0b01010010;
        std::bitset<8> next;
        for (int b = 0; b < 8; b++) {
            int l = (int)left[b];
            int c = (int)center[b];
            int r = (int)right[b];
            int idx = (l << 2) | (c << 1) | r;
            int nextBit = (RULE_82 >> (7 - idx)) & 1;
            next[b] = nextBit;
        }
        return next;
    }

    static std::bitset<8> applyRule110(std::bitset<8> left,
                                       std::bitset<8> center,
                                       std::bitset<8> right)
    {
        // Rule 110 = 01101110 in binary
        //    111->0, 110->1, 101->1, 100->0,
        //    011->1, 010->1, 001->1, 000->0
        static const unsigned char RULE_110 = 0b01101110;
        std::bitset<8> next;
        for (int b = 0; b < 8; b++) {
            int l = (int)left[b];
            int c = (int)center[b];
            int r = (int)right[b];
            int idx = (l << 2) | (c << 1) | r;
            int nextBit = (RULE_110 >> (7 - idx)) & 1;
            next[b] = nextBit;
        }
        return next;
    }

    static std::bitset<8> applyRule150(std::bitset<8> left,
                                       std::bitset<8> center,
                                       std::bitset<8> right)
    {
        // Rule 150 = 10010110 in binary
        //   111->1, 110->0, 101->0, 100->1,
        //   011->0, 010->1, 001->1, 000->0
        static const unsigned char RULE_150 = 0b10010110;
        std::bitset<8> next;
        for (int b = 0; b < 8; b++) {
            int l = (int)left[b];
            int c = (int)center[b];
            int r = (int)right[b];
            int idx = (l << 2) | (c << 1) | r;
            int nextBit = (RULE_150 >> (7 - idx)) & 1;
            next[b] = nextBit;
        }
        return next;
    }

public:
    CellularAutomataProcessor(size_t sz) : dataSize(sz) {
        // Allocate 4 sets of grids
        gridRule30.resize(dataSize);
        gridRule82.resize(dataSize);
        gridRule110.resize(dataSize);
        gridRule150.resize(dataSize);

        nextRule30.resize(dataSize);
        nextRule82.resize(dataSize);
        nextRule110.resize(dataSize);
        nextRule150.resize(dataSize);
    }

    void initializeFromCiphertext(const std::vector<uint8_t> &cipherData) {
        // Copy each byte into each rule's grid
        for (size_t i = 0; i < cipherData.size(); i++) {
            std::bitset<8> bits(cipherData[i]);
            gridRule30[i]  = bits;
            gridRule82[i]  = bits;
            gridRule110[i] = bits;
            gridRule150[i] = bits;
        }
    }

    void updateAllRules() {
        // For multi-threading, we can just do it all in one pass,
        // each thread updates each rule's grid.

        std::vector<std::thread> workers;
        workers.reserve(numThreads);

        auto workerFunc = [&](int t) {
            for (size_t i = t; i < dataSize; i += numThreads) {
                // left / right boundary check
                std::bitset<8> left = (i > 0) ? gridRule30[i - 1] : std::bitset<8>(0);
                std::bitset<8> cent = gridRule30[i];
                std::bitset<8> right = (i < dataSize - 1) ? gridRule30[i + 1] : std::bitset<8>(0);
                nextRule30[i] = applyRule30(left, cent, right);

                left = (i > 0) ? gridRule82[i - 1] : std::bitset<8>(0);
                cent = gridRule82[i];
                right = (i < dataSize - 1) ? gridRule82[i + 1] : std::bitset<8>(0);
                nextRule82[i] = applyRule82(left, cent, right);

                left = (i > 0) ? gridRule110[i - 1] : std::bitset<8>(0);
                cent = gridRule110[i];
                right = (i < dataSize - 1) ? gridRule110[i + 1] : std::bitset<8>(0);
                nextRule110[i] = applyRule110(left, cent, right);

                left = (i > 0) ? gridRule150[i - 1] : std::bitset<8>(0);
                cent = gridRule150[i];
                right = (i < dataSize - 1) ? gridRule150[i + 1] : std::bitset<8>(0);
                nextRule150[i] = applyRule150(left, cent, right);
            }
        };

        for (int t = 0; t < numThreads; t++) {
            workers.emplace_back(workerFunc, t);
        }
        for (auto &wt : workers) {
            wt.join();
        }

        // Swap for each rule
        gridRule30.swap(nextRule30);
        gridRule82.swap(nextRule82);
        gridRule110.swap(nextRule110);
        gridRule150.swap(nextRule150);
    }

    // Extract final data for each rule
    std::vector<uint8_t> extractDataRule30() {
        std::vector<uint8_t> out(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            out[i] = static_cast<uint8_t>(gridRule30[i].to_ulong());
        }
        return out;
    }
    std::vector<uint8_t> extractDataRule82() {
        std::vector<uint8_t> out(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            out[i] = static_cast<uint8_t>(gridRule82[i].to_ulong());
        }
        return out;
    }
    std::vector<uint8_t> extractDataRule110() {
        std::vector<uint8_t> out(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            out[i] = static_cast<uint8_t>(gridRule110[i].to_ulong());
        }
        return out;
    }
    std::vector<uint8_t> extractDataRule150() {
        std::vector<uint8_t> out(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            out[i] = static_cast<uint8_t>(gridRule150[i].to_ulong());
        }
        return out;
    }
};

// ---------------------- Statistical Analysis Class ----------------------
class StatAnalyzer {
public:
    // 1) Byte frequency & spikes
    static void byteFrequency(const std::vector<uint8_t> &data, bool showTopSpikes=true) {
        int freq[256] = {0};
        for (auto b : data) {
            freq[b]++;
        }
        // Print a summary
        std::cout << "Byte Frequency:\n";
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                std::cout << "  0x" << std::hex << i << " : " << std::dec << freq[i] << "\n";
            }
        }
        // Show top spikes (largest frequencies)
        if (showTopSpikes) {
            // gather them in a vector
            std::vector<std::pair<int,int>> sortedBytes; // (freq, byte)
            for (int i = 0; i < 256; i++) {
                if (freq[i] > 0) {
                    sortedBytes.push_back({freq[i], i});
                }
            }
            std::sort(sortedBytes.begin(), sortedBytes.end(),
                      [](auto &a, auto &b){return a.first > b.first;});
            std::cout << "\nTop 5 Most Frequent Bytes:\n";
            for (int i = 0; i < 5 && i < (int)sortedBytes.size(); i++) {
                auto &p = sortedBytes[i];
                std::cout << "  Byte 0x" << std::hex << p.second 
                          << " => " << std::dec << p.first << " occurrences\n";
            }
        }
    }

    // 2) Index of coincidence
    static double indexOfCoincidence(const std::vector<uint8_t> &data) {
        long long freq[256] = {0};
        for (auto b : data) {
            freq[b]++;
        }
        long long N = (long long)data.size();
        long long sum = 0;
        for (int i = 0; i < 256; i++) {
            sum += freq[i]*(freq[i]-1);
        }
        double ic = (double)sum / (double)(N*(N-1));
        return ic;
    }

    // 3) Chi-square test
    //   We'll test how the frequency distribution compares to uniform,
    //   i.e. each byte is expected ~ data.size()/256 times
    static double chiSquare(const std::vector<uint8_t> &data) {
        int freq[256] = {0};
        size_t N = data.size();
        for (auto b : data) {
            freq[b]++;
        }
        double expected = (double)N / 256.0;
        double chi = 0.0;
        for (int i = 0; i < 256; i++) {
            double diff = freq[i] - expected;
            chi += (diff * diff) / expected;
        }
        return chi;
    }

    // 4) Serial correlation
    //   We'll measure correlation between consecutive bytes.
    //   Perfectly random => ~0 correlation
    static double serialCorrelation(const std::vector<uint8_t> &data) {
        if (data.size() < 2) return 0.0;
        double mean = 0.0;
        for (auto b : data) {
            mean += b;
        }
        mean /= data.size();

        double num = 0.0;
        double den1 = 0.0;
        double den2 = 0.0;
        for (size_t i = 0; i+1 < data.size(); i++) {
            double x = data[i]   - mean;
            double y = data[i+1] - mean;
            num  += (x * y);
            den1 += (x * x);
            den2 += (y * y);
        }
        double corr = num / std::sqrt(den1*den2);
        return corr;
    }

    // 5) Simple NIST-like Monobit test
    //   Count the number of 1 bits in the entire dataset, see if it’s near half.
    //   For random data, the proportion of 1 bits ~ 0.5
    static double monobitTestPValue(const std::vector<uint8_t> &data) {
        // Count total bits
        long long totalBits = (long long)data.size()*8;
        long long onesCount = 0;
        for (auto b : data) {
            // builtin popcount is C++20, or do a manual approach
            // We'll do a naive approach:
            for (int i = 0; i < 8; i++) {
                if (b & (1 << i)) {
                    onesCount++;
                }
            }
        }
        double s = std::fabs((double)onesCount - (double)(totalBits/2)) / std::sqrt(totalBits/4.0);
        // For the monobit test, the statistic s ~ N(0,1).
        // We can do a simple approximation for the p-value of a two-tailed normal test.
        // We'll do a quick function:
        // p-value = erfc( s / sqrt(2) ), but let's do an approximate approach
        // or we can do a rough estimate with a small table:
        // For demonstration, let's approximate. We'll do a simple if/else approach:

        // More precise approach: pVal = 1 - erf( s/sqrt(2) )
        // We'll just return s as a "test statistic" for demonstration:
        return s; 
    }

    // A helper to display all stats for a given dataset
    static void displayStats(const std::string &label, const std::vector<uint8_t> &data) {
        std::cout << "\n\n=== Stats for " << label << " ===\n";
        std::cout << "Data size: " << data.size() << " bytes\n";

        // Byte freq
        byteFrequency(data, false);

        // Index of coincidence
        double ic = indexOfCoincidence(data);
        std::cout << "Index of coincidence: " << ic << "\n";

        // Chi-square
        double chi = chiSquare(data);
        std::cout << "Chi-square: " << chi << "\n";

        // Serial correlation
        double scorr = serialCorrelation(data);
        std::cout << "Serial correlation: " << scorr << "\n";

        // Monobit test statistic
        double sVal = monobitTestPValue(data);
        std::cout << "Monobit test statistic (z-score): " << sVal << "\n";

        std::cout << "====================================\n\n";
    }
};

// ---------------------- Main App -------------------------------------
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename> [iterations]\n";
        return 1;
    }
    std::string filename = argv[1];
    int iterations = 5; // default
    if (argc >= 3) {
        iterations = std::stoi(argv[2]);
    }

    try {
        // 1) Load the file
        auto cipherData = CipherDataLoader::loadCipherText(filename);
        size_t dataSize = cipherData.size();
        if (dataSize == 0) {
            std::cerr << "File is empty or not loaded properly.\n";
            return 1;
        }

        // 2) Initialize the multi-rule CA processor
        CellularAutomataProcessor caProcessor(dataSize);
        caProcessor.initializeFromCiphertext(cipherData);

        // 3) Run CA for N iterations, updating all 4 rules in parallel
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; i++) {
            caProcessor.updateAllRules();
        }
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        std::cout << "Ran " << iterations << " iteration(s) for all 4 rules in " 
                  << elapsed << " sec.\n";

        // 4) Extract final data from each rule
        auto data30  = caProcessor.extractDataRule30();
        auto data82  = caProcessor.extractDataRule82();
        auto data110 = caProcessor.extractDataRule110();
        auto data150 = caProcessor.extractDataRule150();

        // 5) For each final dataset, run the stats
        StatAnalyzer::displayStats("Rule 30",  data30);
        StatAnalyzer::displayStats("Rule 82",  data82);
        StatAnalyzer::displayStats("Rule 110", data110);
        StatAnalyzer::displayStats("Rule 150", data150);

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
