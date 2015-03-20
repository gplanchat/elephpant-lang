
#ifndef REPHP_ENGINE_VISITORS_H
#define REPHP_ENGINE_VISITORS_H


template<typename T>
class internal_value_scalar_get_visitor: public boost::static_visitor<T>
{
public:
    template<typename U>
    T operator() (U value) const;
};

class internal_value_string_get_visitor: public boost::static_visitor<string>
{
public:
    template<typename T>
    string operator() (T value) const;
};

class internal_value_vector_get_visitor: public boost::static_visitor<vector<shared_ptr<internal_value>>>
{
public:
    template<typename T>
    vector<shared_ptr<internal_value>> operator() (T value) const;
};

class internal_value_map_get_visitor: public boost::static_visitor<map<string,shared_ptr<internal_value>>>
{
public:
    template<typename T>
    map<string,shared_ptr<internal_value>> operator() (T value) const;
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
internal_value_scalar_get_visitor<bool>::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to boolean is not allowed.");
    return 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to boolean is not allowed.");
    return 0;
}

template<>
template<>
bool
internal_value_scalar_get_visitor<bool>::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
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
internal_value_scalar_get_visitor<char>::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to character is not allowed.");
    return 0;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to character is not allowed.");
    return 0;
}

template<>
template<>
char
internal_value_scalar_get_visitor<char>::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
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
internal_value_scalar_get_visitor<long>::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to integer is not allowed.");
    return 0;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to integer is not allowed.");
    return 0;
}

template<>
template<>
long
internal_value_scalar_get_visitor<long>::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
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
internal_value_scalar_get_visitor<double>::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to float is not allowed.");
    return 0;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to float is not allowed.");
    return 0;
}

template<>
template<>
double
internal_value_scalar_get_visitor<double>::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
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
string internal_value_string_get_visitor::operator()<bool> (bool value) const
{
    return value ? "true" : "false";
}

template<>
string internal_value_string_get_visitor::operator()<char> (char value) const
{
    return to_string(value);
}

template<>
string internal_value_string_get_visitor::operator()<long> (long value) const
{
    return to_string(value);
}

template<>
string internal_value_string_get_visitor::operator()<double> (double value) const
{
    return to_string(value);
}

template<>
string internal_value_string_get_visitor::operator()<string> (string str) const
{
    return str;
}

template<>
string internal_value_string_get_visitor::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to string is not allowed.");
    string str = "vector";
    return str;
}

template<>
string internal_value_string_get_visitor::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to string is not allowed.");
    return string("map");
}

template<>
string internal_value_string_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to string is not allowed.");
    return string("resource");
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<bool> (bool) const
{
    engine::engine::throw_exception(nullptr, "cast from boolean to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<char> (char) const
{
    engine::engine::throw_exception(nullptr, "cast from character to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<long> (long) const
{
    engine::engine::throw_exception(nullptr, "cast from integer to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<double> (double) const
{
    engine::engine::throw_exception(nullptr, "cast from float to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>> vec) const
{
    return vec;
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from map to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
vector<shared_ptr<internal_value>>
internal_value_vector_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to vector is not allowed.");
    return vector<shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<bool> (bool value) const
{
    engine::engine::throw_exception(nullptr, "cast from boolean to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<char> (char value) const
{
    engine::engine::throw_exception(nullptr, "cast from character to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<long> (long value) const
{
    engine::engine::throw_exception(nullptr, "cast from integer to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<double> (double value) const
{
    engine::engine::throw_exception(nullptr, "cast from float to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>> array) const
{
    return array;
}

template<>
map<string,shared_ptr<internal_value>>
internal_value_map_get_visitor::operator()<shared_ptr<void>> (shared_ptr<void>) const
{
    engine::engine::throw_exception(nullptr, "cast from resource to map is not allowed.");
    return map<string,shared_ptr<internal_value>>();
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
internal_value_resource_get_visitor::operator()<string> (string) const
{
    engine::engine::throw_exception(nullptr, "cast from string to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<vector<shared_ptr<internal_value>>> (vector<shared_ptr<internal_value>>) const
{
    engine::engine::throw_exception(nullptr, "cast from vector to resource is not allowed.");
    return nullptr;
}

template<>
shared_ptr<void>
internal_value_resource_get_visitor::operator()<map<string,shared_ptr<internal_value>>> (map<string,shared_ptr<internal_value>>) const
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

#endif