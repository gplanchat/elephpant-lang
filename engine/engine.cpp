

#include <algorithm>

#include "engine.h"

namespace rephp {

namespace engine {

using namespace std;

bool worker::operator() (shared_ptr<internal_value>, vector<shared_ptr<internal_value>>, shared_ptr<internal_value>)
{
    return false;
}

callable_prototype::callable_prototype()
{}

callable_prototype::callable_prototype(shared_ptr<internal_type> return_type) :
    return_type(return_type)
{}

bool
callable_prototype::match(shared_ptr<callable_prototype> prototype)
{
    return equal(
        this->parameters.begin(),
        this->parameters.end(),
        prototype->parameters.begin(),
        prototype->parameters.end(),
        [](parameters_map_t::value_type left, parameters_map_t::value_type right) {
            return left.first == right.first;
        }
    );
}

bool
callable_prototype::call_match(vector<shared_ptr<internal_value>> params)
{
    bool passed_mandatory = false;
    auto it = this->parameters.begin();
    auto cur = params.begin();
    for (;it != this->parameters.end() && cur != params.end(); ++it, ++cur) {
        if (it->second.second != nullptr) {
            passed_mandatory = true;
        }
        auto cur_type = (*cur)->get_type();
        if (cur_type != it->second.second->get_type() && !cur_type->has_parent(it->first) && !cur_type->has_interface(it->first)) {
            return false;
        }
    }

    return passed_mandatory;
}

void
callable_prototype::add_parameter(string name, shared_ptr<internal_type> parameter)
{
    this->parameters[name] = parameters_map_t::mapped_type(parameter, nullptr);
}

void
callable_prototype::add_parameter(string name, shared_ptr<internal_type> parameter, shared_ptr<internal_value> default_value)
{
    this->parameters[name] = parameters_map_t::mapped_type(parameter, default_value);
}

size_t
callable_prototype::count()
{
    return this->parameters.size();
}

method_entry::method_entry(shared_ptr<callable_prototype> prototype, shared_ptr<worker> cb, access_mode access):
    prototype(prototype), worker_callable(cb), access(access)
{}

bool
method_entry::matches_prototype(shared_ptr<callable_prototype> prototype)
{
    return this->prototype->match(prototype);
}

bool
method_entry::matches_access_mode(access_mode mode)
{
    return this->access == mode;
}

internal_type::internal_type(string name, class_type type, shared_ptr<internal_type> parent):
    parent(parent),
    name(name),
    type(type)
{}

void
internal_type::add_interface(shared_ptr<internal_type> interface)
{
    this->interfaces[interface->get_name()] = interface;
}

void
internal_type::add_trait(shared_ptr<internal_type> trait)
{
    this->traits[trait->get_name()] = trait;
}

void
internal_type::add_attribute(string name, shared_ptr<internal_type> type, access_mode access)
{
    this->attributes[name] = attributes_map_t::mapped_type(type, access);
}

void
internal_type::add_method(string name, shared_ptr<callable_prototype> prototype, shared_ptr<worker> cb, access_mode access)
{
    shared_ptr<method_entry> me(new method_entry(prototype, cb, access));
    this->methods.insert(make_pair(name, me));
}

void
internal_type::add_operator(engine_operator op, shared_ptr<callable_prototype> prototype, shared_ptr<worker> cb, access_mode access)
{
    shared_ptr<method_entry> oe(new method_entry(prototype, cb, access));
    this->operators.insert(make_pair(op, oe));
}

bool
internal_type::has_parent(string parent)
{
    if (!this->parent) {
        return false;
    }

    if (parent == this->parent->get_name()) {
        return true;
    }

    return this->parent->has_parent(parent);
}

bool
internal_type::has_parent(shared_ptr<internal_type> parent)
{
    if (!this->parent) {
        return false;
    }

    if (parent == this->parent) {
        return true;
    }

    return this->parent->has_parent(parent);
}

bool
internal_type::has_interface(string interface)
{
    for (auto it = this->interfaces.begin(); it != this->interfaces.end(); ++it) {
        if (it->first == interface) {
            return true;
        }
        if (it->second->has_interface(interface)) {
            return true;
        }
    }

    return false;
}

bool
internal_type::has_interface(shared_ptr<internal_type> interface)
{
    for (auto it = this->interfaces.begin(); it != this->interfaces.end(); ++it) {
        if (it->second == interface) {
            return true;
        }
        if (it->second->has_interface(interface)) {
            return true;
        }
    }

    return false;
}

bool
internal_type::has_trait(string trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](class_map_t::value_type current) {
        if (current.second->get_name() == trait) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

bool
internal_type::has_trait(shared_ptr<internal_type> trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](class_map_t::value_type current) {
        if (current.second == trait) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

bool
internal_type::has_trait_recursive(string trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](class_map_t::value_type current){
        if (current.second->get_name() == trait) {
            return true;
        }
        if (current.second->has_trait(trait)) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

bool
internal_type::has_trait_recursive(shared_ptr<internal_type> trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](class_map_t::value_type current){
        if (current.second == trait) {
            return true;
        }
        if (current.second->has_trait(trait)) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

shared_ptr<method_entry>
internal_type::find_method(string name, shared_ptr<callable_prototype> prototype, shared_ptr<internal_value> context)
{
    auto range = this->methods.equal_range(name);
    if (range.first == range.second) {
        return nullptr;
    }

    auto context_type = context->get_type();
    auto this_shared = shared_from_this();

    auto found = find_if(range.first, range.second, [&](methods_map_t::value_type &method) {
        if (!method.second->matches_prototype(prototype)) {
            return false;
        }

        if (this_shared == context_type || method.second->matches_access_mode(ACC_PUBLIC)) {
            return true;
        }

        if (this->type == TYP_TRAIT) {
            if (context_type->has_trait(this_shared)) {
                return true;
            }
        } else if (this->type == TYP_CLASS) {
            if (context_type->has_parent(this_shared)) {
                if (method.second->matches_access_mode(ACC_PROTECTED)) {
                    return true;
                }

                return false;
            }

            return false;
        } else if (this->type == TYP_INTERFACE) {
            if (context_type->has_interface(this_shared)) {
                if (method.second->matches_access_mode(ACC_PROTECTED)) {
                    return true;
                }

                return false;
            }
        }

        return false;
    });

    if (found == range.second) {
        return nullptr;
    }

    return found->second;
}

bool
internal_type::has_method(string name, shared_ptr<callable_prototype> prototype, shared_ptr<internal_value> context)
{
    return this->find_method(name, prototype, context) != nullptr;
}

shared_ptr<method_entry>
internal_type::find_operator(engine_operator op, shared_ptr<callable_prototype> prototype, shared_ptr<internal_value> context)
{
    auto range = this->operators.equal_range(op);
    if (range.first == range.second) {
        return nullptr;
    }

    auto context_type = context->get_type();
    auto this_shared = shared_from_this();

    auto found = find_if(range.first, range.second, [&](operators_map_t::value_type &op) {
        if (!op.second->matches_prototype(prototype)) {
            return false;
        }

        if (this_shared == context_type || op.second->matches_access_mode(ACC_PUBLIC)) {
            return true;
        }

        if (this->type == TYP_TRAIT) {
            if (context_type->has_trait(this_shared)) {
                return true;
            }
        } else if (this->type == TYP_CLASS) {
            if (context_type->has_parent(this_shared)) {
                if (op.second->matches_access_mode(ACC_PROTECTED)) {
                    return true;
                }

                return false;
            }

            return false;
        } else if (this->type == TYP_INTERFACE) {
            if (context_type->has_interface(this_shared)) {
                if (op.second->matches_access_mode(ACC_PROTECTED)) {
                    return true;
                }

                return false;
            }
        }

        return false;
    });

    if (found == range.second) {
        return nullptr;
    }

    return found->second;
}

bool
internal_type::has_operator(engine_operator op, shared_ptr<callable_prototype> prototype, shared_ptr<internal_value> context)
{
    return this->find_operator(op, prototype, context) != nullptr;
}

bool
internal_type::has_attribute(string name, shared_ptr<internal_value> context)
{
    auto it = this->attributes.find(name);
    if (it == this->attributes.end()) {
        return false;
    }

    auto context_type = context->get_type();
    auto this_shared = shared_from_this();

    if (this_shared == context_type || it->second.second == ACC_PUBLIC) {
        return true;
    }

    if (this->type == TYP_TRAIT) {
        if (context_type->has_trait(this_shared)) {
            return true;
        }
    } else if (this->type == TYP_CLASS) {
        if (context_type->has_parent(this_shared)) {
            if (it->second.second == ACC_PROTECTED) {
                return true;
            }

            return false;
        }

        return false;
    } else if (this->type == TYP_INTERFACE) {
        if (context_type->has_interface(this_shared)) {
            if (it->second.second == ACC_PROTECTED) {
                return true;
            }

            return false;
        }
    }

    return false;
}

string
internal_type::get_name()
{
    return name;
}

shared_ptr<internal_type>
internal_type::get_parent()
{
    return parent;
}


internal_value::internal_value(shared_ptr<internal_type> type):
    type(type)
{}

shared_ptr<internal_type>
internal_value::get_type()
{
    return this->type;
}

bundle::bundle(const char *const name): name(name)
{}

bundle::bundle(string name): name(name)
{}

void
bundle::register_class(shared_ptr<internal_type> ce)
{
    this->classes[ce->get_name()] = ce;
}

shared_ptr<internal_type>
bundle::get_class(string name)
{
    return this->classes[name];
}

void
bundle::merge(string, shared_ptr<bundle> sub_bundle)
{
    this->classes.insert(sub_bundle->classes.begin(), sub_bundle->classes.end());
}

string
bundle::get_name()
{
    return this->name;
}

template<>
inline shared_ptr<internal_value>
bundle::invoke<short>(string &name, short value)
{
    auto type = this->get_class(name);

    shared_ptr<internal_value> item(new internal_value(type));
    long var = static_cast<long>(value);
    item->set<long>(var);

    return item;
}

template<>
inline shared_ptr<internal_value>
bundle::invoke<int>(string &name, int value)
{
    auto type = this->get_class(name);

    shared_ptr<internal_value> item(new internal_value(type));
    long var = static_cast<long>(value);
    item->set<long>(var);

    return item;
}

template<>
inline shared_ptr<internal_value>
bundle::invoke<float>(string &name, float value)
{
    auto type = this->get_class(name);

    shared_ptr<internal_value> item(new internal_value(type));
    double var = static_cast<double>(value);
    item->set<double>(var);

    return item;
}

unordered_map<engine_operator_t, string> engine::operators_map = {
    ADD_OPERATOR(OP_ADD),
    ADD_OPERATOR(OP_SUB),
    ADD_OPERATOR(OP_MUL),
    ADD_OPERATOR(OP_DIV),
    ADD_OPERATOR(OP_MOD),
    ADD_OPERATOR(OP_INTDIV),
    ADD_OPERATOR(OP_AND),
    ADD_OPERATOR(OP_OR),
    ADD_OPERATOR(OP_XOR),
    ADD_OPERATOR(OP_NOT),
    ADD_OPERATOR(OP_ADD),
    ADD_OPERATOR(OP_BITWISE_AND),
    ADD_OPERATOR(OP_BITWISE_OR),
    ADD_OPERATOR(OP_BITWISE_XOR),
    ADD_OPERATOR(OP_BITWISE_NOT),
    ADD_OPERATOR(OP_LHS),
    ADD_OPERATOR(OP_RHS),
    ADD_OPERATOR(OP_OFFSET_SET),
    ADD_OPERATOR(OP_OFFSET_GET),
    ADD_OPERATOR(OP_OFFSET_UNSET),
    ADD_OPERATOR(OP_OFFSET_EXISTS),
    ADD_OPERATOR(OP_DOT)
};

};

};