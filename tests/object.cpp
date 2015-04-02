
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

        assert_equals<grammar_t,space_type,ast::class_identifier> is_equal(parser, space);
        is_equal(sources, ast::class_identifier("Foo"), "simple class identifier.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("Foo\\MyClass");

        assert_equals<grammar_t,space_type,ast::class_identifier> is_equal(parser, space);
        is_equal(sources, ast::class_identifier("Foo\\MyClass"), "namespaced class identifier.");
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

object_construct::object_construct(): suite()
{
    suite << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("new Foo");
        ast::class_identifier identifier("Foo");

        assert_equals<grammar_t,space_type,ast::object_construct> is_equal(parser, space);
        is_equal(sources, ast::object_construct(identifier), "simple class identifier.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("new Foo\\MyClass");
        ast::class_identifier identifier("Foo\\MyClass");

        assert_equals<grammar_t,space_type,ast::object_construct> is_equal(parser, space);
        is_equal(sources, ast::object_construct(identifier), "namespaced class identifier.");
    }));
}

}; // namespace parser

};

};
