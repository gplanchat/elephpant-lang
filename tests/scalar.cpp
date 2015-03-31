
#include "../engine/engine.h"
#include "../engine/parser/ast.h"
#include "../engine/parser/php.h"
#include "tests.h"
#include "scalar.h"


namespace rephp {

namespace test {

namespace parser {

using boost::spirit::ascii::space;
using boost::spirit::ascii::space_type;
namespace ast = rephp::engine::parser::ast;
namespace type = rephp::engine::type;

null::null(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("null");

        assert_variant_is_type<grammar_t,space_type,ast::expression_t,type::null_t> is_type(parser, space);
        is_type(sources, type::null_t(), "null value.");
    }));
}

boolean::boolean(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("false");

        assert_false<grammar_t,space_type,type::boolean_t> is_false(parser, space);
        is_false(sources, "basic boolean, false.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("true");

        assert_true<grammar_t,space_type,type::boolean_t> is_true(parser, space);
        is_true(sources, "basic boolean, true.");
    }));
}

integer::integer(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337");

        assert_equals<grammar_t,space_type, type::integer_t> is_equal(parser, space);
        is_equal(sources, 1337, "positive integer, 1337.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337");

        assert_equals<grammar_t,space_type, type::integer_t> is_equal(parser, space);
        is_equal(sources, -1337, "negative integer, -1337.");
    }));
}

real::real(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (dotted), [13.37].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (dotted), [-13.37].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -1337., "positive double (dotted), [1337.].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -1337., "negative double (dotted), [-1337.].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, .1337, "positive double (dotted), [.1337].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -.1337, "negative double (dotted), [-.1337].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 1337., "positive double (exp), [13.37e2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -1337., "negative double (exp), [-13.37e2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 1337., "positive double (exp), [13.37e+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -1337., "negative double (exp), [-13.37e+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, .1337, "positive double (exp), [13.37e-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -.1337, "negative double (exp), [-13.37e-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 1337., "positive double (exp), [13.37E2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -1337., "negative double (exp), [-13.37E2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 1337., "positive double (exp), [13.37E+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -1337., "negative double (exp), [-13.37E+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, .1337, "positive double (exp), [13.37E-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -.1337, "negative double (exp), [-13.37E-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (exp), [.1337e2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (exp), [-.1337e2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (exp), [.1337e+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (exp), [-.1337e+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, .001337, "positive double (exp), [.1337e-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -.001337, "negative double (exp), [-.1337e-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (exp), [.1337.E2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (exp), [-.1337E2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (exp), [.1337E+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (exp), [-.1337E+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, .001337, "positive double (exp), [.1337E-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -.001337, "negative double (exp), [-.1337E-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 133700, "positive double (exp), [1337.e2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -133700, "negative double (exp), [-1337.e2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 133700, "positive double (exp), [1337.e+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -133700, "negative double (exp), [-1337.e+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (exp), [1337.e-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (exp), [-1337.e-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 133700, "positive double (exp), [.1337.E2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -133700, "negative double (exp), [-1337.E2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 133700, "positive double (exp), [1337.E+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E+2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -133700, "negative double (exp), [-1337.E+2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, 13.37, "positive double (exp), [1337.E-2].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E-2");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, -13.37, "negative double (exp), [-1337.E-2].");
    }));
}

scalar::scalar(): suite()
{

    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(1337L), "positive integer (as scalar), 1337.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(-1337L), "negative integer (as scalar), -1337.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("false");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(false), "basic boolean (as scalar), false.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("true");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(true), "basic boolean (as scalar), true.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(.1337), "positive double (dotted), [.1337].");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e-2");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(13.37), "positive double (exp, as scalar), [1337.e-2].");
    }));
}

}; // namespace parser

};

};
