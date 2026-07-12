#include "test_framework.h"

#include <exception>
#include <iostream>

int main()
{
    std::size_t failures = 0;

    for (const archlab::test::TestCase& test : archlab::test::registry()) {
        try {
            test.function();
            std::cout << "[PASS] " << test.name << '\n';
        } catch (const std::exception& error) {
            ++failures;
            std::cerr << "[FAIL] " << test.name << ": " << error.what() << '\n';
        } catch (...) {
            ++failures;
            std::cerr << "[FAIL] " << test.name << ": unknown exception\n";
        }
    }

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All " << archlab::test::registry().size() << " test(s) passed\n";
    return 0;
}
