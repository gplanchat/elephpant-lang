
#include "../engine/engine.h"
#include "../engine/parser/ast.h"
#include "../engine/parser/php.h"
#include "tests.h"
#include "string.h"

namespace rephp {

namespace test {

namespace parser {

using boost::spirit::ascii::space;
using boost::spirit::ascii::space_type;
namespace ast = rephp::engine::parser::ast;
namespace type = rephp::engine::type;

double_quoted_string::double_quoted_string(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"\"");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t(), "double quoted empty string");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"Hello world\"");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Hello world"), "double quoted string");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"Hell'o world\"");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Hell'o world"), "double quoted string, with inline single quote");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"Hell\\\"o\\\" world\"");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Hell\"o\" world"), "double quoted string, with inline double quote");
    }));
}

simple_quoted_string::simple_quoted_string(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("''");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t(), "simple quoted empty string");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("'Hello world'");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Hello world"), "simple quoted string");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("'Hell\\'o world'");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Hell'o world"), "simple quoted string, with inline single quote");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("'Hell\\\"o\\\" world'");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Hell\"o\" world"), "simple quoted string, with inline double quote");
    }));
}

string::string(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"\"");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("")), "double quoted empty string (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"Hello world\"");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("Hello world")), "double quoted string (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"Hell'o world\"");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("Hell'o world")), "double quoted string, with inline single quote (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("\"Hell\\\"o\\\" world\"");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("Hell\"o\" world")), "double quoted string, with inline single quote (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("''");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("")), "simple quoted empty string (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("'Hello world'");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("Hello world")), "simple quoted string (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("'Hell\\'o world'");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("Hell'o world")), "simple quoted string, with inline single quote (as expression)");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("'Hell\\\"o\\\" world'");

        assert_equals<grammar_t,space_type,ast::expression_t> is_equal(parser, space);
        is_equal(sources, ast::expression_t(std::string("Hell\"o\" world")), "simple quoted string, with inline double quote (as expression)");
    }));
}

}; // namespace parser

};

};