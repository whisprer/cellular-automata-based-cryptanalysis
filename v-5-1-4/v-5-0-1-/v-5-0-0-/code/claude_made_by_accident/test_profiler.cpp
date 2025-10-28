namespace caca::tests::profiling {
    class TestProfiler {
    private:
        struct TestMetrics {
            std::string test_name;
            std::chrono::high_resolution_clock::time_point start_time;
            std::chrono::high_resolution_clock::time_point end_time;
            std::chrono::microseconds duration;
            size_t data_size;
            bool test_passed;
            double p_value;
        };

        std::vector<TestMetrics> test_results;
        std::ofstream log_file;

    public:
        TestProfiler(const std::string& log_path = "nist_test_profiling.csv") {
            log_file.open(log_path, std::ios::out | std::ios::app);
            log_file << "Test Name,Data Size,Duration (μs),P-Value,Result\n";
        }

        template<typename TestFunction>
        TestResult profile_test(
            const std::string& test_name, 
            const BitSequence& data, 
            TestFunction&& test_func
        ) {
            TestMetrics metrics;
            metrics.test_name = test_name;
            metrics.data_size = data.size();
            metrics.start_time = std::chrono::high_resolution_clock::now();

            // Execute the test
            TestResult result = test_func(data);

            metrics.end_time = std::chrono::high_resolution_clock::now();
            metrics.duration = std::chrono::duration_cast<std::chrono::microseconds>(
                metrics.end_time - metrics.start_time
            );
            metrics.test_passed = result.success;
            metrics.p_value = result.p_value;

            // Log to CSV
            log_file << test_name << ","
                     << metrics.data_size << ","
                     << metrics.duration.count() << ","
                     << metrics.p_value << ","
                     << (metrics.test_passed ? "PASS" : "FAIL") << "\n";

            test_results.push_back(metrics);
            return result;
        }

        void generate_performance_report() {
            // Sort tests by duration, descending
            std::sort(test_results.begin(), test_results.end(), 
                [](const TestMetrics& a, const TestMetrics& b) {
                    return a.duration > b.duration;
                }
            );

            std::cout << "NIST Test Performance Report:\n";
            std::cout << "----------------------------\n";
            for (const auto& result : test_results) {
                std::cout << fmt::format(
                    "Test: {:<25} Size: {:<10} Duration: {:<10} μs | Result: {}\n",
                    result.test_name, 
                    result.data_size, 
                    result.duration.count(), 
                    result.test_passed ? "PASS" : "FAIL"
                );
            }
        }
    };
}

/* Example Usage

TestProfiler profiler;

// Profile individual tests
auto frequency_result = profiler.profile_test(
    "Frequency Test", 
    test_data, 
    [](const BitSequence& data) { 
        return FrequencyTest().execute(data); 
    }
);

// After all tests
profiler.generate_performance_report();

*/
