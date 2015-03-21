
#ifndef REPHP_ENGINE_VISITORS_H
#define REPHP_ENGINE_VISITORS_H

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>

#include <boost/variant.hpp>

#include "engine.h"
#include "types.h"

namespace rephp {

namespace engine {

namespace visitor {

using namespace std;
using namespace rephp::engine;

class internal_value_stream_visitor: public boost::static_visitor<type::string_t>
{
public:
    inline type::string_t operator() (type::boolean_t value) const
    {
        return type::string_t(value ? "true" : "false");
    }

    inline type::string_t operator() (type::character_t value) const
    {
        return to_string(value);
    }

    inline type::string_t operator() (type::integer_t value) const
    {
        return to_string(value);
    }

    inline type::string_t operator() (type::float_t value) const
    {
        return to_string(value);
    }

    inline type::string_t operator() (type::string_t value) const
    {
        return "\"" + value + "\"";
    }

    inline type::string_t operator() (type::vector_t value) const
    {
        return string("vector(") + to_string(value.size()) + ")";
    }

    inline type::string_t operator() (type::list_t value) const
    {
        return string("list(") + to_string(value.size()) + ")";
    }

    inline type::string_t operator() (type::map_t value) const
    {
        return type::string_t("map(") + to_string(value.size()) + ")";
    }

    inline type::string_t operator() (type::multimap_t value) const
    {
        return type::string_t("multimap(") + to_string(value.size()) + ")";
    }

    inline type::string_t operator() (type::registry_t value) const
    {
        return type::string_t("registry(") + to_string(value.size()) + ")";
    }

    inline type::string_t operator() (type::repository_t value) const
    {
        return type::string_t("repository(") + to_string(value.size()) + ")";
    }

    inline type::string_t operator() (type::stream_t) const
    {
        return type::string_t("stream");
    }

    inline type::string_t operator() (type::worker_t) const
    {
        return type::string_t("worker");
    }

    inline type::string_t operator() (type::resource_t) const
    {
        return type::string_t("resource");
    }
};

};

};

};

/*



template<typename T>
class internal_value_scalar_get_visitor: public boost::static_visitor<T>
{
public:
    template<typename U>
    T operator() (U value) const;
};

class internal_value_string_get_visitor: public boost::static_visitor<type::string_t>
{
public:
    template<typename T>
    type::string_t operator() (T value) const;
};

class internal_value_vector_get_visitor: public boost::static_visitor<type::vector_t>
{
public:
    template<typename T>
    type::vector_t operator() (T value) const;
};

class internal_value_map_get_visitor: public boost::static_visitor<map<type::string_t,type::value_t>>
{
public:
    template<typename T>
    map<type::string_t,type::value_t> operator() (T value) const;
};

class internal_value_resource_get_visitor: public boost::static_visitor<shared_ptr<void>>
{
public:
    template<typename T>
    shared_ptr<void> operator() (T value) const;
};

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<bool> (bool value) const
{
    return value;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<char> (char value) const
{
    return value != 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<long> (long value) const
{
    return value != 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<double> (double value) const
{
    return value != 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to boolean is not allowed.");
    return 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to boolean is not allowed.");
    return 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to boolean is not allowed.");
    return 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to boolean is not allowed.");
    return 0;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<bool> (bool value) const
{
    return value;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<char> (char value) const
{
    return value;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<long> (long value) const
{
    return value;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<double> (double value) const
{
    return value;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to character is not allowed.");
    return 0;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to character is not allowed.");
    return 0;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to character is not allowed.");
    return 0;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to character is not allowed.");
    return 0;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<bool> (bool value) const
{
    return value;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<char> (char value) const
{
    return value;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<long> (long value) const
{
    return value;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<double> (double value) const
{
    return value;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to integer is not allowed.");
    return 0;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to integer is not allowed.");
    return 0;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to integer is not allowed.");
    return 0;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to integer is not allowed.");
    return 0;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<bool> (bool value) const
{
    return value;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<char> (char value) const
{
    return value;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<long> (long value) const
{
    return value;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<double> (double value) const
{
    return value;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to float is not allowed.");
    return 0;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to float is not allowed.");
    return 0;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to float is not allowed.");
    return 0;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to float is not allowed.");
    return 0;
}

template<>
type::string_t internal_value_string_get_visitor::operator()<bool> (bool value) const
{
    return value ? "true" : "false";
}

template<>
type::string_t internal_value_string_get_visitor::operator()<char> (char value) const
{
    return to_string(value);
}

template<>
type::string_t internal_value_string_get_visitor::operator()<long> (long value) const
{
    return to_string(value);
}

template<>
type::string_t internal_value_string_get_visitor::operator()<double> (double value) const
{
    return to_string(value);
}

template<>
type::string_t internal_value_string_get_visitor::operator()<type::string_t> (type::string_t str) const
{
    return str;
}

template<>
type::string_t internal_value_string_get_visitor::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to string is not allowed.");
    type::string_t str = "vector";
    return str;
}

template<>
type::string_t internal_value_string_get_visitor::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to string is not allowed.");
    return type::string_t("map");
}

template<>
type::string_t internal_value_string_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to string is not allowed.");
    return type::string_t("resource");
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<bool> (bool) const
{
    engine::engine::throw_exception(nullptr, "cast from boolean to vector is not allowed.");
    return type::vector_t();
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<char> (char) const
{
    engine::engine::throw_exception(nullptr, "cast from character to vector is not allowed.");
    return type::vector_t();
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<long> (long) const
{
    engine::engine::throw_exception(nullptr, "cast from integer to vector is not allowed.");
    return type::vector_t();
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<double> (double) const
{
    engine::engine::throw_exception(nullptr, "cast from float to vector is not allowed.");
    return type::vector_t();
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to vector is not allowed.");
    return type::vector_t();
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<type::vector_t> (type::vector_t vec) const
{
    return vec;
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to vector is not allowed.");
    return type::vector_t();
}

template<>
type::vector_t
internal_value_vector_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to vector is not allowed.");
    return type::vector_t();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<bool> (bool value) const
{
    engine::engine::throw_exception(nullptr, "cast from boolean to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<char> (char value) const
{
    engine::engine::throw_exception(nullptr, "cast from character to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<long> (long value) const
{
    engine::engine::throw_exception(nullptr, "cast from integer to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<double> (double value) const
{
    engine::engine::throw_exception(nullptr, "cast from float to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t> array) const
{
    return array;
}

template<>
map<type::string_t,type::value_t>
internal_value_map_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to map is not allowed.");
    return map<type::string_t,type::value_t>();
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<bool> (bool value) const
{
    engine::engine::throw_exception(nullptr, "cast from boolean to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<char> (char value) const
{
    engine::engine::throw_exception(nullptr, "cast from character to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<long> (long value) const
{
    engine::engine::throw_exception(nullptr, "cast from integer to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<double> (double value) const
{
    engine::engine::throw_exception(nullptr, "cast from float to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<type::string_t> (type::string_t) const
{
    engine::engine::throw_exception(nullptr, "cast from string to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<type::vector_t> (type::vector_t) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<map<type::string_t,type::value_t>> (map<type::string_t,type::value_t>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void> res) const
{
    return res;
}
*/
#endif