
#include "standard.h"

#include <cstdlib>

namespace rephp {

namespace lang {

using namespace std;
using namespace rephp::engine;

template<typename T1, typename T2>
type::value_t
scalar_worker_add<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    if (parameters.size() != 1) {
        rephp::engine::engine::throw_exception(nullptr, "Invalid operator call.");
        return rephp::engine::engine::invoke("Null", 0);
    }

    type::value_t return_value = self->copy();

    auto left = self->get<T1>();
    auto right = static_cast<T1>(parameters[0]->get<T2>());

    return_value->set(left + right);

    return return_value;
}

template<typename T1, typename T2>
type::value_t
scalar_worker_sub<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    if (parameters.size() != 1) {
        rephp::engine::engine::throw_exception(nullptr, "Invalid operator call.");
        return rephp::engine::engine::invoke("Null", 0);
    }

    type::value_t return_value = self->copy();

    auto left = self->get<T1>();
    auto right = static_cast<T1>(parameters[0]->get<T2>());

    return_value->set(left - right);

    return return_value;
}

template<typename T1, typename T2>
type::value_t
scalar_worker_mul<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    if (parameters.size() != 1) {
        rephp::engine::engine::throw_exception(nullptr, "Invalid operator call.");
        return rephp::engine::engine::invoke("Null", 0);
    }

    type::value_t return_value = self->copy();

    auto left = self->get<T1>();
    auto right = static_cast<T1>(parameters[0]->get<T2>());

    return_value->set(left * right);

    return return_value;
}

template<typename T1, typename T2>
type::value_t
scalar_worker_div<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    if (parameters.size() != 1) {
        rephp::engine::engine::throw_exception(nullptr, "Invalid operator call.");
        return rephp::engine::engine::invoke("Null", 0);
    }

    type::value_t return_value = self->copy();

    auto left = self->get<T1>();
    auto right = static_cast<T1>(parameters[0]->get<T2>());

    return_value->set(left / right);

    return return_value;
}

template<typename T1, typename T2>
type::value_t
scalar_worker_mod<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    if (parameters.size() != 1) {
        rephp::engine::engine::throw_exception(nullptr, "Invalid operator call.");
        return rephp::engine::engine::invoke("Null", 0);
    }

    type::value_t return_value = self->copy();

    auto left = self->get<T1>();
    auto right = static_cast<T1>(parameters[0]->get<T2>());

    return_value->set(left % right);

    return return_value;
}

template<typename T1, typename T2>
type::value_t
scalar_worker_intdiv<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    if (parameters.size() != 1) {
        rephp::engine::engine::throw_exception(nullptr, "Invalid operator call.");
        return rephp::engine::engine::invoke("Null", 0);
    }

    type::value_t return_value = self->copy();

    auto left = self->get<T1>();
    auto right = static_cast<T1>(parameters[0]->get<T2>());

    return_value->set(left - right);

    return return_value;
}
/*
template<typename T>
type::value_t
scalar_worker_sub<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    auto cur = self->get<T>();
    if (parameters.size() != 2) {
        return false;
    }

    auto offset = parameters[0]->get<T::size_type>();
    auto value = parameters[1]->get<T::value_type>();

    shared_ptr<T> result(new T(*cur));
    (*result)[offset] = value;
    return_value->set<T>(result);

    return true;
}

template<typename T>
type::value_t
scalar_worker_sub<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    auto cur = self->get<T>();
    if (parameters.size() != 2) {
        return false;
    }

    auto offset = parameters[0]->get<T::size_type>();

    return_value->set<T::value_type>((*cur)[offset]);

    return true;
}

template<typename T>
type::value_t
scalar_worker_sub<T1, T2>::operator() (type::call_parameters_list_t parameters, type::value_t self)
{
    auto cur = self->get<T>();
    if (parameters.size() > 1) {
        return false;
    }

    auto operand = parameters[0]->get<T>();

    shared_ptr<T> result(new T(*cur));
    result->append(*operand);
    return_value->set<T>(result);

    return true;
}
*/
standard_bundle::standard_bundle(const char *const name): standard_bundle(type::string_t(name))
{}

standard_bundle::standard_bundle(type::string_t name): bundle(name)
{
/*
    auto worker_unary_operator_registration = [](type::type_t ce, type::operator_t op, type::worker_t worker) {
        type::prototype_t proto(new callable_prototype(ce));

        ce->add_operator(op, proto, worker);
    };
*/
    auto worker_binary_operator_registration = [](type::type_t ce, type::operator_t op, type::worker_t worker, type::type_t operand_ce) {
        type::prototype_t proto(new callable_prototype(ce));
        proto->add_parameter("operand", operand_ce);

        ce->add_operator(op, proto, worker);
    };
    auto worker_ternary_operator_registration = [](type::type_t ce, type::operator_t op, type::worker_t worker, type::type_t first_operand_ce, type::type_t second_operand_ce) {
        type::prototype_t proto(new callable_prototype(ce));
        proto->add_parameter("first", first_operand_ce);
        proto->add_parameter("second", second_operand_ce);

        ce->add_operator(op, proto, worker);
    };

    type::type_t ce_mixed(new internal_type("Mixed", TYP_NATIVE_TYPE));
    this->register_class(ce_mixed);

    type::type_t ce_null(new internal_type("Null", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_null);

    type::type_t ce_scalar(new internal_type("Scalar", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_scalar);

    type::type_t ce_boolean(new internal_type("Boolean", TYP_NATIVE_TYPE, ce_scalar));
    this->register_class(ce_boolean);

    type::type_t ce_integer(new internal_type("Integer", TYP_NATIVE_TYPE, ce_scalar));
    this->register_class(ce_integer);

    type::type_t ce_float(new internal_type("Float", TYP_NATIVE_TYPE, ce_scalar));
    this->register_class(ce_float);

    worker_binary_operator_registration(ce_integer, OP_ADD, type::worker_t(new scalar_worker_add<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_SUB, type::worker_t(new scalar_worker_sub<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_MUL, type::worker_t(new scalar_worker_mul<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_DIV, type::worker_t(new scalar_worker_div<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_MOD, type::worker_t(new scalar_worker_mod<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_INTDIV, type::worker_t(new scalar_worker_intdiv<long, long>()), ce_integer);

    worker_binary_operator_registration(ce_integer, OP_ADD, type::worker_t(new scalar_worker_add<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_SUB, type::worker_t(new scalar_worker_sub<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_MUL, type::worker_t(new scalar_worker_mul<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_DIV, type::worker_t(new scalar_worker_div<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_INTDIV, type::worker_t(new scalar_worker_intdiv<long, double>()), ce_float);

    worker_binary_operator_registration(ce_float, OP_ADD, type::worker_t(new scalar_worker_add<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_SUB, type::worker_t(new scalar_worker_sub<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_MUL, type::worker_t(new scalar_worker_mul<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_DIV, type::worker_t(new scalar_worker_div<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_INTDIV, type::worker_t(new scalar_worker_intdiv<double, double>()), ce_float);

    worker_binary_operator_registration(ce_float, OP_ADD, type::worker_t(new scalar_worker_add<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_SUB, type::worker_t(new scalar_worker_sub<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_MUL, type::worker_t(new scalar_worker_mul<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_DIV, type::worker_t(new scalar_worker_div<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_INTDIV, type::worker_t(new scalar_worker_intdiv<double, long>()), ce_integer);

//    type::type_t ce_char(new internal_type("Char", TYP_NATIVE_TYPE, ce_mixed));
//    this->register_class(ce_char);
//
//    type::type_t ce_callable(new internal_type("Callable", TYP_NATIVE_TYPE, ce_mixed));
//    this->register_class(ce_callable);
//
//    type::type_t ce_composite(new internal_type("Composite", TYP_NATIVE_TYPE, ce_mixed));
//    this->register_class(ce_composite);
//
//    type::type_t ce_traversable(new internal_type("Traversable", TYP_INTERFACE));
//    ce_traversable->add_interface(ce_composite);
//    this->register_class(ce_traversable);
//
//    type::type_t ce_iterator(new internal_type("Iterator", TYP_INTERFACE));
//    ce_iterator->add_interface(ce_traversable);
//    this->register_class(ce_iterator);
//
//    type::type_t ce_iterator_aggregate(new internal_type("IteratorAggregate", TYP_INTERFACE));
//    ce_iterator_aggregate->add_interface(ce_traversable);
//    this->register_class(ce_iterator_aggregate);
//    ce_iterator_aggregate->add_method("getIterator", type::prototype_t(new callable_prototype(ce_iterator)), nullptr, ACC_PUBLIC);
//
//    type::type_t ce_countable(new internal_type("Countable", TYP_INTERFACE));
//    ce_countable->add_interface(ce_composite);
//    this->register_class(ce_countable);
//    ce_countable->add_method("count", type::prototype_t(new callable_prototype(ce_integer)), nullptr, ACC_PUBLIC);
//
//    type::type_t ce_sortable(new internal_type("Sortable", TYP_INTERFACE));
//    ce_sortable->add_interface(ce_composite);
//    this->register_class(ce_sortable);
//
//    type::type_t ce_sorter(new internal_type("Sorter", TYP_INTERFACE));
//    this->register_class(ce_sorter);
//    auto proto_sorter_sort = type::prototype_t(new callable_prototype(ce_null));
//    proto_sorter_sort->add_parameter("sortableCollection", ce_sortable);
//    ce_sorter->add_method("sort", proto_sorter_sort, nullptr, ACC_PUBLIC);
//
//    type::type_t ce_string(new internal_type("String", TYP_NATIVE_TYPE, ce_composite));
//    ce_string->add_interface(ce_traversable);
//    ce_string->add_interface(ce_countable);
//    ce_string->add_interface(ce_sortable);
//    this->register_class(ce_string);
//
//    auto proto_string_foo = type::prototype_t(new callable_prototype(ce_null));
//    proto_string_foo->add_parameter("param1", ce_integer);
//    proto_string_foo->add_parameter("param2", ce_string);
//    proto_string_foo->add_parameter("param3", ce_float, this->invoke("Float", 2.));
//    ce_string->add_method("sort", proto_string_foo, nullptr, ACC_PUBLIC);
//
//    worker_ternary_operator_registration(ce_string, OP_OFFSET_SET, type::worker_t(new string_worker_offset_set<type::string_t>()), ce_integer, ce_char);
//    worker_binary_operator_registration(ce_string, OP_OFFSET_GET, type::worker_t(new string_worker_offset_get<type::string_t>()), ce_integer);
//    worker_binary_operator_registration(ce_string, OP_DOT, type::worker_t(new string_worker_add<type::string_t>()), ce_string);
}


};

};