export module Test;

import std;

namespace Tester {

    // ANSI escape codes for text colors
    constexpr auto GREEN = "\033[32m";  // Green color
    constexpr auto RED = "\033[31m";    // Red color
    constexpr auto RESET = "\033[0m";   // Reset to default color

    class TestBase {
    public:
        virtual void run() const = 0;
        virtual ~TestBase() = default;
    };

    class TestStats {
    public:
        static TestStats& getInstance() {
            static TestStats instance;
            return instance;
        }
        TestStats(const TestStats& other) = delete;
        void operator=(const TestStats& other) = delete;

        void passedATest() {
            ++m_passedTestCount;
        }
        void failedATest() {
            ++m_failedTestCount;
        }
        std::size_t getPassedCount() const {
            return m_passedTestCount;
        }
        std::size_t getFailedCount() const {
            return m_failedTestCount;
        }
    private:
        TestStats() = default;
        std::size_t m_passedTestCount{};
        std::size_t m_failedTestCount{};
    };

    class TestHandle {
    public:
        static TestHandle& getInstance() {
            static TestHandle instance;
            return instance;
        }
        TestHandle(const TestHandle& other) = delete;
        void operator=(const TestHandle& other) = delete;

        static TestStats& stats() {
            return TestStats::getInstance();
        }
        void registerTest(std::unique_ptr<TestBase> test) {
            m_tests.push_back(std::move(test));
        }
        void runTests() {
            for (const auto& test : m_tests) {
                test->run();
            }
            std::println("TEST RESULTS:");
            std::println("{}PASSED: {}{}, {}FAILED: {}{}",
                GREEN, TestStats::getInstance().getPassedCount(), RESET,
                RED, TestStats::getInstance().getFailedCount(), RESET);
        }
    private:
        TestHandle() = default;
        std::vector<std::unique_ptr<TestBase>> m_tests;
    };

    export template<typename... Args>
        class Test : public TestBase {
        public:
            template<typename Func, typename... Args2>
                requires(std::is_constructible_v<std::tuple<Args...>, Args2&&...>&&
            std::is_invocable_r_v<bool, Func, Args...>)
                explicit Test(std::string_view testMessage, Func&& testFunction, Args2&&... args) :
                m_testMessage(testMessage),
                m_testFunction(std::forward<Func>(testFunction)),
                m_testArguments(std::forward<Args2>(args)...)
            {
                TestHandle::getInstance().registerTest(std::make_unique<Test>(*this));
            }

            void run() const override {
                if (std::apply(m_testFunction, m_testArguments)) {
                    std::println("{}'{}' PASSED!{}", GREEN, m_testMessage, RESET);
                    TestHandle::stats().passedATest();
                }
                else {
                    std::println("{}'{}' FAILED!{}", RED, m_testMessage, RESET);
                    TestHandle::stats().failedATest();
                }
            }

        private:
            std::string m_testMessage;
            std::function<bool(Args...)> m_testFunction;
            std::tuple<Args...> m_testArguments;
    };

    export void runTests() {
        TestHandle::getInstance().runTests();
    }

}
