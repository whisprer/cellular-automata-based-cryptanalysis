// BitSequence class to replace the raw BitSequence array
class BitSequence {
private:
    std::vector<bool> bits;
    
public:
    BitSequence(size_t size = 0);
    bool operator[](size_t index) const;
    bool& operator[](size_t index);
    size_t size() const;
    // Additional utility methods
};

// Base class for all statistical tests
class StatisticalTest {
public:
    virtual ~StatisticalTest() = default;
    virtual TestResult execute(const BitSequence& data) = 0;
    virtual std::string getName() const = 0;
};

// Random number generator interface
class RandomNumberGenerator {
public:
    virtual ~RandomNumberGenerator() = default;
    virtual BitSequence generate(size_t length) = 0;
    virtual std::string getName() const = 0;
};

// Main test suite class
class TestSuite {
private:
    std::vector<std::unique_ptr<StatisticalTest>> tests;
    TestParameters params;
    
public:
    TestSuite();
    void addTest(std::unique_ptr<StatisticalTest> test);
    void removeTest(const std::string& testName);
    void setParameters(const TestParameters& params);
    TestResults runTests(const BitSequence& data);
    TestResults runTests(RandomNumberGenerator& generator);
};