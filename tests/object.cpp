
#include "../engine/engine.h"
#include "../engine/parser/ast.h"
#include "../engine/parser/php.h"
#include "tests.h"
#include "object.h"


namespace rephp {

namespace test {

namespace parser {

using boost::spirit::ascii::space;
using boost::spirit::ascii::space_type;
namespace ast = rephp::engine::parser::ast;
namespace type = rephp::engine::type;

class_identifier::class_identifier(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("Foo");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Foo"), "simple class identifier.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("Foo\\Bar");

        assert_equals<grammar_t,space_type,type::string_t> is_equal(parser, space);
        is_equal(sources, type::string_t("Foo\\Bar"), "namespaced class identifier.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("2Foo");

        assert_parse_failure<grammar_t,space_type> parse_failure(parser, space);
        parse_failure(sources, "errored class identifier.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("Foo\\2Foo");

        assert_parse_failure<grammar_t,space_type> parse_failure(parser, space);
        parse_failure(sources, "namespaced errored class identifier.", false);
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("2Foo\\Foo");

        assert_parse_failure<grammar_t,space_type> parse_failure(parser, space);
        parse_failure(sources, "errored namespaced class identifier.");
    }));
}

}; // namespace parser

};

};
