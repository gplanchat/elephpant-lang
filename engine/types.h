
#ifndef REPHP_ENGINE_TYPES_H
#define REPHP_ENGINE_TYPES_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <boost/variant.hpp>

namespace rephp {

namespace engine {

class worker;
class callable_prototype;
class method_entry;
class internal_type;
class internal_value;
class bundle;

typedef enum access_mode {
    ACC_PUBLIC,
    ACC_PROTECTED,
    ACC_PRIVATE,
    ACC_PACKAGE
} access_mode_t;

typedef enum class_type {
    TYP_NATIVE_TYPE,
    TYP_CLASS,
    TYP_INTERFACE,
    TYP_TRAIT,
    TYP_ENUM
} class_type_t;

typedef enum engine_operator {
    OP_LET,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_INTDIV,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_BITWISE_AND,
    OP_BITWISE_OR,
    OP_BITWISE_XOR,
    OP_BITWISE_NOT,
    OP_LHS,
    OP_RHS,
    OP_OFFSET_SET,
    OP_OFFSET_GET,
    OP_OFFSET_UNSET,
    OP_OFFSET_EXISTS,
    OP_DOT,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_LTEQ,
    OP_GT,
    OP_GTEQ,
    OP_SPACESHIP
} operator_t;

namespace type {

using namespace rephp::engine;

typedef std::shared_ptr<worker>              worker_t;
typedef std::shared_ptr<callable_prototype>  prototype_t;
typedef std::shared_ptr<method_entry>        method_t;
typedef std::shared_ptr<internal_type>       type_t;
typedef std::shared_ptr<internal_value>      value_t;
typedef std::shared_ptr<bundle>              bundle_t;

typedef bool                            boolean_t;
typedef char                            character_t;
typedef signed long                     integer_t;
typedef double                          float_t;
typedef std::string                     string_t;
typedef std::vector<value_t>            vector_t;
typedef std::list<value_t>              list_t;
typedef std::map<string_t,value_t>      map_t;
typedef std::multimap<string_t,value_t> multimap_t;
typedef std::map<value_t,value_t>       registry_t;
typedef std::multimap<value_t,value_t>  repository_t;
typedef std::shared_ptr<std::iostream>  stream_t;
typedef std::shared_ptr<void>           resource_t;

typedef boost::variant<
    boolean_t,
    character_t,
    integer_t,
    float_t,
    string_t,
    vector_t,
    list_t,
    map_t,
    multimap_t,
    registry_t,
    repository_t,
    stream_t,
    worker_t,
    resource_t
> internal_variant_t;

typedef std::map<string_t, std::tuple<type_t, value_t>>         parameters_map_t;
typedef std::map<string_t, type_t>                              classes_map_t;
typedef std::map<string_t, value_t>                             consts_map_t;
typedef std::map<string_t, std::tuple<type_t, access_mode_t>>   attributes_map_t;
typedef std::map<string_t, value_t>                             value_attributes_map_t;
typedef std::multimap<string_t, method_t>                       methods_map_t;
typedef std::multimap<operator_t, method_t>                     operators_map_t;

template<typename K, typename V>
inline K &get_key(std::pair<K,V> &current)
{
    return current.first;
}

template<typename K, typename V>
inline V get_value(std::pair<K,V> &current)
{
    return current.second;
}

inline type_t get_parameter_type(parameters_map_t::value_type current)
{
    return std::get<0>(current.second);
}

inline value_t get_parameter_value(parameters_map_t::value_type current)
{
    return std::get<1>(current.second);
}

inline type_t get_attribute_type(attributes_map_t::value_type current)
{
    return std::get<0>(current.second);
}

inline access_mode get_attribute_access_mode(type::attributes_map_t::value_type current)
{
    return std::get<1>(current.second);
}

};

};

};

#endif
