
#ifndef REPHP_TESTS_SCALAR_H
#define REPHP_TESTS_SCALAR_H

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

}; // namespace parser

};

};

#endif