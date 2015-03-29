
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
        std::string sources("1337;");

        assert_equals<grammar_t,space_type, type::integer_t> is_equal(parser, space);
        is_equal(sources, "positive integer, 1337.", 1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337;");

        assert_equals<grammar_t,space_type, type::integer_t> is_equal(parser, space);
        is_equal(sources, "negative integer, -1337.", -1337);
    }));
}

real::real(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [13.37].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (dotted), [-13.37].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [1337.].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (dotted), [-1337.].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [.1337].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (dotted), [-.1337].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37e2].", 1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37e2].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37e+2].", 1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37e+2].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37e-2].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37e-2].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37E2].", 1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37E2].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37E+2].", 1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37E+2].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37E-2].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37E-2].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337e2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337e2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337e+2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337e+2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337e-2].", .001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337e-2].", -.001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337.E2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337E2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337E+2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337E+2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337E-2].", .001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337E-2].", -.001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.e2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.e2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.e+2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.e+2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.e-2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.e-2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337.E2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.E2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.E+2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E+2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.E+2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.E-2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E-2;");

        assert_equals<grammar_t,space_type,type::real_t> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.E-2].", -13.37);
    }));
}

scalar::scalar(): suite()
{

    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337;");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, "positive integer (as scalar), 1337.", ast::expression_t(1337L));
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337;");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, "negative integer (as scalar), -1337.", ast::expression_t(-1337L));
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("false");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, "basic boolean (as scalar), false.", ast::expression_t(false));
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("true");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, "basic boolean (as scalar), true.", ast::expression_t(true));
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337;");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [.1337].", ast::expression_t(.1337));
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e-2;");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, "positive double (exp, as scalar), [1337.e-2].", ast::expression_t(13.37));
    }));
}

}; // namespace parser

};

};
