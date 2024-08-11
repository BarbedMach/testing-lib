import std;
import Test;

auto main() -> int {
    // Basic arithmetic tests
    Tester::Test("1 + 1 == 2", []() { return 1 + 1 == 2; });
    Tester::Test("1 + 1 != 3", []() { return 1 + 1 != 3; });

    // Correct filter test
    auto v1 = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Tester::Test("Odd number filter test 1", [v1]() {
        auto v2 = v1 | std::views::filter([](auto x) { return x % 2 == 1; });
        std::vector<int> expected{ 1, 3, 5, 7, 9 };
        return std::ranges::equal(v2, expected);
        });

    // Additional tests
    Tester::Test("Odd number filter test 2 (empty vector)", []() {
        auto v1 = std::vector<int>{};
        auto v2 = v1 | std::views::filter([](auto x) { return x % 2 == 1; });
        return v2.empty();  // Expect the filtered result to be empty
        });

    Tester::Test("Odd number filter test 3 (no odd numbers)", []() {
        auto v1 = std::vector{ 2, 4, 6, 8 };
        auto v2 = v1 | std::views::filter([](auto x) { return x % 2 == 1; });
        return v2.empty();  // Expect the filtered result to be empty
        });

    Tester::Test("Odd number filter test 4 (all odd numbers)", []() {
        auto v1 = std::vector{ 1, 3, 5, 7, 9 };
        auto v2 = v1 | std::views::filter([](auto x) { return x % 2 == 1; });
        return std::ranges::equal(v1, v2);  // Expect the filtered result to be the same as input
        });

    // Test for string length
    Tester::Test("String length test", []() {
        std::string str = "Hello, World!";
        return str.size() == 13;
        });

    // Factorial calculation test
    auto factorial = [](int n) -> int {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
        };
    Tester::Test("Factorial of 5", [factorial]() {
        return factorial(5) == 120;
        });

    // Failing tests
    Tester::Test("Failing test: 1 + 1 != 2", []() { return 1 + 1 != 2; });  // This will fail
    Tester::Test("Failing test: String length is 10", []() {
        std::string str = "Hello, World!";
        return str.size() == 10;  // This will fail
        });
    Tester::Test("Failing test: Factorial of 5 is 100", [factorial]() {
        return factorial(5) == 100;  // This will fail
        });

    Tester::runTests();
}