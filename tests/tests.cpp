
#include "tests.h"


namespace rephp {

namespace test {

test_suite &
test_suite::operator<< (std::function<void()> test_case)
{
    std::cout << "[ " << this << " ] Adding test." << std::endl;
    test_cases.push_back(test_case);

    return *this;
}

test_suite &
test_suite::operator() ()
{
    std::cout << "[ " << this << " ] Running tests" << std::endl;
    for (auto it = test_cases.begin(); it != test_cases.end(); ++it) {
        std::cout << "[ " << this << " ] Testing..." << std::endl;
        (*it)();
    }

    return *this;
}

};

};
