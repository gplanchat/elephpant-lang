
#ifndef REPHP_TESTS_OBJECT_H
#define REPHP_TESTS_OBJECT_H

#include "tests.h"

namespace rephp {

namespace test {

namespace parser {

namespace grammar = rephp::engine::parser::grammar::php;
using boost::spirit::ascii::space_type;

class class_identifier
{
public:
    typedef grammar::class_identifier<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    class_identifier();
};

class object_construct
{
public:
    typedef grammar::object_construct<std::string::iterator, space_type> grammar_t;

    test_suite suite;

    object_construct();
};

}; // namespace parser

};

};

#endif