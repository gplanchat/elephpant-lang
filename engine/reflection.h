
#ifndef REPHP_REFLECTION_H
#define REPHP_REFLECTION_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

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

using rephp::engine::access_mode;

typedef map<string, pair<shared_ptr<rephp::engine::internal_type>, shared_ptr<rephp::engine::internal_value>>> parameters_map_t;
typedef boost::variant<bool,char,long,double,string,vector<shared_ptr<rephp::engine::internal_value>>,map<string,shared_ptr<rephp::engine::internal_value>>,shared_ptr<void>> internal_variant_t;

class callable_prototype
{
public:
    inline static shared_ptr<rephp::engine::internal_type> get_return_type(shared_ptr<rephp::engine::callable_prototype> pe)
    {
        return pe->return_type;
    }

    inline static parameters_map_t get_parameters_map(shared_ptr<rephp::engine::callable_prototype> pe)
    {
        return pe->parameters;
    }

    inline static void walk_parameters_map(shared_ptr<rephp::engine::callable_prototype> pe, std::function<void(parameters_map_t::mapped_type::first_type,parameters_map_t::key_type,parameters_map_t::mapped_type::second_type)> callback)
    {
        auto map = get_parameters_map(pe);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second.first, it->first, it->second.second);
        }
    }

    inline static std::ostream &dump(std::ostream &ios, shared_ptr<rephp::engine::callable_prototype> pe);
};

class method_entry
{
public:
    inline static const shared_ptr<rephp::engine::callable_prototype> get_prototype(shared_ptr<rephp::engine::method_entry> me)
    {
        return me->prototype;
    }

    inline static const shared_ptr<rephp::engine::worker> get_worker_callable(shared_ptr<rephp::engine::method_entry> me)
    {
        return me->worker_callable;
    }

    inline static rephp::engine::access_mode get_access(shared_ptr<rephp::engine::method_entry> me)
    {
        return me->access;
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

    inline static const internal_variant_t &get_raw_value(shared_ptr<rephp::engine::internal_value> &ve)
    {
        return ve->raw_value;
    }

    inline static std::ostream &dump(std::ostream &ios, shared_ptr<rephp::engine::internal_value> ve);
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

    inline static void walk_consts_map(shared_ptr<rephp::engine::internal_type> te, std::function<void(const_map_t::mapped_type,const_map_t::key_type)> callback)
    {
        auto map = get_consts_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_attributes_map(shared_ptr<rephp::engine::internal_type> te, std::function<void(attributes_map_t::mapped_type,attributes_map_t::key_type)> callback)
    {
        auto map = get_attributes_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_methods_map(shared_ptr<rephp::engine::internal_type> te, std::function<void(methods_map_t::mapped_type,methods_map_t::key_type)> callback)
    {
        auto map = get_methods_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static void walk_operators_map(shared_ptr<rephp::engine::internal_type> te, std::function<void(operators_map_t::mapped_type,operators_map_t::key_type)> callback)
    {
        auto map = get_operators_map(te);
        for (auto it = map.begin(); it != map.end(); ++it) {
            callback(it->second, it->first);
        }
    }

    inline static std::ostream &dump(std::ostream &ios, shared_ptr<rephp::engine::internal_type> te);
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

    inline static std::ostream &dump(std::ostream &ios, shared_ptr<rephp::engine::bundle> be);
};

inline std::ostream &
callable_prototype::dump(std::ostream &ios, shared_ptr<rephp::engine::callable_prototype> pe)
{
    auto map = get_parameters_map(pe);
    ios << " ( ";

    for (auto it = map.begin(); it != map.end();) {
        ios << internal_type::get_name(it->second.first) << " $" << it->first;

        if (it->second.second != nullptr) {
            ios << " = ";
            internal_value::dump(ios, it->second.second);
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
internal_value::dump(std::ostream &ios, shared_ptr<rephp::engine::internal_value> ve)
{
    auto visitor = rephp::engine::visitor::internal_value_stream_visitor();

    ios << internal_type::get_name(internal_value::get_type(ve)) << " { " ;
    ios << boost::apply_visitor(visitor, internal_value::get_raw_value(ve)) << " } ";

    return ios;
}

inline std::ostream &
internal_type::dump(std::ostream &ios, shared_ptr<rephp::engine::internal_type> te)
{
    switch (internal_type::get_type(te)) {
    case engine::TYP_NATIVE_TYPE:
        ios << "    - Type [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case engine::TYP_CLASS:
        ios << "    - Class [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case engine::TYP_INTERFACE:
        ios << "    - Interface [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case engine::TYP_TRAIT:
        ios << "    - Trait [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    case engine::TYP_ENUM:
        ios << "    - Enum [ " << internal_type::get_name(te) << " ]" << std::endl;
        break;
    }

    auto parent_ce = internal_type::get_parent(te);
    if (parent_ce != nullptr) {
        ios << "      - Parent [ " << internal_type::get_name(parent_ce) << " ]" << std::endl;
    }

    if (internal_type::get_interfaces_map(te).size()) {
        ios << "      - Interfaces:" << std::endl;
        internal_type::walk_interfaces_map(te, [&](internal_type::classes_map_t::mapped_type interface_te, internal_type::classes_map_t::key_type ) {
            ios << "        - [ " << internal_type::get_name(interface_te) << " ]" << std::endl;
        });
    }

    if (internal_type::get_traits_map(te).size()) {
        ios << "      - Traits:" << std::endl;
        internal_type::walk_traits_map(te, [&](internal_type::classes_map_t::mapped_type trait_te, internal_type::classes_map_t::key_type) {
            ios << "        - [ " << internal_type::get_name(trait_te) << " ]" << std::endl;
        });
    }

    if (internal_type::get_attributes_map(te).size()) {
        ios << "      - Attributes:" << std::endl;
        internal_type::walk_attributes_map(te, [&](internal_type::attributes_map_t::mapped_type attribute, internal_type::attributes_map_t::key_type name) {
            ios << "        - " << (attribute.second == rephp::engine::ACC_PUBLIC ? "public" : (attribute.second == rephp::engine::ACC_PROTECTED ? "protected" : "private"));
            ios << " " << name << " [ " << internal_type::get_name(attribute.first) << " ]" << std::endl;
        });
    }

    if (internal_type::get_methods_map(te).size()) {
        ios << "      - Methods:" << std::endl;
        internal_type::walk_methods_map(te, [&](internal_type::methods_map_t::mapped_type method, internal_type::methods_map_t::key_type name) {
            ios << "        - " << (method_entry::get_access(method) == rephp::engine::ACC_PUBLIC ? "public" : (method_entry::get_access(method) == rephp::engine::ACC_PROTECTED ? "protected" : "private"));
            ios << " " << name;
            callable_prototype::dump(ios, method_entry::get_prototype(method)) << std::endl;
        });
    }

    if (internal_type::get_operators_map(te).size()) {
        ios << "      - Operators:" << std::endl;
        internal_type::walk_operators_map(te, [&](internal_type::operators_map_t::mapped_type method, internal_type::operators_map_t::key_type op) {
            ios << "        - " << (method_entry::get_access(method) == rephp::engine::ACC_PUBLIC ? "public" : (method_entry::get_access(method) == rephp::engine::ACC_PROTECTED ? "protected" : "private"));
            ios << " " << engine::engine::get_operator_string(op);
            callable_prototype::dump(ios, method_entry::get_prototype(method)) << std::endl;
        });
    }

    return ios;
}

inline std::ostream &
bundle::dump(std::ostream &ios, shared_ptr<rephp::engine::bundle> be)
{
    using namespace rephp::reflection;

    ios << "- Bundle [ " << bundle::get_name(be) << " ]" << std::endl;
    ios << "  - Declared Classes: " << std::endl;

    bundle::walk_classes_map(be, [&](bundle::classes_map_t::mapped_type ce, bundle::classes_map_t::key_type) {
        internal_type::dump(ios, ce);
    });

    return ios;
}

};

};

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::callable_prototype> const pe)
{
    using namespace rephp::reflection;

    return callable_prototype::dump(ios, pe);
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::method_entry> const)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::internal_type> const)
{
    using namespace rephp::reflection;
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::internal_value> const)
{
    return ios;
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<rephp::engine::bundle> const re)
{
    using namespace rephp::reflection;

    return bundle::dump(ios, re);
}

inline std::ostream&
operator<< (std::ostream &ios, std::vector<std::shared_ptr<rephp::engine::internal_value>> const vec)
{
    return ios << "vector(" << vec.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, std::map<std::string,std::shared_ptr<rephp::engine::internal_value>> const arr)
{
    return ios << "map(" << arr.size() << ")";
}

inline std::ostream&
operator<< (std::ostream &ios, std::shared_ptr<void> const)
{
    return ios << "resource";
}

#endif