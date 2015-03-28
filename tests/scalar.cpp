
#include "../engine/engine.h"
#include "../engine/parser/php.h"
#include "tests.h"
#include "scalar.h"


namespace rephp {

namespace test {

namespace parser {

using boost::spirit::ascii::space;
using boost::spirit::ascii::space_type;

boolean::boolean(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("false;");

        assert_false<grammar_t,space_type> is_false(parser, space);
        is_false(sources, "basic boolean, false.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("false;");

        assert_true<grammar_t,space_type> is_true(parser, space);
        is_true(sources, "basic boolean, false.");
    }));
}

integer::integer(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337;");

        assert_equals<grammar_t,space_type, long> is_equal(parser, space);
        is_equal(sources, "positive integer, 1337.", 1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337;");

        assert_equals<grammar_t,space_type, long> is_equal(parser, space);
        is_equal(sources, "negative integer, -1337.", -1337);
    }));
}

real::real(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [13.37].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (dotted), [-13.37].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [1337.].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (dotted), [-1337.].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (dotted), [.1337].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (dotted), [-.1337].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37e2].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37e2].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37e+2].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37e+2].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37e-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37e-2].", 1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37e-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37e-2].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37E2].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37E2].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37E+2].", .1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37E+2].", -.1337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("13.37E-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [13.37E-2].", 1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-13.37E-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-13.37E-2].", -1337.);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337e2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337e2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337e+2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337e+2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337e-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337e-2].", .001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337e-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337e-2].", -.001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337.E2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337E2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337E+2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337E+2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources(".1337E-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337E-2].", .001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-.1337E-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-.1337E-2].", -.001337);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.e2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.e2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.e+2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.e+2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.e-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.e-2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.e-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.e-2].", -13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [.1337.E2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.E2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.E+2].", 133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E+2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.E+2].", -133700);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("1337.E-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "positive double (exp), [1337.E-2].", 13.37);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("-1337.E-2;");

        assert_equals<grammar_t,space_type,double> is_equal(parser, space);
        is_equal(sources, "negative double (exp), [-1337.E-2].", -13.37);
    }));
}

}; // namespace parser

};

};
