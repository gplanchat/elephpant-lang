
#ifndef REPHP_ENGINE_PARSER_PHP_H
#define REPHP_ENGINE_PARSER_PHP_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <functional>
#include <memory>
#include <list>

#include "../engine.h"
#include "ast.h"

namespace rephp { namespace engine {namespace parser {

namespace grammar {

namespace php {

namespace qi = boost::spirit::qi;
namespace pnx = boost::phoenix;
namespace ast = rephp::engine::parser::ast;

template <typename Iterator, typename Skipper>
class null: public qi::grammar<Iterator,ast::expression_t(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        root = qi::lit("null") [ qi::_val = ast::expression_t() ];
    }

public:
    null(const null &): null::base_type(root)
    {
        init();
    }

    null(null &&): null::base_type(root)
    {
        init();
    }

    null(): null::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,ast::expression_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class boolean: public qi::grammar<Iterator,type::boolean_t(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        root %= qi::bool_;
    }

public:
    boolean(const boolean &): boolean::base_type(root)
    {
        init();
    }

    boolean(boolean &&): boolean::base_type(root)
    {
        init();
    }

    boolean(): boolean::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,type::boolean_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class integer: public qi::grammar<Iterator,type::integer_t(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        root %=
            ( qi::no_case [ qi::lit("b")  > qi::bin ] ) |
            ( qi::no_case [ qi::lit("0x") > qi::hex ] ) |
            (               qi::lit("0")  > qi::oct   ) |
            ( qi::long_                               )
        ;
    }

public:
    integer(const integer &): integer::base_type(root)
    {
        init();
    }

    integer(integer &&): integer::base_type(root)
    {
        init();
    }

    integer(): integer::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,type::integer_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class real: public qi::grammar<Iterator,type::real_t(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        qi::real_parser<double, qi::strict_real_policies<double>> strict_double;

        root %= strict_double;
    }

public:
    real(const real &): real::base_type(root)
    {
        init();
    }

    real(real &&): real::base_type(root)
    {
        init();
    }

    real(): real::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,type::real_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class scalar: public qi::grammar<Iterator,ast::expression_t(),Skipper>
{
private:
    void init()
    {
        root %= (boolean_ | real_ | integer_);
    }

public:
    scalar(const scalar &): scalar::base_type(root)
    {
        init();
    }

    scalar(scalar &&): scalar::base_type(root)
    {
        init();
    }

    scalar(): scalar::base_type(root)
    {
        init();
    }

    boolean<Iterator, Skipper> boolean_;
    integer<Iterator, Skipper> integer_;
    real<Iterator, Skipper>    real_;

    qi::rule<Iterator,ast::expression_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class simple_quoted_string: public qi::grammar<Iterator,type::string_t(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        unesc_char.add
            ("\\a",  '\a')
            ("\\b",  '\b')
            ("\\f",  '\f')
            ("\\n",  '\n')
            ("\\r",  '\r')
            ("\\t",  '\t')
            ("\\v",  '\v')
            ("\\\\", '\\')
            ("\\\'", '\'')
            ("\\\"", '\"')
        ;

        content %= qi::as_string [ *( unesc_char | ~qi::char_('\'') ) ];

        root %= qi::no_skip [ qi::lit('\'') >> content >> qi::lit('\'') ];
    }

public:
    simple_quoted_string(const simple_quoted_string &): simple_quoted_string::base_type(root)
    {
        init();
    }

    simple_quoted_string(simple_quoted_string &&): simple_quoted_string::base_type(root)
    {
        init();
    }

    simple_quoted_string(): simple_quoted_string::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,type::string_t()> content;
    qi::rule<Iterator,type::string_t(),Skipper> root;

    qi::symbols<char const, char const> unesc_char;
};

template <typename Iterator, typename Skipper>
class double_quoted_string: public qi::grammar<Iterator,type::string_t(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        unescaped_char.add
            ("\\a",  '\a')
            ("\\b",  '\b')
            ("\\f",  '\f')
            ("\\n",  '\n')
            ("\\r",  '\r')
            ("\\t",  '\t')
            ("\\v",  '\v')
            ("\\\\", '\\')
            ("\\\'", '\'')
            ("\\\"", '\"')
        ;

        content %= qi::as_string [ *( unescaped_char | ~qi::char_('"') ) ];

        root %= qi::no_skip [ qi::lit('"') >> content >> qi::lit('"') ];
    }

public:
    double_quoted_string(const double_quoted_string &): double_quoted_string::base_type(root)
    {
        init();
    }

    double_quoted_string(double_quoted_string &&): double_quoted_string::base_type(root)
    {
        init();
    }

    double_quoted_string(): double_quoted_string::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,type::string_t()> content;
    qi::rule<Iterator,type::string_t(),Skipper> root;

    qi::symbols<char const, char const> unescaped_char;
};

template <typename Iterator, typename Skipper>
class string: public qi::grammar<Iterator,ast::expression_t(),Skipper>
{
private:
    void init()
    {
        root %= (simple_quoted | double_quoted);
    }

public:
    string(const string &): string::base_type(root)
    {
        init();
    }

    string(string &&): string::base_type(root)
    {
        init();
    }

    string(): string::base_type(root)
    {
        init();
    }

    simple_quoted_string<Iterator, Skipper> simple_quoted;
    double_quoted_string<Iterator, Skipper> double_quoted;

    qi::rule<Iterator,ast::expression_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class code_expression: public qi::grammar<Iterator,ast::expression_t(),Skipper>
{
private:
    void init()
    {
        root %= (scalar | string);
    }

public:
    code_expression(const code_expression &): code_expression::base_type(root)
    {
        init();
    }

    code_expression(code_expression &&): code_expression::base_type(root)
    {
        init();
    }

    code_expression(): code_expression::base_type(root)
    {
        init();
    }

    scalar<Iterator, Skipper> scalar;
    string<Iterator, Skipper> string;

    qi::rule<Iterator,ast::expression_t(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class class_identifier: public qi::grammar<Iterator,ast::class_identifier(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        base_identifier %= qi::lexeme [ qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z0-9_") ];

        root %= qi::as_string [ base_identifier >> *(qi::char_('\\') >> base_identifier) ];
    }

public:
    class_identifier(const class_identifier &): class_identifier::base_type(root)
    {
        init();
    }

    class_identifier(class_identifier &&): class_identifier::base_type(root)
    {
        init();
    }

    class_identifier(): class_identifier::base_type(root)
    {
        init();
    }

    qi::rule<Iterator,type::string_t(),Skipper> base_identifier;
    qi::rule<Iterator,ast::class_identifier(),Skipper> root;
};

template <typename Iterator, typename Skipper>
class object_construct: public qi::grammar<Iterator,ast::object_construct(),Skipper>
{
private:
    void init()
    {
        namespace qi = boost::spirit::qi;

        root %= qi::lit("new") >> identifier >> !(qi::char_('(') >> (expression % ',') >> qi::char_(')'));
    }

public:
    object_construct(const object_construct &): object_construct::base_type(root)
    {
        init();
    }

    object_construct(object_construct &&): object_construct::base_type(root)
    {
        init();
    }

    object_construct(): object_construct::base_type(root)
    {
        init();
    }

    class_identifier<Iterator, Skipper> identifier;
    code_expression<Iterator, Skipper> expression;

    qi::rule<Iterator,ast::object_construct(),Skipper> root;
};

} // namespace php

} // namespace grammar

}; }; };

#endif
