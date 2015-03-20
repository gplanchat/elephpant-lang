
#ifndef REPHP_REFLECTION_H
#define REPHP_REFLECTION_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "engine.h"

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

using rephp::engine::access_mode;

class callable_prototype
{
};

class method_entry
{
};

class internal_type
{
public:
    typedef unordered_map<string, shared_ptr<rephp::engine::internal_type>>                                   classes_map_t;
    typedef unordered_map<string, shared_ptr<rephp::engine::internal_value>>                                  const_map_t;
    typedef unordered_map<string, pair<shared_ptr<rephp::engine::internal_type>, rephp::engine::access_mode>> attributes_map_t;
    typedef unordered_multimap<string, shared_ptr<rephp::engine::method_entry>>                               methods_map_t;
    typedef unordered_multimap<rephp::engine::engine_operator, shared_ptr<rephp::engine::method_entry>>       operators_map_t;

    inline static const shared_ptr<rephp::engine::internal_type> get_parent(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->parent;
    }

    inline static const classes_map_t &get_interfaces_map(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->interfaces;
    }

    inline static const classes_map_t &get_traits_map(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->traits;
    }

    inline static const string &get_package(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->package;
    }

    inline static const string &get_name(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->name;
    }

    inline static const rephp::engine::class_type &get_type(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->type;
    }

    inline static const const_map_t &get_consts_map(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->consts;
    }

    inline static const attributes_map_t &get_attributes_map(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->attributes;
    }

    inline static const methods_map_t &get_methods_map(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->methods;
    }

    inline static const operators_map_t &get_operators_map(shared_ptr<rephp::engine::internal_type> te)
    {
        return te->operators;
    }

    inline static void walk_interfaces_map(shared_ptr<rephp::engine::internal_type> te, std::function<void(classes_map_t::mapped_type,classes_map_t::key_type)> callback)
    {
        auto map = get_interfaces_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_traits_map(shared_ptr<rephp::engine::internal_type> te, std::function<void(classes_map_t::mapped_type,classes_map_t::key_type)> callback)
    {
        auto map = get_traits_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_consts(shared_ptr<rephp::engine::internal_type> te, std::function<void(const_map_t::mapped_type,const_map_t::key_type)> callback)
    {
        auto map = get_consts_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_attributes(shared_ptr<rephp::engine::internal_type> te, std::function<void(attributes_map_t::mapped_type,attributes_map_t::key_type)> callback)
    {
        auto map = get_attributes_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_methods(shared_ptr<rephp::engine::internal_type> te, std::function<void(methods_map_t::mapped_type,methods_map_t::key_type)> callback)
    {
        auto map = get_methods_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_operators(shared_ptr<rephp::engine::internal_type> te, std::function<void(operators_map_t::mapped_type,operators_map_t::key_type)> callback)
    {
        auto map = get_operators_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }
};


class internal_value
{
public:
    typedef unordered_map<string, shared_ptr<internal_value>> attributes_map_t;

    inline static const shared_ptr<rephp::engine::internal_type> get_type(shared_ptr<rephp::engine::internal_value> &ve)
    {
        return ve->type;
    }

    inline static const unordered_map<string, shared_ptr<rephp::engine::internal_value>> &get_attributes_map(shared_ptr<rephp::engine::internal_value> &ve)
    {
        return ve->attributes;
    }
};

class bundle
{
public:
    typedef unordered_map<string, shared_ptr<rephp::engine::internal_type>> classes_map_t;

    inline static const string &get_name(shared_ptr<rephp::engine::bundle> be)
    {
        return be->name;
    }

    inline static const classes_map_t &get_classes_map(shared_ptr<rephp::engine::bundle> be)
    {
        return be->classes;
    }

    inline static void walk_classes_map(shared_ptr<rephp::engine::bundle> be, std::function<void(classes_map_t::mapped_type,classes_map_t::key_type)> callback)
    {
        auto map = get_classes_map(be);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }
};

};

};

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::callable_prototype> const pe)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::method_entry> const me)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::internal_type> const te)
{
    using namespace rephp::reflection;

    ios << "    - Class [ " << internal_type::get_name(te) << " ]" << std::endl;
    auto parent_ce = internal_type::get_parent(te);
    if (parent_ce != nullptr) {
        ios << "      - Parent [ " << internal_type::get_name(parent_ce) << " ]" << std::endl;
    }
/*
    if (internal_type::get_interfaces_map(te).size()) {
        ios << "      - Interfaces:" << std::endl;
        internal_type::walk_interfaces_map(te, [&](internal_type::classes_map_t::mapped_type interface_te, internal_type::classes_map_t::key_type ) {
            ios << "        - [ " << internal_type::get_name(interface_te) << " ]" << std::endl;
        });
    }

    if (internal_type::get_traits_map(te).size()) {
        ios << "      - Traits:" << std::endl;
        internal_type::walk_traits_map(te, [&](internal_type::classes_map_t::mapped_type trait_te, internal_type::classes_map_t::key_type name) {
            ios << "        - [ " << internal_type::get_name(trait_te) << " ]" << std::endl;
        });
    }

    if (internal_type::get_attributes_map(te).size()) {
        ios << "      - Attributes:" << std::endl;
        internal_type::walk_attributes_map(te, [&](internal_type::attributes_map_t::mapped_type attribute, internal_type::attributes_map_t::key_type name) {
            ios << "        - " << (attribute.second == ACC_PUBLIC ? "public" : (attribute.second == ACC_PROTECTED ? "protected" : "private"))
            ios << " " << name << " [ " << internal_type::get_name(attribute.first) << " ]" << std::endl;
        });
    }

    if (internal_type::get_methods_map(te).size()) {
        ios << "      - Methods:" << std::endl;
        internal_type::walk_methods_map(te, [&](internal_type::methods_map_t::mapped_type method, internal_type::methods_map_t::key_type name) {
            ios << "        - " << (attribute.second == ACC_PUBLIC ? "public" : (attribute.second == ACC_PROTECTED ? "protected" : "private"))
            ios << " " << name << std::endl;
        });
    }

    if (internal_type::get_operators_map(te).size()) {
        ios << "      - Methods:" << std::endl;
        internal_type::walk_operators_map(te, [&](internal_type::operators_map_t::mapped_type method, internal_type::operators_map_t::key_type op) {
            ios << "        - " << op << " " << name << std::endl;
        });
    }
*/
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::internal_value> const re)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::bundle> const re)
{
    using namespace rephp::reflection;

    ios << "- Bundle [ " << bundle::get_name(re) << " ]" << std::endl;
    ios << "  - Declared Classes: " << std::endl;

    bundle::walk_classes_map(re, [&](bundle::classes_map_t::mapped_type ce, bundle::classes_map_t::key_type name) {
        ios << ce << std::endl;
    });

    return ios;
}

#endif