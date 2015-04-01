
#ifndef REPHP_ENGINE_STREAMS_H
#define REPHP_ENGINE_STREAMS_H

#include "engine.h"
#include "reflection.h"

inline static ostream &
operator<< (ostream &ios, internal_value &val)
{
    using namespace rephp::engine::visitor;

    auto visitor = internal_value_stream_visitor();
    ios << type::apply_visitor(visitor, rephp::reflection::internal_value::get_raw_value(ve));

    return ios;
}

#endif