
#ifndef REPHP_REFLECTION_H
#define REPHP_REFLECTION_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "types.h"
#include "engine.h"
#include "visitors.h"

namespace rephp {

namespace engine {

class worker;
class callable_prototype;
class method_entry;
class internal_type;
class internal_value;
class bundle;

};

namespace reflection {

using namespace std;
using namespace rephp::engine::type;

class callable_prototype
{
public:
    inline static type_t get_return_type(prototype_t pe)
    {
        return pe->return_type;
    }

    inline static parameters_map_t get_parameters_map(prototype_t pe)
    {
        return pe->parameters;
    }

    inline static void walk_parameters_map(prototype_t pe, std::function<void(type_t,string_t,value_t)> callback)
    {
        auto map = get_parameters_map(pe);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_parameter_type(*it), get_key(*it), get_parameter_value(*it));
        }
    }

    inline static std::ostream &dump(std::ostream &ios, prototype_t pe);
};

class method_entry
{
public:
    inline static const prototype_t get_prototype(shared_ptr<rephp::engine::method_entry> me)
    {
        return me->prototype;
    }

    inline static const worker_t get_worker_callable(method_t me)
    {
        return me->worker;
    }

    inline static access_mode_t get_access(method_t me)
    {
        return me->access;
    }
};

class internal_value
{
public:
    inline static const type_t get_type(value_t &ve)
    {
        return ve->type;
    }

    inline static const value_attributes_map_t &get_attributes_map(value_t &ve)
    {
        return ve->attributes;
    }

    inline static const internal_variant_t &get_raw_value(value_t &ve)
    {
        return ve->raw_value;
    }

    inline static std::ostream &dump(std::ostream &ios, value_t ve);
};

class internal_type
{
public:
    inline static const type_t get_parent(type_t te)
    {
        return te->parent;
    }

    inline static const classes_map_t &get_interfaces_map(type_t te)
    {
        return te->interfaces;
    }

    inline static const classes_map_t &get_traits_map(type_t te)
    {
        return te->traits;
    }

    inline static const string_t &get_package(type_t te)
    {
        return te->package;
    }

    inline static const string_t &get_name(type_t te)
    {
        return te->name;
    }

    inline static const class_type_t &get_type(type_t te)
    {
        return te->type;
    }

    inline static const consts_map_t &get_consts_map(type_t te)
    {
        return te->consts;
    }

    inline static const attributes_map_t &get_attributes_map(type_t te)
    {
        return te->attributes;
    }

    inline static const methods_map_t &get_methods_map(type_t te)
    {
        return te->methods;
    }

    inline static const operators_map_t &get_operators_map(type_t te)
    {
        return te->operators;
    }

    inline static void walk_interfaces_map(type_t te, std::function<void(type_t,string_t)> callback)
    {
        auto map = get_interfaces_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_value(*it), get_key(*it));
        }
    }

    inline static void walk_traits_map(type_t te, std::function<void(type_t,string_t)> callback)
    {
        auto map = get_traits_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_value(*it), get_key(*it));
        }
    }

    inline static void walk_consts_map(type_t te, std::function<void(value_t,string_t)> callback)
    {
        auto map = get_consts_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_value(*it), get_key(*it));
        }
    }

    inline static void walk_attributes_map(type_t te, std::function<void(type_t,string_t,access_mode_t)> callback)
    {
        auto map = get_attributes_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_attribute_type(*it), get_key(*it), get_attribute_access_mode(*it));
        }
    }

    inline static void walk_methods_map(type_t te, std::function<void(method_t,string_t)> callback)
    {
        auto map = get_methods_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_value(*it), get_key(*it));
        }
    }

    inline static void walk_operators_map(type_t te, std::function<void(method_t,operator_t)> callback)
    {
        auto map = get_operators_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(get_value(*it), get_key(*it));
        }
    }

    inline static std::ostream &dump(std::ostream &ios, type_t te);
};

class bundle
{
public:
    inline static const string_t &get_name(bundle_t be)
    {
        return be->name;
    }

    inline static const classes_map_t &get_classes_map(bundle_t be)
    {
        return be->classes;
    }

    inline static void walk_classes_map(bundle_t be, std::function<void(type_t,string_t)> callback)
    {
        auto map = get_classes_map(be);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(type::get_value(*it), type::get_key(*it));
        }
    }

    inline static std::ostream &dump(std::ostream &ios, bundle_t be);
};

inline std::ostream &
callable_prototype::dump(std::ostream &ios, prototype_t pe)
{
    auto map = callable_prototype::get_parameters_map(pe);
    ios << " ( ";

    for (auto it = map.begin(); it != map.end();) {
        ios << internal_type::get_name(get_parameter_type(*it)) << " $" << it->first;

        if (get_parameter_value(*it) != nullptr) {
            ios << " = ";
            internal_value::dump(ios, get_parameter_value(*it));
        }

        ++it;
        if (it != map.end()) {
            ios << ", ";
            continue;
        }

        break;
    }

    ios << " ) : " << internal_type::get_name(get_return_type(pe));

    return ios;
}

inline std::ostream &
internal_value::dump(std::ostream &ios, value_t ve)
{
    auto visitor = rephp::engine::visitor::internal_value_stream_visitor();

    ios << internal_type::get_name(internal_value::get_type(ve)) << " { " ;
    ios << boost::apply_visitor(visitor, internal_value::get_raw_value(ve)) << " } ";

    return ios;
}

inline std::ostream &
internal_type::dump(std::ostream &ios, type_t te)
{
    switch (internal_type::get_type(te)) {
    case type::TYP_NATIVE_TYPE:
        ios << "    - Type [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case type::TYP_CLASS:
        ios << "    - Class [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case type::TYP_INTERFACE:
        ios << "    - Interface [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case type::TYP_TRAIT:
        ios << "    - Trait [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case type::TYP_ENUM:
        ios << "    - Enum [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    }

    auto parent_ce = internal_type::get_parent(te);
    if (parent_ce != nullptr) {
        ios << "      - Parent [ " << internal_type::get_name(parent_ce) << " ]" << std::endl;
    }

    if (internal_type::get_interfaces_map(te).size()) {
        ios << "      - Interfaces:" << std::endl;
        internal_type::walk_interfaces_map(te, [&](type_t interface_te, string_t) {
            ios << "        - [ " << internal_type::get_name(interface_te) << " ]" << std::endl;
        });
    }

    if (internal_type::get_traits_map(te).size()) {
        ios << "      - Traits:" << std::endl;
        internal_type::walk_traits_map(te, [&](type_t trait_te, string_t) {
            ios << "        - [ " << internal_type::get_name(trait_te) << " ]" << std::endl;
        });
    }

    if (internal_type::get_attributes_map(te).size()) {
        ios << "      - Attributes:" << std::endl;
        internal_type::walk_attributes_map(te, [&](type_t type, string_t name, access_mode_t access_mode) {
            ios << "        - " << (access_mode == rephp::engine::ACC_PUBLIC ? "public" : (access_mode == rephp::engine::ACC_PROTECTED ? "protected" : "private"));
            ios << " " << name << " [ " << internal_type::get_name(type) << " ]" << std::endl;
        });
    }

    if (internal_type::get_methods_map(te).size()) {
        ios << "      - Methods:" << std::endl;
        internal_type::walk_methods_map(te, [&](method_t method, string_t name) {
            ios << "        - " << (method_entry::get_access(method) == rephp::engine::ACC_PUBLIC ? "public" : (method_entry::get_access(method) == rephp::engine::ACC_PROTECTED ? "protected" : "private"));
            ios << " " << name;
            callable_prototype::dump(ios, method_entry::get_prototype(method)) << std::endl;
        });
    }

    if (internal_type::get_operators_map(te).size()) {
        ios << "      - Operators:" << std::endl;
        internal_type::walk_operators_map(te, [&](method_t method, operator_t op) {
            ios << "        - " << (method_entry::get_access(method) == rephp::engine::ACC_PUBLIC ? "public" : (method_entry::get_access(method) == rephp::engine::ACC_PROTECTED ? "protected" : "private"));
            ios << " " << rephp::engine::engine::get_operator_string(op);
            callable_prototype::dump(ios, method_entry::get_prototype(method)) << std::endl;
        });
    }

    return ios;
}

inline std::ostream &
bundle::dump(std::ostream &ios, bundle_t be)
{
    using namespace rephp::reflection;

    ios << "- Bundle [ " << bundle::get_name(be) << " ]" << std::endl;
    ios << "  - Declared Classes: " << std::endl;

    bundle::walk_classes_map(be, [&](type_t ce, string_t) {
        internal_type::dump(ios, ce);
    });

    return ios;
}

};

};

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::prototype_t const pe)
{
    return rephp::reflection::callable_prototype::dump(ios, pe);
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::method_t const)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::type_t const)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::value_t const)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::bundle_t const re)
{
    return rephp::reflection::bundle::dump(ios, re);
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::vector_t const vec)
{
    return ios << "vector(" << vec.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::list_t const list)
{
    return ios << "list(" << list.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::map_t const array)
{
    return ios << "map(" << array.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::multimap_t const array)
{
    return ios << "multimap(" << array.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::registry_t const registry)
{
    return ios << "registry(" << registry.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::repository_t const repository)
{
    return ios << "repository(" << repository.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::stream_t const)
{
    return ios << "stream";
}

inline std::ostream&
operator<< (std::ostream &ios, rephp::engine::type::resource_t const)
{
    return ios << "resource";
}

#endif