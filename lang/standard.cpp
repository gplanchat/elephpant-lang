
#include "standard.h"

#include <cstdlib>

namespace rephp {

namespace lang {

using namespace std;
using namespace rephp::engine;

template<typename T1, typename T2>
bool scalar_worker_add<T1, T2>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
{
    auto cur = self->get<T1>();
    if (parameters.size() != 1) {
        return false;
    }

    auto operand = parameters[0]->get<T2>();

    return_value->set<T1>(cur + operand);

    return true;
}

template<typename T1, typename T2>
bool
scalar_worker_sub<T1, T2>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
{
    auto cur = self->get<T1>();
    if (parameters.size() != 1) {
        return false;
    }

    auto operand = parameters[0]->get<T2>();

    return_value->set<T1>(cur - operand);

    return true;
}

template<typename T1, typename T2>
bool
scalar_worker_mul<T1, T2>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
{
    auto cur = self->get<T1>();
    if (parameters.size() != 1) {
        return false;
    }

    auto operand = parameters[0]->get<T2>();

    return_value->set<T1>(cur * operand);

    return true;
}

template<typename T1, typename T2>
bool
scalar_worker_div<T1, T2>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
{
    auto cur = self->get<T1>();
    if (parameters.size() != 1) {
        return false;
    }

    auto operand = parameters[0]->get<T2>();

    return_value->set<T1>(cur / operand);

    return true;
}

template<typename T1, typename T2>
bool
scalar_worker_mod<T1, T2>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
{
    auto cur = self->get<T1>();
    if (parameters.size() != 1) {
        return false;
    }

    auto operand = parameters[0]->get<T2>();
    auto result = std::div(cur, operand);

    return_value->set<T1>(result.rem);

    return true;
}

template<typename T1, typename T2>
bool
scalar_worker_intdiv<T1, T2>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
{
    auto cur = self->get<T1>();
    if (parameters.size() != 1) {
        return false;
    }

    auto operand = parameters[0]->get<T2>();
    auto result = std::div(cur, operand);

    return_value->set<T1>(result.quot);

    return true;
}

template<typename T>
bool
string_worker_offset_set<T>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
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
bool
string_worker_offset_get<T>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
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
bool string_worker_add<T>::operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value)
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

standard_bundle::standard_bundle(const char *const name): standard_bundle(string(name))
{}

standard_bundle::standard_bundle(string name): bundle(name)
{
/*
    auto worker_unary_operator_registration = [](shared_ptr<internal_type> ce, engine_operator op, shared_ptr<worker> worker) {
        shared_ptr<callable_prototype> proto(new callable_prototype(ce));

        ce->add_operator(op, proto, worker);
    };
*/
    auto worker_binary_operator_registration = [](shared_ptr<internal_type> ce, engine_operator op, shared_ptr<worker> worker, shared_ptr<internal_type> operand_ce) {
        shared_ptr<callable_prototype> proto(new callable_prototype(ce));
        proto->add_parameter("operand", operand_ce);

        ce->add_operator(op, proto, worker);
    };
    auto worker_ternary_operator_registration = [](shared_ptr<internal_type> ce, engine_operator op, shared_ptr<worker> worker, shared_ptr<internal_type> first_operand_ce, shared_ptr<internal_type> second_operand_ce) {
        shared_ptr<callable_prototype> proto(new callable_prototype(ce));
        proto->add_parameter("first", first_operand_ce);
        proto->add_parameter("second", second_operand_ce);

        ce->add_operator(op, proto, worker);
    };

    shared_ptr<internal_type> ce_mixed(new internal_type("Mixed", TYP_NATIVE_TYPE));
    this->register_class(ce_mixed);

    shared_ptr<internal_type> ce_null(new internal_type("Null", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_null);

    shared_ptr<internal_type> ce_scalar(new internal_type("Scalar", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_scalar);

    shared_ptr<internal_type> ce_boolean(new internal_type("Boolean", TYP_NATIVE_TYPE, ce_scalar));
    this->register_class(ce_boolean);

    shared_ptr<internal_type> ce_integer(new internal_type("Integer", TYP_NATIVE_TYPE, ce_scalar));
    this->register_class(ce_integer);

    shared_ptr<internal_type> ce_float(new internal_type("Float", TYP_NATIVE_TYPE, ce_scalar));
    this->register_class(ce_float);

    worker_binary_operator_registration(ce_integer, OP_ADD, shared_ptr<worker>(new scalar_worker_add<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_SUB, shared_ptr<worker>(new scalar_worker_sub<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_MUL, shared_ptr<worker>(new scalar_worker_mul<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_DIV, shared_ptr<worker>(new scalar_worker_div<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_MOD, shared_ptr<worker>(new scalar_worker_mod<long, long>()), ce_integer);
    worker_binary_operator_registration(ce_integer, OP_INTDIV, shared_ptr<worker>(new scalar_worker_intdiv<long, long>()), ce_integer);

    worker_binary_operator_registration(ce_integer, OP_ADD, shared_ptr<worker>(new scalar_worker_add<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_SUB, shared_ptr<worker>(new scalar_worker_sub<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_MUL, shared_ptr<worker>(new scalar_worker_mul<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_DIV, shared_ptr<worker>(new scalar_worker_div<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_MOD, shared_ptr<worker>(new scalar_worker_mod<long, double>()), ce_float);
    worker_binary_operator_registration(ce_integer, OP_INTDIV, shared_ptr<worker>(new scalar_worker_intdiv<long, double>()), ce_float);

    worker_binary_operator_registration(ce_float, OP_ADD, shared_ptr<worker>(new scalar_worker_add<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_SUB, shared_ptr<worker>(new scalar_worker_sub<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_MUL, shared_ptr<worker>(new scalar_worker_mul<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_DIV, shared_ptr<worker>(new scalar_worker_div<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_MOD, shared_ptr<worker>(new scalar_worker_mod<double, double>()), ce_float);
    worker_binary_operator_registration(ce_float, OP_INTDIV, shared_ptr<worker>(new scalar_worker_intdiv<double, double>()), ce_float);

    worker_binary_operator_registration(ce_float, OP_ADD, shared_ptr<worker>(new scalar_worker_add<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_SUB, shared_ptr<worker>(new scalar_worker_sub<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_MUL, shared_ptr<worker>(new scalar_worker_mul<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_DIV, shared_ptr<worker>(new scalar_worker_div<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_MOD, shared_ptr<worker>(new scalar_worker_mod<double, long>()), ce_integer);
    worker_binary_operator_registration(ce_float, OP_INTDIV, shared_ptr<worker>(new scalar_worker_intdiv<double, long>()), ce_integer);

    shared_ptr<internal_type> ce_char(new internal_type("Char", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_char);

    shared_ptr<internal_type> ce_callable(new internal_type("Callable", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_callable);

    shared_ptr<internal_type> ce_composite(new internal_type("Composite", TYP_NATIVE_TYPE, ce_mixed));
    this->register_class(ce_composite);

    shared_ptr<internal_type> ce_traversable(new internal_type("Traversable", TYP_INTERFACE));
    ce_traversable->add_interface(ce_composite);
    this->register_class(ce_traversable);

    shared_ptr<internal_type> ce_iterator(new internal_type("Iterator", TYP_INTERFACE));
    ce_iterator->add_interface(ce_traversable);
    this->register_class(ce_iterator);

    shared_ptr<internal_type> ce_iterator_aggregate(new internal_type("IteratorAggregate", TYP_INTERFACE));
    ce_iterator_aggregate->add_interface(ce_traversable);
    this->register_class(ce_iterator_aggregate);
    ce_iterator_aggregate->add_method("getIterator", shared_ptr<callable_prototype>(new callable_prototype(ce_iterator)), nullptr, ACC_PUBLIC);

    shared_ptr<internal_type> ce_countable(new internal_type("Countable", TYP_INTERFACE));
    ce_countable->add_interface(ce_composite);
    this->register_class(ce_countable);
    ce_countable->add_method("count", shared_ptr<callable_prototype>(new callable_prototype(ce_integer)), nullptr, ACC_PUBLIC);

    shared_ptr<internal_type> ce_sortable(new internal_type("Sortable", TYP_INTERFACE));
    ce_sortable->add_interface(ce_composite);
    this->register_class(ce_sortable);

    shared_ptr<internal_type> ce_sorter(new internal_type("Sorter", TYP_INTERFACE));
    this->register_class(ce_sorter);
    auto proto_sorter_sort = shared_ptr<callable_prototype>(new callable_prototype(ce_null));
    proto_sorter_sort->add_parameter("sortableCollection", ce_sortable);
    ce_sorter->add_method("sort", proto_sorter_sort, nullptr, ACC_PUBLIC);

    shared_ptr<internal_type> ce_string(new internal_type("String", TYP_NATIVE_TYPE, ce_composite));
    ce_string->add_interface(ce_traversable);
    ce_string->add_interface(ce_countable);
    ce_string->add_interface(ce_sortable);
    this->register_class(ce_string);

    auto proto_string_foo = shared_ptr<callable_prototype>(new callable_prototype(ce_null));
    proto_string_foo->add_parameter("param1", ce_integer);
    proto_string_foo->add_parameter("param2", ce_string);
    proto_string_foo->add_parameter("param3", ce_float, this->invoke("Float", 2.));
    ce_string->add_method("sort", proto_string_foo, nullptr, ACC_PUBLIC);

    worker_ternary_operator_registration(ce_string, OP_OFFSET_SET, shared_ptr<worker>(new string_worker_offset_set<string>()), ce_integer, ce_char);
    worker_binary_operator_registration(ce_string, OP_OFFSET_GET, shared_ptr<worker>(new string_worker_offset_get<string>()), ce_integer);
    worker_binary_operator_registration(ce_string, OP_DOT, shared_ptr<worker>(new string_worker_add<string>()), ce_string);
}


};

};