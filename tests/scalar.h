
#ifndef REPHP_TESTS_SCALAR_H
#define REPHP_TESTS_SCALAR_H

#include "tests.h"

namespace rephp {

namespace test {

namespace parser {

namespace grammar = rephp::engine::parser::grammar::php;
using boost::spirit::ascii::space_type;

class boolean: public test_suite
{
private:
    typedef grammar::boolean<std::string::iterator, space_type> grammar_t;

    test_suite suite;

public:
    boolean();
};

class integer: test_suite
{
private:
    typedef grammar::boolean<std::string::iterator, space_type> grammar_t;

    test_suite suite;

public:
    integer();
};

class real: test_suite
{
private:
    typedef grammar::boolean<std::string::iterator, space_type> grammar_t;

    test_suite suite;

public:
    real();
};

}; // namespace parser

};

};

#endif