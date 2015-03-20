
#ifndef REPHP_ENGINE_H
#define REPHP_ENGINE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

typedef enum access_mode {
    ACC_PUBLIC,
    ACC_PROTECTED,
    ACC_PRIVATE,
    ACC_PACKAGE
} access_mode_t;

typedef enum class_type {
    TYP_CLASS,
    TYP_INTERFACE,
    TYP_TRAIT,
    TYP_ENUM
} class_type_t;

typedef enum engine_operator {
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
    OP_OFFSET_EXISTS
} engine_operator_t;

class worker;
class callable_prototype;
class method_entry;
class internal_type;
class internal_value;
class bundle;

class worker
{
public:
    bool operator() (shared_ptr<internal_value> self, vector<shared_ptr<internal_value>> parameters, shared_ptr<internal_value> return_value);
};

class callable_prototype: public enable_shared_from_this<callable_prototype>
{
    friend rephp::reflection::callable_prototype;

private:
    typedef unordered_map<string, pair<shared_ptr<internal_type>, shared_ptr<internal_value>>> parameters_map_t;

    shared_ptr<internal_type> return_type;
    parameters_map_t          parameters;

public:
    callable_prototype();
    callable_prototype(shared_ptr<internal_type> return_type);

    bool match(shared_ptr<callable_prototype> prototype);
    bool call_match(vector<shared_ptr<internal_value>> params);

    void add_parameter(string name, shared_ptr<internal_type> parameter);
    void add_parameter(string name, shared_ptr<internal_type> parameter, shared_ptr<internal_value> default_value);

    size_t count();
};

class method_entry: public enable_shared_from_this<method_entry>
{
    friend rephp::reflection::method_entry;

private:
    shared_ptr<callable_prototype> prototype;
    shared_ptr<worker>             worker_callable;
    access_mode                    access;

public:
    method_entry(shared_ptr<callable_prototype> prototype, shared_ptr<worker> cb, access_mode access);

    bool matches_prototype(shared_ptr<callable_prototype> prototype);
    bool matches_access_mode(access_mode mode);
};

class internal_type: public enable_shared_from_this<internal_type>
{
    friend rephp::reflection::internal_type;

private:
    typedef unordered_map<string, shared_ptr<internal_type>>                    class_map_t;
    typedef unordered_map<string, shared_ptr<internal_value>>                   const_map_t;
    typedef unordered_map<string, pair<shared_ptr<internal_type>, access_mode>> attributes_map_t;
    typedef unordered_multimap<string, shared_ptr<method_entry>>                methods_map_t;
    typedef unordered_multimap<engine_operator, shared_ptr<method_entry>>       operators_map_t;

    shared_ptr<internal_type> parent;
    class_map_t               interfaces;
    class_map_t               traits;

    string     package;
    string     name;
    class_type type;

    const_map_t      consts;
    attributes_map_t attributes;
    methods_map_t    methods;
    operators_map_t  operators;

public:
    internal_type(string name, class_type type, shared_ptr<internal_type> parent = nullptr);

    void add_interface(shared_ptr<internal_type> interface);
    void add_trait(shared_ptr<internal_type> trait);
    void add_attribute(string name, shared_ptr<internal_type> type, access_mode access = ACC_PRIVATE);
    void add_method(string name, shared_ptr<callable_prototype> prototype, shared_ptr<worker> cb, access_mode access = ACC_PRIVATE);
    void add_operator(engine_operator op, shared_ptr<callable_prototype> prototype, shared_ptr<worker> cb, access_mode access = ACC_PUBLIC);

    bool has_parent(string parent);
    bool has_parent(shared_ptr<internal_type> parent);
    bool has_interface(string interface);
    bool has_interface(shared_ptr<internal_type> interface);
    bool has_trait(string trait);
    bool has_trait(shared_ptr<internal_type> trait);
    bool has_trait_recursive(string trait);
    bool has_trait_recursive(shared_ptr<internal_type> trait);
    bool has_method(string name, shared_ptr<callable_prototype> prototype, shared_ptr<internal_value> context);
    bool has_operator(engine_operator op, shared_ptr<callable_prototype> prototype, shared_ptr<internal_value> context);
    bool has_attribute(string name, shared_ptr<internal_value> context);

    string get_name();
    shared_ptr<internal_type> get_parent();
};

class internal_value: public enable_shared_from_this<internal_value>
{
    friend rephp::reflection::internal_value;

private:
    typedef unordered_map<string, shared_ptr<internal_value>> attributes_map_t;

    shared_ptr<internal_type>                         type;
    unordered_map<string, shared_ptr<internal_value>> attributes;
    shared_ptr<void>                                  raw_value_ptr;

public:
    internal_value(shared_ptr<internal_type> type);

    shared_ptr<internal_type> get_type();

    shared_ptr<void> get_ptr();
    void set_ptr(shared_ptr<void> &value);

    template<typename T>
    const T get() const;

    template<typename T>
    void set(const T &value);
};

class bundle: public enable_shared_from_this<bundle>
{
    friend rephp::reflection::bundle;

private:
    typedef unordered_map<string, shared_ptr<internal_type>> classes_map_t;

    string name;

    classes_map_t classes;

protected:
    void register_class(shared_ptr<internal_type> ce);

    shared_ptr<internal_type> get_class(string name);

public:
    bundle(const char *const name);
    bundle(string name);

    void merge(string ns, shared_ptr<bundle> sub_bundle);

    string get_name();
};

};

};

#endif