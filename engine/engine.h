
#ifndef REPHP_ENGINE_H
#define REPHP_ENGINE_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>

#include <boost/variant.hpp>

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
    OP_DOT
} engine_operator_t;

class worker;
class callable_prototype;
class method_entry;
class internal_type;
class internal_value;
class bundle;

typedef boost::variant<bool,char,long,double,string,vector<shared_ptr<internal_value>>,map<string,shared_ptr<internal_value>>,shared_ptr<void>> internal_variant_t;

class engine
{
private:
    static unordered_map<engine_operator_t, string> operators_map;

public:
    inline static const string &get_operator_string(engine_operator_t op)
    {
        return engine::operators_map[op];
    }

    void static throw_exception(shared_ptr<internal_type> type, const char *message, long code = 0, shared_ptr<internal_value> previous = nullptr)
    {
        std::cerr << "An exception occured: " << message << std::endl;
    }

    void static throw_exception(shared_ptr<internal_type> type, string &message, long code = 0, shared_ptr<internal_value> previous = nullptr)
    {
        std::cerr << "An exception occured: " << message << std::endl;
    }
};

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

    shared_ptr<internal_type> type;
    attributes_map_t          attributes;
    internal_variant_t        raw_value;

public:
    internal_value(shared_ptr<internal_type> type);

    shared_ptr<internal_type> get_type();

    template<typename T>
    inline T get() const;

    template<typename T>
    inline void set(T value)
    {
        this->raw_value = value;
    }
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
inline string
internal_value::get<string>() const
{
    return boost::get<string>(this->raw_value);
}

template<>
inline vector<shared_ptr<internal_value>>
internal_value::get<vector<shared_ptr<internal_value>>>() const
{
    return boost::get<vector<shared_ptr<internal_value>>>(this->raw_value);
}

template<>
inline map<string,shared_ptr<internal_value>>
internal_value::get<map<string,shared_ptr<internal_value>>>() const
{
    return boost::get<map<string,shared_ptr<internal_value>>>(this->raw_value);
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
internal_value::set<string&>(string &value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<vector<shared_ptr<internal_value>>&>(vector<shared_ptr<internal_value>> &value)
{
    this->raw_value = value;
}

template<>
inline void
internal_value::set<map<string,shared_ptr<internal_value>>&>(map<string,shared_ptr<internal_value>> &value)
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

    template<typename T>
    inline shared_ptr<internal_value> invoke(const char *name, T value)
    {
        string str(name);
        return this->invoke<T>(str, value);
    }

    template<typename T>
    inline shared_ptr<internal_value> invoke(string &name, T value)
    {
        auto type = this->get_class(name);

        shared_ptr<internal_value> item(new internal_value(type));
        item->set<T>(value);
        return item;
    }
};

};

};

#endif