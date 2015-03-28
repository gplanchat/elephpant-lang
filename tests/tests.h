
#ifndef REPHP_TESTS_H
#define REPHP_TESTS_H

#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

namespace rephp {

namespace test {

template<typename Type>
class value_bag
{
public:
    Type value;

    value_bag(Type value): value(value) {}
};

namespace asserter {

template<typename Expected, typename Actual>
class base_asserter
{
public:
    virtual ~base_asserter() {}
    virtual bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual) = 0;
};

template<typename Expected, typename Actual>
class equals: public base_asserter<Expected, Actual>
{
public:
    bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual);
};

template<typename Expected, typename Actual>
class not_equals: public base_asserter<Expected, Actual>
{
public:
    bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual);
};

template<typename Expected, typename Actual>
class greater_than: public base_asserter<Expected, Actual>
{
public:
    bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual);
};

template<typename Expected, typename Actual>
class lower_than: public base_asserter<Expected, Actual>
{
public:
    bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual);
};

template<typename Expected, typename Actual>
class greater_equal: public base_asserter<Expected, Actual>
{
public:
    bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual);
};

template<typename Expected, typename Actual>
class lower_equal: public base_asserter<Expected, Actual>
{
public:
    bool operator() (value_bag<Expected> &expected, value_bag<Actual> &actual);
};

template<typename Expected, typename Actual>
bool
equals<Expected, Actual>::operator() (value_bag<Expected> &expected, value_bag<Actual> &actual)
{
    return expected.value == actual.value;
}

template<typename Expected, typename Actual>
bool
not_equals<Expected, Actual>::operator() (value_bag<Expected> &expected, value_bag<Actual> &actual)
{
    return expected.value != actual.value;
}

template<typename Expected, typename Actual>
bool
greater_than<Expected, Actual>::operator() (value_bag<Expected> &expected, value_bag<Actual> &actual)
{
    return expected.value > actual.value;
}

template<typename Expected, typename Actual>
bool
lower_than<Expected, Actual>::operator() (value_bag<Expected> &expected, value_bag<Actual> &actual)
{
    return expected.value < actual.value;
}

template<typename Expected, typename Actual>
bool
greater_equal<Expected, Actual>::operator() (value_bag<Expected> &expected, value_bag<Actual> &actual)
{
    return expected.value >= actual.value;
}

template<typename Expected, typename Actual>
bool
lower_equal<Expected, Actual>::operator() (value_bag<Expected> &expected, value_bag<Actual> &actual)
{
    return expected.value <= actual.value;
}

}; //namespace asserter

template<typename Grammar, typename Space>
class assertion
{
    Grammar grammar;
    Space   space;

    void success(std::string &message);
    void error(std::string &message);
    void failure(std::string &message);
    void ignore(std::string &message);
    void exception(std::string &message);

public:
    assertion(Grammar &grammar, const Space &space): grammar(grammar), space(space)
    {}

    template<typename Expected>
    bool operator() (asserter::base_asserter<Expected, Expected> &asserter, std::string &ios, value_bag<Expected> expected, std::string &message)
    {
        Expected result;
        std::string str;

        try {
            if (!phrase_parse(ios.begin(), ios.end(), grammar, space, result)) {
                if (message.size() > 0) {
                    str = "Parsing failed: " + message;
                    error(str);
                } else {
                    str = "Parsing failed.";
                    error(str);
                }
                return false;
            }
        } catch (...) {
            if (message.size() > 0) {
                str = "An exception occurred while parsing: " + message;
                exception(str);
            } else {
                str = "An exception occurred while parsing.";
                exception(str);
            }
            return false;
        }

        auto actual = value_bag<Expected>(result);

        if (asserter(expected, actual)) {
            success(message);
            return true;
        }

        if (message.size() > 0) {
            str = "Failure: " + message;
            failure(str);
        } else {
            str = "Failure";
            failure(str);
        }

        return false;
    }
};

template<typename Grammar, typename Space>
class assert_true
{
private:
    assertion<Grammar, Space>            assertion;
    asserter::base_asserter<bool, bool> *asserter;

public:
    assert_true(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::equals<bool, bool>();
    }

    ~assert_true()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message)
    {
        return assertion(*asserter, ios, value_bag<bool>(true), message);
    }
};

template<typename Grammar, typename Space>
class assert_false
{
private:
    assertion<Grammar, Space>            assertion;
    asserter::base_asserter<bool, bool> *asserter;

public:
    assert_false(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::equals<bool, bool>();
    }

    ~assert_false()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message)
    {
        return assertion(*asserter, ios, value_bag<bool>(false), message);
    }
};

template<typename Grammar, typename Space, typename Expected>
class assert_equals
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_equals(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::equals<Expected, Expected>();
    }

    ~assert_equals()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message, Expected expected)
    {
        return assertion(*asserter, ios, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space, typename Expected>
class assert_not_equals
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_not_equals(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::not_equals<Expected, Expected>();
    }

    ~assert_not_equals()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message, Expected expected)
    {
        return assertion(*asserter, ios, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space, typename Expected>
class assert_greater_than
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_greater_than(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::greater_than<Expected, Expected>();
    }

    ~assert_greater_than()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message, Expected expected)
    {
        return assertion(*asserter, ios, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space, typename Expected>
class assert_lower_than
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_lower_than(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::lower_than<Expected, Expected>();
    }

    ~assert_lower_than()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message, Expected expected)
    {
        return assertion(*asserter, ios, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space, typename Expected>
class assert_greater_equal
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_greater_equal(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::greater_equal<Expected, Expected>();
    }

    ~assert_greater_equal()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message, Expected expected)
    {
        return assertion(*asserter, ios, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space, typename Expected>
class assert_lower_equal
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_lower_equal(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::lower_equal<Expected, Expected>();
    }

    ~assert_lower_equal()
    {
        delete asserter;
    }

    bool operator() (std::string &ios, std::string message, Expected expected)
    {
        return assertion(*asserter, ios, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::success(std::string &message)
{
    std::cerr << "\e[1;30;41m" << "[ OK ] " << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::error(std::string &message)
{
    std::cerr << "\e[5;31;47m" << "[ ERROR ] " << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::failure(std::string &message)
{
    std::cerr << "\e[5;31;47m" << "[ FAILURE ] " << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::ignore(std::string &message)
{
    std::cerr << "\e[5;31;47m" << "[ IGNORE ] " << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::exception(std::string &message)
{
    std::cerr << "\e[5;31;47m" << "[ EXCEPTION ] " << message << "\e[0m" << std::endl;
}

class test_suite
{
private:
    std::list<std::function<void()>> test_cases;

public:
    test_suite &operator<< (std::function<void()> test_case);
    test_suite &operator() ();
};

/*
std::ostream &_error(std::ostream &ios)
{
    return ios << "\e[5;31;47m";
}

std::ostream &_success(std::ostream &ios)
{
    return ios << "\e[1;30;41m";
}

std::ostream &_end(std::ostream &ios)
{
    return ios << "\e[0m" << std::endl;
}
*/

};

};

#endif