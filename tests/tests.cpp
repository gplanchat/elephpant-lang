
#include "tests.h"


namespace rephp {

namespace test {

void
test_suite::operator() (std::function<void()> &test_case)
{
    test_cases.push_back(test_case);
}

void
test_suite::operator() ()
{
    for (auto it = test_cases.begin(); it != test_cases.end(); ++it) {
        (*it)();
    }
}

};

};
