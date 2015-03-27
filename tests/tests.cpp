
#include "tests.h"


namespace rephp {

namespace test {

test_suite &
test_suite::operator<< (std::function<void()> &test_case)
{
    test_cases.push_back(test_case);

    return *this;
}

test_suite &
test_suite::operator() ()
{
    for (auto it = test_cases.begin(); it != test_cases.end(); ++it) {
        (*it)();
    }

    return *this;
}

};

};
