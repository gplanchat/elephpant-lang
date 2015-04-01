
#ifndef REPHP_TESTS_H
#define REPHP_TESTS_H

#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "../engine/types.h"
#include "../engine/parser/ast.h"
#include "../engine/variant.h"

namespace rephp {

namespace test {

namespace ast = rephp::engine::parser::ast;
namespace type = rephp::engine::type;
using rephp::engine::visitor::internal_value_stream_visitor;

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
    virtual ~base_asserter()
    {}

    virtual bool operator() (value_bag<Expected> &, value_bag<Actual> &)
    {
        return false;
    }
};

template<typename Variant, typename Type>
class variant_is_type: public base_asserter<Variant, Variant>
{
public:
    bool operator() (value_bag<Variant> &expected, value_bag<Variant> &actual);
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

template<typename Variant, typename Type>
bool
variant_is_type<Variant, Type>::operator() (value_bag<Variant> &, value_bag<Variant> &actual)
{
    return type::variant_is_type<Type>(actual.value);
}

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

    void success(std::string message);
    template<typename Type>
    void error(std::string message, Type &expected);
    void failure(std::string message);
    template<typename Type>
    void failure(std::string message, Type &expected, Type &actual);
    void ignore(std::string message);
    void exception(std::string message);

public:
    assertion(Grammar &grammar, const Space &space): grammar(grammar), space(space)
    {}

    template<typename Iterator, typename Expected>
    bool operator() (asserter::base_asserter<Expected, Expected> &asserter, Iterator &begin, Iterator &end, value_bag<Expected> expected, std::string message)
    {
        Expected result;
        std::string str;

        try {
            if (!phrase_parse(begin, end, grammar, space, result)) {
                if (message.size() > 0) {
                    str = "Parsing failed: " + message;
                    error(str, expected.value);
                } else {
                    str = "Parsing failed.";
                    error(str, expected.value);
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
            failure(str, expected.value, actual.value);
        } else {
            str = "Failure";
            failure(str, expected.value, actual.value);
        }

        return false;
    }

    template<typename Iterator>
    bool operator() (Iterator &begin, Iterator &end, std::string message)
    {
        std::string str;

        try {
            if (phrase_parse(begin, end, grammar, space)) {
                if (message.size() > 0) {
                    str = "Parsing succeeded, while it shouldn't: " + message;
                    failure(str);
                } else {
                    str = "Parsing succeeded, while it shouldn't.";
                    failure(str);
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

        if (message.size() > 0) {
            str = "Success: " + message;
            success(str);
        } else {
            str = "Success.";
            success(str);
        }

        return false;
    }

    template<typename Expected>
    bool operator() (asserter::base_asserter<Expected, Expected> &asserter, std::string input, value_bag<Expected> expected, std::string message)
    {
        auto it = input.begin();
        auto end = input.end();
        return (*this)(asserter, it, end, expected, message);
    }

    bool operator() (std::string input, std::string message)
    {
        auto it = input.begin();
        auto end = input.end();
        return (*this)(it, end, message);
    }
};

template<typename Grammar, typename Space, typename Variant, typename Type>
class assert_variant_is_type
{
private:
    assertion<Grammar, Space>                  assertion;
    asserter::base_asserter<Variant, Variant> *asserter;

public:
    assert_variant_is_type(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::variant_is_type<Variant, Type>();
    }

    ~assert_variant_is_type()
    {
        delete asserter;
    }

    bool operator() (std::string input, Type expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Variant>(expected), message);
    }
};

template<typename Grammar, typename Space, typename Expected = bool>
class assert_true
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_true(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::equals<Expected, Expected>();
    }

    ~assert_true()
    {
        delete asserter;
    }

    bool operator() (std::string input, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(true), message);
    }
};

template<typename Grammar, typename Space, typename Expected = bool>
class assert_false
{
private:
    assertion<Grammar, Space>                    assertion;
    asserter::base_asserter<Expected, Expected> *asserter;

public:
    assert_false(Grammar &grammar, const Space &space): assertion(grammar, space)
    {
        asserter = new asserter::equals<Expected, Expected>();
    }

    ~assert_false()
    {
        delete asserter;
    }

    bool operator() (std::string input, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(false), message);
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

    bool operator() (std::string input, Expected expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(expected), message);
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

    bool operator() (std::string input, Expected expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(expected), message);
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

    bool operator() (std::string input, Expected expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(expected), message);
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

    bool operator() (std::string input, Expected expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(expected), message);
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

    bool operator() (std::string input, Expected expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(expected), message);
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

    bool operator() (std::string input, Expected expected, std::string message)
    {
        return assertion(*asserter, input, value_bag<Expected>(expected), message);
    }
};

template<typename Grammar, typename Space>
class assert_parse_failure
{
private:
    assertion<Grammar, Space> assertion;

public:
    assert_parse_failure(Grammar &grammar, const Space &space): assertion(grammar, space)
    {}

    ~assert_parse_failure()
    {}

    bool operator() (std::string input, std::string message)
    {
        return assertion(input, message);
    }
};

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::success(std::string message)
{
    // fg.white(37); bg.green(42);
    std::cerr << "[ \e[5;37;42mOK\e[0m ] - \e[1;32m" << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
template<typename Type>
void
assertion<Grammar,Space>::error(std::string message, Type &expected)
{
    // fg.white(31); bg.red(47);
    std::cerr << "[ \e[5;31;47mERROR\e[0m ] - \e[1;37m" << message << "\e[0m" << std::endl;
    std::cerr << "Expectation was : [ \e[5;31;47m" << expected << "\e[0m ]" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::failure(std::string message)
{
    // fg.white(37); bg.red(41);
    std::cerr << "[ \e[5;37;41mFAILURE\e[0m ] - \e[1;31m" << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
template<typename Type>
void
assertion<Grammar,Space>::failure(std::string message, Type &expected, Type &actual)
{
    // fg.white(37); bg.red(41);
    std::cerr << "[ \e[5;37;41mFAILURE\e[0m ] - \e[1;31m" << message << "\e[0m" << std::endl;
    std::cerr << "Expectation was :  [ \e[5;30;46m" << expected << "\e[0m ]" << std::endl;
    std::cerr << "Actual value was : [ \e[5;31;47m" << actual << "\e[0m ]" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::ignore(std::string message)
{
    // fg.black(30); bg.cyan(46);
    std::cerr << "[ \e[5;30;46mIGNORE\e[0m ] - \e[1;36m" << message << "\e[0m" << std::endl;
}

template<typename Grammar, typename Space>
void
assertion<Grammar,Space>::exception(std::string message)
{
    // fg.magenta(35); bg.black(40);
    std::cerr << "[ \e[5;35;40mEXCEPTIO\e0m ] - \e[1;30m" << message << "\e[0m" << std::endl;
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