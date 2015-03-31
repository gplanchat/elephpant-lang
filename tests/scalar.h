
#ifndef REPHP_TESTS_SCALAR_H
#define REPHP_TESTS_SCALAR_H

#include "tests.h"

namespace rephp {

namespace test {

namespace parser {

namespace grammar = rephp::engine::parser::grammar::php;
using boost::spirit::ascii::space_type;

class null
{
public:
    typedef grammar::null<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    null();
};

class boolean
{
public:
    typedef grammar::boolean<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    boolean();
};

class integer
{
public:
    typedef grammar::integer<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    integer();
};

class real
{
public:
    typedef grammar::real<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    real();
};

class scalar
{
public:
    typedef grammar::scalar<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    scalar();
};

}; // namespace parser

};

};

#endif