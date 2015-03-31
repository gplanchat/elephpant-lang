
#ifndef REPHP_TESTS_STRING_H
#define REPHP_TESTS_STRING_H

#include "tests.h"

namespace rephp {

namespace test {

namespace parser {

namespace grammar = rephp::engine::parser::grammar::php;
using boost::spirit::ascii::space_type;

class double_quoted_string
{
public:
    typedef grammar::double_quoted_string<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    double_quoted_string();
};

class simple_quoted_string
{
public:
    typedef grammar::simple_quoted_string<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    simple_quoted_string();
};

class string
{
public:
    typedef grammar::string<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    string();
};

}; // namespace parser

};

};

#endif
