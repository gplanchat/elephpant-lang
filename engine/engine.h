
#ifndef REPHP_ENGINE_H
#define REPHP_ENGINE_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <initializer_list>
#include <algorithm>

#include <boost/variant.hpp>

#include "types.h"
#include "visitors.h"

#define ADD_OPERATOR(op) {op, #op}

namespace rephp {

namespace reflection {

class worker;
class callable_prototype;
class method_entry;
class internal_type;
class internal_value;
class bundle;

};

namespace engine {

using namespace std;

class engine
{
private:
    static std::map<type::operator_t,string>       operators_map;
    static std::map<type::string_t,type::bundle_t> bundles_map;

public:
    inline static const type::string_t &get_operator_string(type::operator_t op)
    {
        return operators_map[op];
    }

    inline void static throw_exception(type::type_t, const char *message, long = 0, type::value_t = nullptr)
    {
        std::cerr << "An exception occured: " << message << std::endl;
    }

    inline void static throw_exception(type::type_t, type::string_t &message, long = 0, type::value_t = nullptr)
    {
        std::cerr << "An exception occured: " << message << std::endl;
    }

    inline void static register_bundle(type::bundle_t bundle);

    template<typename T>
    type::value_t static invoke(type::string class_name, T value)
    {
        auto found = find_if(bundles_map.begin(), bundles_map.end(), [&](auto current) {
            if (type::get_value(current)->has_class(class_name)) {
                return true;
            }
            return false;
        });

        if (found != bundles_map.end()) {
            return type::get_value(*found)->invoke(class_name, value);
        }

        return nullptr;
    }
};

class worker
{
public:
    virtual type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self) = 0;
};

class callable_prototype: public enable_shared_from_this<callable_prototype>
{
    friend rephp::reflection::callable_prototype;

private:
    type::type_t return_type;
    type::parameters_map_t parameters;

public:
    callable_prototype();
    callable_prototype(type::type_t return_type);

    bool match(type::call_parameters_list_t params);
    bool match(type::prototype_t prototype);

    void add_parameter(type::string_t name, type::type_t parameter);
    void add_parameter(type::string_t name, type::type_t parameter, type::value_t default_value);

    type::size_t count();
};

class method_entry: public enable_shared_from_this<method_entry>
{
    friend rephp::reflection::method_entry;

private:
    type::prototype_t   prototype;
    type::worker_t      worker;
    type::access_mode_t access;

public:
    method_entry(type::prototype_t prototype, type::worker_t cb, type::access_mode_t access);

    bool matches_params(type::call_parameters_list_t params);
    bool matches_prototype(type::prototype_t prototype);
    bool matches_access_mode(type::access_mode_t mode);

    type::value_t call(type::call_parameters_list_t parameters, type::value_t context);
};

class internal_type: public enable_shared_from_this<internal_type>
{
    friend rephp::reflection::internal_type;

private:
    type::type_t            parent;
    type::classes_map_t     interfaces;
    type::classes_map_t     traits;
    type::string_t          package;
    type::string_t          name;
    type::class_type_t      type;
    type::consts_map_t      consts;
    type::attributes_map_t  attributes;
    type::methods_map_t     methods;
    type::operators_map_t   operators;

public:
    internal_type(type::string_t name, type::class_type_t type, type::type_t parent = nullptr);

    void add_interface(type::type_t interface);
    void add_trait(type::type_t trait);
    void add_attribute(type::string_t name, type::type_t type, type::access_mode_t access = ACC_PRIVATE);
    void add_method(type::string_t name, type::prototype_t prototype, type::worker_t cb, type::access_mode_t access = ACC_PRIVATE);
    void add_operator(type::operator_t op, type::prototype_t prototype, type::worker_t cb, type::access_mode_t access = ACC_PUBLIC);

    bool has_parent(type::string_t parent);
    bool has_parent(type::type_t parent);
    bool has_interface(type::string_t interface);
    bool has_interface(type::type_t interface);
    bool has_interface_recursive(type::string_t interface);
    bool has_interface_recursive(type::type_t interface);
    bool has_trait(type::string_t trait);
    bool has_trait(type::type_t trait);
    bool has_trait_recursive(type::string_t trait);
    bool has_trait_recursive(type::type_t trait);
    bool has_method(type::string_t name, type::call_parameters_list_t params, type::value_t context);
    bool has_operator(type::operator_t op, type::call_parameters_list_t params, type::value_t context);
    bool has_attribute(type::string_t name, type::value_t context);

    type::method_t find_method(type::string_t name, type::call_parameters_list_t params, type::value_t context);
    type::method_t find_operator(type::operator_t op, type::call_parameters_list_t params, type::value_t context);

    type::string_t get_name();
    type::type_t get_parent();
};

class internal_value: public enable_shared_from_this<internal_value>
{
    friend rephp::reflection::internal_value;

private:
    type::type_t type;
    type::value_attributes_map_t    attributes;
    type::internal_variant_t        raw_value;

public:
    internal_value(type::type_t type);

    type::type_t get_type();

    template<typename T>
    inline T get() const;

    template<typename T>
    inline void set(T value)
    {
        this->raw_value = value;
    }

    template<typename T>
    inline T to()
    {
        return boost::get<T>(this->raw_value);
    }

    inline type::value_t call(type::operator_t op, std::initializer_list<type::value_t> parameters)
    {
        type::value_t retval;

        auto this_ptr = shared_from_this();
        auto method = this->get_type()->find_operator(op, parameters, this_ptr);

        if (method == nullptr) {
            engine::throw_exception(nullptr, "Operator not found.");
            return engine::invoke("Null", 0);
        }

        return method->call(parameters, this_ptr);
    }

    inline type::value_t copy()
    {
        type::value_t copied_value(new internal_value(this->get_type()));

        copied_value->raw_value = this->raw_value;

        return copied_value;
    }
/*
    inline type::value_t call(type::string_t method_name, std::initializer_list<type::value_t> parameters)
    {
        type::value_t retval;
        type::prototype_t proto(new callable_prototype())

        for (auto it = parameters.begin(); it != parameters.end(); ++it) {
            proto->add_parameter(it->first, it->second.get_type());
        }

        auto method = this->get_type()->find_method(method_name, proto, shared_from_this());
        if (method == nullptr) {
            throw wrong_call("method does not exist.");
        }

        //method->call();
    }
*/
};

template<>
inline bool
internal_value::get<bool>() const
{
    return boost::get<bool>(this->raw_value);
}

template<>
inline char
internal_value::get<char>() const
{
    return boost::get<char>(this->raw_value);
}

template<>
inline long
internal_value::get<long>() const
{
    return boost::get<long>(this->raw_value);
}

template<>
inline double
internal_value::get<double>() const
{
    return boost::get<double>(this->raw_value);
}

template<>
inline type::string_t
internal_value::get<type::string_t>() const
{
    return boost::get<type::string_t>(this->raw_value);
}

template<>
inline type::vector_t
internal_value::get<type::vector_t>() const
{
    return boost::get<type::vector_t>(this->raw_value);
}

template<>
inline type::map_t
internal_value::get<type::map_t>() const
{
    return boost::get<type::map_t>(this->raw_value);
}

template<>
inline type::multimap_t
internal_value::get<type::multimap_t>() const
{
    return boost::get<type::multimap_t>(this->raw_value);
}

template<>
inline shared_ptr<void>
internal_value::get<shared_ptr<void>>() const
{
    return boost::get<shared_ptr<void>>(this->raw_value);
}

template<>
inline void
internal_value::set<bool>(bool value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<char>(char value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<long>(long value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<int>(int value)
{
    this->raw_value = static_cast<long>(value);
}

template<>
inline void
internal_value::set<short>(short value)
{
    this->raw_value = static_cast<long>(value);
}

template<>
inline void
internal_value::set<double>(double value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<float>(float value)
{
    this->raw_value = static_cast<double>(value);
}

template<>
inline void
internal_value::set<type::string_t&>(type::string_t &value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<type::vector_t&>(type::vector_t &value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<map<type::string_t,type::value_t>&>(map<type::string_t,type::value_t> &value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<shared_ptr<void>>(shared_ptr<void> value)
{
    this->raw_value = value;
}

class bundle: public enable_shared_from_this<bundle>
{
    friend rephp::reflection::bundle;

private:
    type::string_t      name;
    type::classes_map_t classes;

protected:
    void register_class(type::type_t ce);

public:
    bundle(const char *const name);
    bundle(type::string_t name);

    void merge(type::string_t ns, type::bundle_t sub_bundle);

    type::string_t get_name();

    inline bool has_class(const char *name)
    {
        type::string_t n(name);
        return this->has_class(n);
    }

    inline type::type_t get_class(const char *name)
    {
        type::string_t n(name);
        return this->get_class(n);
    }

    bool has_class(type::string_t &name);
    type::type_t get_class(type::string_t &name);

    template<typename T>
    inline type::value_t invoke(const char *name, T value)
    {
        type::string_t str(name);
        return this->invoke<T>(str, value);
    }

    template<typename T>
    inline type::value_t invoke(type::string_t &name, T value)
    {
        auto type = this->get_class(name);

        if (type == nullptr) {
            return nullptr;
        }

        type::value_t item(new internal_value(type));
        item->set<T>(value);

        return item;
    }
};

inline void
engine::register_bundle(type::bundle_t bundle)
{
    bundles_map.insert(std::make_pair(bundle->get_name(), bundle));
}

inline type::value_t
operator+ (type::value_t left, type::value_t right)
{
    return left->call(type::OP_ADD, {right});
}

inline type::value_t
operator- (type::value_t left, type::value_t right)
{
    return left->call(type::OP_SUB, {right});
}

inline type::value_t
operator* (type::value_t left, type::value_t right)
{
    return left->call(type::OP_MUL, {right});
}

inline type::value_t
operator/ (type::value_t left, type::value_t right)
{
    return left->call(type::OP_DIV, {right});
}

inline type::value_t
operator% (type::value_t left, type::value_t right)
{
    return left->call(type::OP_MOD, {right});
}

};

};

#endif
