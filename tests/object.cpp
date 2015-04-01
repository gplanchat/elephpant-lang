
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

        assert_not_equals<grammar_t,space_type,type::string_t> is_not_equal(parser, space);
        is_not_equal(sources, type::string_t("2Foo"), "errored class identifier.");
    })) << (std::function<void()>([]() {
        grammar_t parser;
        std::string sources("Foo\\2Foo");

        assert_not_equals<grammar_t,space_type,type::string_t> is_not_equal(parser, space);
        is_not_equal(sources, type::string_t("2Foo"), "errored namespaced class identifier.");
    }));
}

}; // namespace parser

};

};
