
#include "visitors.h"

std::ostream &operator<< (std::ostream &ios, rephp::engine::type::internal_variant_t &var)
{
    using namespace rephp::engine::type;
    using namespace rephp::engine::visitor;

    internal_value_stream_visitor visitor;

    ios << apply_visitor(visitor, var);

    return ios;
}