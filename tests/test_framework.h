#ifndef ARCHLAB_RVSOC_SIM_TESTS_TEST_FRAMEWORK_H_
#define ARCHLAB_RVSOC_SIM_TESTS_TEST_FRAMEWORK_H_

#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace archlab::test {

using TestFunction = void (*)();

struct TestCase {
    std::string name;
    TestFunction function{};
};

inline std::vector<TestCase>& registry()
{
    static std::vector<TestCase> tests;
    return tests;
}

class Registrar {
public:
    Registrar(std::string name, TestFunction function)
    {
        registry().push_back(TestCase{std::move(name), function});
    }
};

[[noreturn]] inline void fail(const char* expression, const char* file, int line)
{
    std::ostringstream message;
    message << file << ':' << line << ": check failed: " << expression;
    throw std::runtime_error(message.str());
}

}  // namespace archlab::test

#define ARCHLAB_TEST_DETAIL_JOIN_INNER(lhs, rhs) lhs##rhs
#define ARCHLAB_TEST_DETAIL_JOIN(lhs, rhs) ARCHLAB_TEST_DETAIL_JOIN_INNER(lhs, rhs)

#define ARCHLAB_TEST(name)                                                                    \
    static void ARCHLAB_TEST_DETAIL_JOIN(archlab_test_function_, __LINE__)();                 \
    static ::archlab::test::Registrar ARCHLAB_TEST_DETAIL_JOIN(archlab_test_registrar_,       \
                                                                __LINE__)(                    \
        name, &ARCHLAB_TEST_DETAIL_JOIN(archlab_test_function_, __LINE__));                   \
    static void ARCHLAB_TEST_DETAIL_JOIN(archlab_test_function_, __LINE__)()

#define ARCHLAB_CHECK(expression)                                                             \
    do {                                                                                      \
        if (!(expression)) {                                                                  \
            ::archlab::test::fail(#expression, __FILE__, __LINE__);                          \
        }                                                                                     \
    } while (false)

#define ARCHLAB_CHECK_EQ(lhs, rhs) ARCHLAB_CHECK((lhs) == (rhs))

#define ARCHLAB_CHECK_THROWS_AS(expression, exception_type)                                   \
    do {                                                                                      \
        bool archlab_exception_seen = false;                                                  \
        try {                                                                                 \
            static_cast<void>(expression);                                                    \
        } catch (const exception_type&) {                                                     \
            archlab_exception_seen = true;                                                    \
        }                                                                                     \
        ARCHLAB_CHECK(archlab_exception_seen);                                                \
    } while (false)

#endif  // ARCHLAB_RVSOC_SIM_TESTS_TEST_FRAMEWORK_H_
