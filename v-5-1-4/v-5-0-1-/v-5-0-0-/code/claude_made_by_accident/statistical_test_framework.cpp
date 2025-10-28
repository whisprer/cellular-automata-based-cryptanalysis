namespace caca::tests {
    // Concept for statistical tests
    template<typename T>
    concept StatisticalTest = requires(T test, const BitSequence& data) {
        { test.execute(data) } -> std::same_as<TestResult>;
        { test.getName() } -> std::convertible_to<std::string>;
    };

    // Compile-time test registration and management
    template<StatisticalTest T>
    class TestSuite {
        std::vector<std::unique_ptr<T>> tests;
    public:
        void addTest(std::unique_ptr<T> test) {
            tests.push_back(std::move(test));
        }
    };
}