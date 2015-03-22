
#ifndef REPHP_LANG_STANDARD_H
#define REPHP_LANG_STANDARD_H

#include <memory>
#include <vector>

#include "../engine/engine.h"

namespace rephp {

namespace lang {

using namespace std;
using namespace rephp::engine;

template<typename T1, typename T2>
class scalar_worker_add: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T1, typename T2>
class scalar_worker_sub: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T1, typename T2>
class scalar_worker_mul: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T1, typename T2>
class scalar_worker_div: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T1, typename T2>
class scalar_worker_mod: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T1, typename T2>
class scalar_worker_intdiv: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T>
class string_worker_offset_set: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T>
class string_worker_offset_get: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename T>
class string_worker_add: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};
/*
template<typename K, typename V>
class collection_worker_offset_set: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename K, typename R>
class collection_worker_offset_get: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename K>
class collection_worker_offset_unset: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};

template<typename K, typename R>
class collection_worker_offset_exists: public worker
{
public:
    type::value_t operator() (type::call_parameters_list_t parameters, type::value_t self);
};
*/

class standard_bundle: public bundle
{
public:
    standard_bundle(const char *const name);
    standard_bundle(type::string_t name);
};

};

};

#endif