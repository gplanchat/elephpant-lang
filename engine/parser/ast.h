
#ifndef REPHP_ENGINE_PARSER_AST_H
#define REPHP_ENGINE_PARSER_AST_H

#include <boost/spirit/include/qi.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <list>

#include <cxxabi.h>

#include "../engine.h"
#include "../types.h"
#include "../variant.h"

namespace rephp { namespace engine { namespace parser {

namespace ast {

namespace op {

struct let {
};
struct add {
};
struct sub {
};
struct mul {
};
struct div {
};
struct mod {
};
struct intdiv {
};
struct exp {
};
struct logical_and {
};
struct logical_or {
};
struct logical_xor {
};
struct logical_not {
};
struct bitwise_and {
};
struct bitwise_or {
};
struct bitwise_xor {
};
struct bitwise_not {
};
struct lhs {
};
struct rhs {
};
struct offset {
};
struct concat {
};
struct eq {
};
struct strict_eq {
};
struct neq {
};
struct strict_neq {
};
struct lt {
};
struct gt {
};
struct lteq {
};
struct gteq {
};
struct spaceship {
};
struct increment_pre {
};
struct decrement_pre {
};
struct increment_post {
};
struct decrement_post {
};
struct choice {
};
struct isset {
};
struct grouping {
};

}; // namespace op

namespace control {

struct if_ {
};
struct else_ {
};
struct while_ {
};
struct do_while_ {
};
struct for_ {
};
struct foreach_ {
};
struct try_ {
};
struct catch_ {
};
struct finally_ {
};

} // namespace control

namespace engine = rephp::engine;

template<typename Tag>
class unary_op;

template<typename Tag>
class binary_op;

template<typename Tag>
class ternary_op;

template<typename Tag>
class control_structure;

class array;

class array_item;

class variable;

class class_identifier;

class statement_list;

class program;

class object_construct;

typedef engine::type::infinite_variant<
        engine::type::null_t,
        engine::type::boolean_t,
        engine::type::integer_t,
        engine::type::real_t,
        engine::type::string_t,
        engine::type::recursive_wrapper<array>,
        variable,
        object_construct//,

//    engine::type::recursive_wrapper<binary_op<op::let>>,
//    engine::type::recursive_wrapper<unary_op<op::add>>,
//    engine::type::recursive_wrapper<unary_op<op::sub>>,
//    engine::type::recursive_wrapper<binary_op<op::add>>,
//    engine::type::recursive_wrapper<binary_op<op::sub>>,
//    engine::type::recursive_wrapper<binary_op<op::mul>>,
//    engine::type::recursive_wrapper<binary_op<op::div>>,
//    engine::type::recursive_wrapper<binary_op<op::mod>>,
//    engine::type::recursive_wrapper<binary_op<op::intdiv>>,
//    engine::type::recursive_wrapper<binary_op<op::exp>>,
//    engine::type::recursive_wrapper<binary_op<op::logical_and>>,
//    engine::type::recursive_wrapper<binary_op<op::logical_or>>,
//    engine::type::recursive_wrapper<binary_op<op::logical_xor>>,
//    engine::type::recursive_wrapper<unary_op<op::logical_not>>,
//    engine::type::recursive_wrapper<binary_op<op::bitwise_and>>,
//    engine::type::recursive_wrapper<binary_op<op::bitwise_or>>,
//    engine::type::recursive_wrapper<binary_op<op::bitwise_xor>>,
//    engine::type::recursive_wrapper<unary_op<op::bitwise_not>>,
//    engine::type::recursive_wrapper<binary_op<op::lhs>>,
//    engine::type::recursive_wrapper<binary_op<op::rhs>>,
//    engine::type::recursive_wrapper<binary_op<op::offset>>,
//    engine::type::recursive_wrapper<binary_op<op::concat>>,
//    engine::type::recursive_wrapper<binary_op<op::eq>>,
//    engine::type::recursive_wrapper<binary_op<op::neq>>,
//    engine::type::recursive_wrapper<binary_op<op::strict_eq>>,
//    engine::type::recursive_wrapper<binary_op<op::strict_neq>>,
//    engine::type::recursive_wrapper<binary_op<op::lt>>,
//    engine::type::recursive_wrapper<binary_op<op::gt>>,
//    engine::type::recursive_wrapper<binary_op<op::gteq>>,
//    engine::type::recursive_wrapper<binary_op<op::lteq>>,
//    engine::type::recursive_wrapper<binary_op<op::spaceship>>,
//    engine::type::recursive_wrapper<unary_op<op::increment_pre>>,
//    engine::type::recursive_wrapper<unary_op<op::decrement_pre>>,
//    engine::type::recursive_wrapper<unary_op<op::increment_post>>,
//    engine::type::recursive_wrapper<unary_op<op::decrement_post>>,
//    engine::type::recursive_wrapper<ternary_op<op::choice>>,
//    engine::type::recursive_wrapper<ternary_op<op::isset>>,
//    engine::type::recursive_wrapper<unary_op<op::grouping>>
> expression_t;

typedef engine::type::infinite_variant<
        engine::type::recursive_wrapper<control_structure<control::if_>>,
        engine::type::recursive_wrapper<control_structure<control::else_>>,
        engine::type::recursive_wrapper<control_structure<control::while_>>,
        engine::type::recursive_wrapper<control_structure<control::do_while_>>,
        engine::type::recursive_wrapper<control_structure<control::for_>>,
        engine::type::recursive_wrapper<control_structure<control::foreach_>>,
        engine::type::recursive_wrapper<control_structure<control::try_>>,
        engine::type::recursive_wrapper<control_structure<control::catch_>>,
        engine::type::recursive_wrapper<control_structure<control::finally_>>,

        expression_t
> statement_t;

std::ostream &expression_dump(std::ostream &ios, expression_t &expr, size_t offset = 0);

std::ostream &expression_dump(std::ostream &ios, std::shared_ptr<expression_t> expr, size_t offset = 0);

}; //namespace ast

}; }; };

std::ostream &operator<< (std::ostream &ios, rephp::engine::parser::ast::expression_t &expr);
std::ostream &operator<< (std::ostream &ios, rephp::engine::parser::ast::class_identifier &identifier);
std::ostream &operator<< (std::ostream &ios, rephp::engine::parser::ast::object_construct &construct);

namespace rephp { namespace engine { namespace parser {

namespace ast {

namespace type = rephp::engine::type;

class node
{
    static std::map<std::string, size_t> counters;
    size_t identifier;

public:
    size_t get_identifier() const
    {
        return identifier;
    }

    template<typename T>
    void debug(T *self, const char * const qualifier)
    {
        std::string q(qualifier);
        debug(self, q);
    }

    template<typename T>
    void debug(T *self, std::string &qualifier)
    {
        int status;
        auto name = typeid(*self).name();
        auto unmangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        auto current = counters[name];

        identifier = ++current;

        counters[name] = current;

        std::cout << "[" << self << "]  " << unmangled << " (" << current << ") > " << qualifier << std::endl;
    }
};

class variable: public node
{
public:
    std::string name;

    variable(): node(), name()
    {}

    variable(const char *name): node(), name(name)
    {}

    variable(const std::string &name): node(), name(name)
    {}

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "variable: { " << name << " }";
    }

    variable &operator= (const variable &self)
    {
        name = self.name;

        return *this;
    }

    variable &operator= (const std::string &new_name)
    {
        name = new_name;

        return *this;
    }

    bool operator== (const variable &) const
    {
        return false;
    }

    bool operator== (variable)
    {
        return false;
    }

    bool operator!= (const variable &) const
    {
        return true;
    }

    bool operator!= (variable)
    {
        return true;
    }
};

class class_identifier: public node
{
public:
    std::string name;

    class_identifier(): node(), name()
    {}

    class_identifier(const char *name): node(), name(name)
    {}

    class_identifier(const std::string &name): node(), name(name)
    {}

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "class-ientifier: { " << name << " }";
    }

    class_identifier &operator= (const class_identifier &self)
    {
        name = self.name;

        return *this;
    }

    class_identifier &operator= (const std::string &new_name)
    {
        name = new_name;

        return *this;
    }

    bool operator== (const class_identifier &other) const
    {
        return name == other.name;
    }

    bool operator== (class_identifier other)
    {
        return name == other.name;
    }

    bool operator!= (const class_identifier &other) const
    {
        return name != other.name;
    }

    bool operator!= (class_identifier other)
    {
        return name != other.name;
    }
};

class object_construct: public node
{
public:
    class_identifier identifier;

    object_construct(): node(), identifier()
    {}

    object_construct(class_identifier identifier): node(), identifier(identifier)
    {}

    object_construct(const object_construct &self): node(), identifier(self.identifier)
    {}

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "object-construct: { " << identifier << " }";
    }

    object_construct &operator= (const class_identifier &identifier)
    {
        this->identifier = identifier;

        return *this;
    }

    object_construct &operator= (const object_construct &self)
    {
        identifier = self.identifier;

        return *this;
    }

    bool operator== (const object_construct &other) const
    {
        return identifier == other.identifier;
    }

    bool operator== (object_construct other)
    {
        return identifier == other.identifier;
    }

    bool operator!= (const object_construct &other) const
    {
        return identifier != other.identifier;
    }

    bool operator!= (object_construct other)
    {
        return identifier != other.identifier;
    }
};

template<typename Tag>
class unary_op: public node
{
public:
    std::shared_ptr<expression_t> operand;

    unary_op(std::shared_ptr<expression_t> operand): node(), operand(operand)
    {
        debug(this, "standard(expression_t&)");
    }

    bool operator== (const unary_op &) const
    {
        return false;
    }

    bool operator== (unary_op)
    {
        return false;
    }

    bool operator!= (const unary_op &) const
    {
        return true;
    }

    bool operator!= (unary_op)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "unary-operator(" << typeid(Tag).name() << "): {\n"
            << padding << expression_dump(ios, operand, offset + 1) << '\n'
            << padding << "}"
        ;
    }
};

template<typename Tag>
class binary_op: public node
{
public:
    std::shared_ptr<expression_t> first;
    std::shared_ptr<expression_t> second;

    binary_op(std::shared_ptr<expression_t> first, std::shared_ptr<expression_t> second): node(), first(first), second(second)
    {
        debug(this, "standard(expression_t&, expression_t&)");
    }

    bool operator== (const binary_op &) const
    {
        return false;
    }

    bool operator== (binary_op)
    {
        return false;
    }

    bool operator!= (const binary_op &) const
    {
        return true;
    }

    bool operator!= (binary_op)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "binary-operator(" << typeid(Tag).name() << "): {\n"
            << padding << expression_dump(ios, first, offset + 1) << ",\n"
            << padding << expression_dump(ios, second, offset + 1) << '\n'
            << padding << "}"
        ;
    }
};

template<typename Tag>
class ternary_op: public node
{
public:
    std::shared_ptr<expression_t> first;
    std::shared_ptr<expression_t> second;
    std::shared_ptr<expression_t> third;

    ternary_op(std::shared_ptr<expression_t> first, std::shared_ptr<expression_t> second, std::shared_ptr<expression_t> third):
        node(), first(first), second(second), third(third)
    {
        debug(this, "standard(expression_t&, expression_t&, expression_t&)");
    }

    bool operator== (const ternary_op &) const
    {
        return false;
    }

    bool operator== (ternary_op)
    {
        return false;
    }

    bool operator!= (const ternary_op &) const
    {
        return true;
    }

    bool operator!= (ternary_op)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "ternary-operator(" << typeid(Tag).name() << "): {\n"
            << padding << expression_dump(ios, first, offset + 1) << ",\n"
            << padding << expression_dump(ios, second, offset + 1) << ",\n"
            << padding << expression_dump(ios, third, offset + 1) << '\n'
            << padding << "}"
        ;
    }
};

template<typename Tag>
class control_structure: public node
{
public:
    std::shared_ptr<expression_t>           condition;
    std::list<std::shared_ptr<statement_t>> statement_list;

    control_structure(std::shared_ptr<expression_t> condition, std::list<std::shared_ptr<statement_t>> statement_list):
        node(), condition(condition), statement_list(statement_list)
    {}

    bool operator== (const control_structure &) const
    {
        return false;
    }

    bool operator== (control_structure)
    {
        return false;
    }

    bool operator!= (const control_structure &) const
    {
        return true;
    }

    bool operator!= (control_structure)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        return ios << padding << "control-structure(" << typeid(Tag).name() << "): {\n"
            << padding << expression_dump(ios, condition, offset + 1) << ",\n"
            << padding << expression_dump(ios, statement_list, offset + 1) << '\n'
            << padding << "}"
        ;
    }
};

class array_item: public node
{
public:
    std::shared_ptr<expression_t> key;
    std::shared_ptr<expression_t> value;

    array_item(): node(), key(new expression_t(false)), value(new expression_t(false))
    {
        debug(this, "standard");
    }

    array_item(std::shared_ptr<expression_t> key, std::shared_ptr<expression_t> value): node(), key(key), value(value)
    {
        debug(this, "standard(expression_t&, expression_t&)");
    }

    array_item(std::shared_ptr<expression_t> value): node(), key(new expression_t(false)), value(value)
    {
        debug(this, "standard(expression_t&)");
    }

    array_item(const array_item &self): node(), key(self.key), value(self.value)
    {
        debug(this, "const copy");
    }

    array_item(array_item &&self): node(), key(self.key), value(self.value)
    {
        debug(this, "copy");
    }

    array_item &operator= (const array_item &self)
    {
        this->key = self.key;
        this->value = self.value;
        return *this;
    }

    array_item &operator= (array_item &&self)
    {
        this->key = self.key;
        this->value = self.value;
        return *this;
    }

    bool operator== (const array_item &) const
    {
        return false;
    }

    bool operator== (array_item)
    {
        return false;
    }

    bool operator!= (const array_item &) const
    {
        return true;
    }

    bool operator!= (array_item)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');
        ios << padding << "array-item: {\n";
        expression_dump(ios, key, offset + 1) << "\n";
        expression_dump(ios, value, offset + 1) << "\n";
        ios << padding << "}";

        return ios;
    }
};

class array: public node
{
public:
    std::shared_ptr<list<std::shared_ptr<array_item>>> items;

    array(const array &self): node(), items(self.items)
    {}

    array(array &&self): node(), items(self.items)
    {}

    array(): node(), items(new list<std::shared_ptr<array_item>>)
    {}

    array &operator= (const array &self)
    {
        this->items = self.items;
        return *this;
    }

    array &operator= (array &&self)
    {
        this->items = self.items;
        return *this;
    }

    bool operator== (const array &) const
    {
        return false;
    }

    bool operator== (array)
    {
        return false;
    }

    bool operator!= (const array &) const
    {
        return true;
    }

    bool operator!= (array)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');

        ios << padding << "array(" << items->size() << ") #" << get_identifier() << ": {\n";

        for (auto it = items->begin(); it != items->end();) {
            (*it)->dump(ios, offset + 1);

            if (++it == items->end()) {
                ios << "\n";
            } else {
                ios << ",\n";
            }
        };
        ios << padding << "}";
        return ios;
    }
};

class statement_list: public node
{
public:
    std::shared_ptr<list<std::shared_ptr<expression_t>>> statements;

    statement_list(const statement_list &self): node(), statements(self.statements)
    {}

    statement_list(statement_list &&self): node(), statements(self.statements)
    {}

    statement_list(): node(), statements(new list<std::shared_ptr<expression_t>>)
    {}

    statement_list &operator= (const statement_list &self)
    {
        statements = self.statements;
        return *this;
    }

    statement_list &operator= (statement_list &&self)
    {
        statements = self.statements;
        return *this;
    }

    bool operator== (const statement_list &) const
    {
        return false;
    }

    bool operator== (statement_list)
    {
        return false;
    }

    bool operator!= (const statement_list &) const
    {
        return true;
    }

    bool operator!= (statement_list)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');

        ios << padding << "statement-list: {\n";
        for (auto it = statements->begin(); it != statements->end();) {
            expression_dump(ios, *it, offset + 1);

            if (++it == statements->end()) {
                ios << "\n";
            } else {
                ios << ",\n";
            }
        };

        ios << padding << "}";
        return ios;
    }
};

class program: public node
{
public:
    std::shared_ptr<list<std::shared_ptr<expression_t>>> expressions;

    program(const program &self): node(), expressions(self.expressions)
    {}

    program(program &&self): node(), expressions(self.expressions)
    {}

    program(): node(), expressions(new list<std::shared_ptr<expression_t>>)
    {}

    program &operator= (const program &self)
    {
        expressions = self.expressions;
        return *this;
    }

    program &operator= (program &&self)
    {
        expressions = self.expressions;
        return *this;
    }

    bool operator== (const program &) const
    {
        return false;
    }

    bool operator== (program)
    {
        return false;
    }

    bool operator!= (const program &) const
    {
        return true;
    }

    bool operator!= (program)
    {
        return true;
    }

    std::ostream &dump(std::ostream &ios, size_t offset = 0)
    {
        auto padding = std::string(offset * 4, ' ');

        ios << padding << "program: {\n";
        for (auto it = expressions->begin(); it != expressions->end();) {
            expression_dump(ios, *it, offset + 1);

            if (++it == expressions->end()) {
                ios << "\n";
            } else {
                ios << ",\n";
            }
        };

        ios << padding << "}";
        return ios;
    }
};

}; // namespace ast

}; }; };

#endif
