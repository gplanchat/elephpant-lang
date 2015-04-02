

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>

#include "../engine/engine.h"
#include "../engine/parser/php.h"
#include "../engine/reflection.h"
#include "../lang/standard.h"

#include "scalar.h"
#include "string.h"
#include "object.h"

std::map<std::string, size_t> rephp::engine::parser::ast::node::counters;

int main()
{
    using namespace rephp::engine;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    namespace test = rephp::test::parser;

    using qi::parse;
    using ascii::space;

    auto be_standard = type::bundle_t(new rephp::lang::standard_bundle("core"));

    engine::register_bundle(be_standard);

    test::null null;
    test::boolean boolean;
    test::integer integer;
    test::real real;
    test::scalar scalar;
    test::double_quoted_string double_quoted_string;
    test::simple_quoted_string simple_quoted_string;
    test::string string;
    test::class_identifier identifier;
    test::object_construct construct;

    null.suite();
    boolean.suite();
    integer.suite();
    real.suite();
    scalar.suite();
    double_quoted_string.suite();
    simple_quoted_string.suite();
    string.suite();
    identifier.suite();
    construct.suite();

    return 0;
}