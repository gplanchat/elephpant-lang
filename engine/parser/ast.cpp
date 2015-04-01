
#include "ast.h"


namespace rephp { namespace engine { namespace parser {

namespace ast {

std::ostream &expression_dump(std::ostream &ios, expression_t &expr, size_t offset)
{
    auto padding = std::string(offset * 4, ' ');
    if (type::variant_is_type<type::null_t>(expr)) {
        ios << padding << "null";
    } else if (type::variant_is_type<type::boolean_t>(expr)) {
        ios << padding << "boolean(" << (type::get<type::boolean_t>(expr) ? "true" : "false") << ")";
    } else if (type::variant_is_type<type::integer_t>(expr)) {
        ios << padding << "integer(" << type::get<type::integer_t>(expr) << ")";
    } else if (type::variant_is_type<type::real_t>(expr)) {
        ios << padding << "real(" << type::get<type::real_t>(expr) << ")";
    } else if (type::variant_is_type<type::string_t>(expr)) {
        type::string_t str(type::get<type::string_t>(expr));
        ios << padding << "string(" << str.size() << "): \"" << str << "\"";
    } else if (type::variant_is_type<ast::array_item>(expr)) {
        type::get<ast::array_item>(expr).dump(ios, offset);
    } else if (type::variant_is_type<ast::array>(expr)) {
        type::get<ast::array>(expr).dump(ios, offset);
    } else {
        ios << padding << "expression";
    }
    return ios;
}

std::ostream &expression_dump(std::ostream &ios, std::shared_ptr<expression_t> expr, size_t offset)
{
    expression_dump(ios, *expr, offset);

    return ios;
}

}; // namespace ast

}; }; };

std::ostream &operator<< (std::ostream &ios, rephp::engine::parser::ast::expression_t &expr)
{
    expression_dump(ios, expr);

    return ios;
}