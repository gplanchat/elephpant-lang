

#include <algorithm>

#include "engine.h"

namespace rephp {

namespace engine {

using namespace std;

bool worker::operator() (type::value_t, type::vector_t, type::value_t)
{
    return false;
}

callable_prototype::callable_prototype()
{}

callable_prototype::callable_prototype(type::type_t return_type) :
    return_type(return_type)
{}

bool
callable_prototype::match(type::prototype_t prototype)
{
    return equal(
        this->parameters.begin(),
        this->parameters.end(),
        prototype->parameters.begin(),
        prototype->parameters.end(),
        [](type::parameters_map_t::value_type left, type::parameters_map_t::value_type right) {
            return type::get_key(left) == type::get_key(right);
        }
    );
}

bool
callable_prototype::call_match(type::vector_t params)
{
    bool passed_mandatory = false;
    auto it = this->parameters.begin();
    auto cur = params.begin();
    for (;it != this->parameters.end() && cur != params.end(); ++it, ++cur) {
        if (type::get_parameter_type(*it) != nullptr) {
            passed_mandatory = true;
        }
        auto cur_type = (*cur)->get_type();
        if (cur_type != type::get_parameter_value(*it)->get_type() && !cur_type->has_parent(type::get_key(*it)) && !cur_type->has_interface(type::get_key(*it))) {
            return false;
        }
    }

    return passed_mandatory;
}

void
callable_prototype::add_parameter(type::string_t name, type::type_t parameter)
{
    this->parameters[name] = type::parameters_map_t::mapped_type(parameter, nullptr);
}

void
callable_prototype::add_parameter(type::string_t name, type::type_t parameter, type::value_t default_value)
{
    this->parameters[name] = type::parameters_map_t::mapped_type(parameter, default_value);
}

type::size_t
callable_prototype::count()
{
    return this->parameters.size();
}

method_entry::method_entry(type::prototype_t prototype, type::worker_t cb, access_mode access):
    prototype(prototype), worker(cb), access(access)
{}

bool
method_entry::matches_prototype(type::prototype_t prototype)
{
    return this->prototype->match(prototype);
}

bool
method_entry::matches_access_mode(access_mode mode)
{
    return this->access == mode;
}

internal_type::internal_type(type::string_t name, class_type type, type::type_t parent):
    parent(parent),
    name(name),
    type(type)
{}

void
internal_type::add_interface(type::type_t interface)
{
    this->interfaces[interface->get_name()] = interface;
}

void
internal_type::add_trait(type::type_t trait)
{
    this->traits[trait->get_name()] = trait;
}

void
internal_type::add_attribute(type::string_t name, type::type_t type, access_mode access)
{
    this->attributes[name] = type::attributes_map_t::mapped_type(type, access);
}

void
internal_type::add_method(type::string_t name, type::prototype_t prototype, type::worker_t cb, access_mode access)
{
    type::method_t me(new method_entry(prototype, cb, access));
    this->methods.insert(make_pair(name, me));
}

void
internal_type::add_operator(engine_operator op, type::prototype_t prototype, type::worker_t cb, access_mode access)
{
    type::method_t oe(new method_entry(prototype, cb, access));
    this->operators.insert(make_pair(op, oe));
}

bool
internal_type::has_parent(type::string_t parent)
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
internal_type::has_parent(type::type_t parent)
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
internal_type::has_interface(type::string_t interface)
{
    for (auto it = this->interfaces.begin(); it != this->interfaces.end(); ++it) {
        if (type::get_key(*it) == interface) {
            return true;
        }
        if (type::get_value(*it)->has_interface(interface)) {
            return true;
        }
    }

    return false;
}

bool
internal_type::has_interface(type::type_t interface)
{
    for (auto it = this->interfaces.begin(); it != this->interfaces.end(); ++it) {
        if (type::get_value(*it) == interface) {
            return true;
        }
        if (type::get_value(*it)->has_interface(interface)) {
            return true;
        }
    }

    return false;
}

bool
internal_type::has_trait(type::string_t trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](type::classes_map_t::value_type current) {
        if (type::get_value(current)->get_name() == trait) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

bool
internal_type::has_trait(type::type_t trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](type::classes_map_t::value_type current) {
        if (type::get_value(current) == trait) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

bool
internal_type::has_trait_recursive(type::string_t trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](type::classes_map_t::value_type current){
        if (type::get_value(current)->get_name() == trait) {
            return true;
        }
        if (type::get_value(current)->has_trait(trait)) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

bool
internal_type::has_trait_recursive(type::type_t trait)
{
    auto found = find_if(this->traits.begin(), this->traits.end(), [&](type::classes_map_t::value_type current){
        if (type::get_value(current) == trait) {
            return true;
        }
        if (type::get_value(current)->has_trait(trait)) {
            return true;
        }
        return false;
    });

    return found != this->traits.end();
}

type::method_t
internal_type::find_method(type::string_t name, type::prototype_t prototype, type::value_t context)
{
    auto range = this->methods.equal_range(name);
    if (range.first == range.second) {
        return nullptr;
    }

    auto context_type = context->get_type();
    auto this_shared = shared_from_this();

    auto found = find_if(range.first, range.second, [&](type::methods_map_t::value_type &method) {
        if (!type::get_value(method)->matches_prototype(prototype)) {
            return false;
        }

        if (this_shared == context_type || type::get_value(method)->matches_access_mode(ACC_PUBLIC)) {
            return true;
        }

        if (this->type == TYP_TRAIT) {
            if (context_type->has_trait(this_shared)) {
                return true;
            }
        } else if (this->type == TYP_CLASS) {
            if (context_type->has_parent(this_shared)) {
                if (type::get_value(method)->matches_access_mode(ACC_PROTECTED)) {
                    return true;
                }

                return false;
            }

            return false;
        } else if (this->type == TYP_INTERFACE) {
            if (context_type->has_interface(this_shared)) {
                if (type::get_value(method)->matches_access_mode(ACC_PROTECTED)) {
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
internal_type::has_method(type::string_t name, type::prototype_t prototype, type::value_t context)
{
    return this->find_method(name, prototype, context) != nullptr;
}

type::method_t
internal_type::find_operator(engine_operator op, type::prototype_t prototype, type::value_t context)
{
    auto range = this->operators.equal_range(op);
    if (range.first == range.second) {
        return nullptr;
    }

    auto context_type = context->get_type();
    auto this_shared = shared_from_this();

    auto found = find_if(range.first, range.second, [&](type::operators_map_t::value_type &op) {
        if (!type::get_value(op)->matches_prototype(prototype)) {
            return false;
        }

        if (this_shared == context_type || type::get_value(op)->matches_access_mode(ACC_PUBLIC)) {
            return true;
        }

        if (this->type == TYP_TRAIT) {
            if (context_type->has_trait(this_shared)) {
                return true;
            }
        } else if (this->type == TYP_CLASS) {
            if (context_type->has_parent(this_shared)) {
                if (type::get_value(op)->matches_access_mode(ACC_PROTECTED)) {
                    return true;
                }

                return false;
            }

            return false;
        } else if (this->type == TYP_INTERFACE) {
            if (context_type->has_interface(this_shared)) {
                if (type::get_value(op)->matches_access_mode(ACC_PROTECTED)) {
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
internal_type::has_operator(engine_operator op, type::prototype_t prototype, type::value_t context)
{
    return this->find_operator(op, prototype, context) != nullptr;
}

bool
internal_type::has_attribute(type::string_t name, type::value_t context)
{
    auto it = this->attributes.find(name);
    if (it == this->attributes.end()) {
        return false;
    }

    auto context_type = context->get_type();
    auto this_shared = shared_from_this();

    if (this_shared == context_type || type::get_attribute_access_mode(*it) == ACC_PUBLIC) {
        return true;
    }

    if (this->type == TYP_TRAIT) {
        if (context_type->has_trait(this_shared)) {
            return true;
        }
    } else if (this->type == TYP_CLASS) {
        if (context_type->has_parent(this_shared)) {
            if (type::get_attribute_access_mode(*it) == ACC_PROTECTED) {
                return true;
            }

            return false;
        }

        return false;
    } else if (this->type == TYP_INTERFACE) {
        if (context_type->has_interface(this_shared)) {
            if (type::get_attribute_access_mode(*it) == ACC_PROTECTED) {
                return true;
            }

            return false;
        }
    }

    return false;
}

type::string_t
internal_type::get_name()
{
    return name;
}

type::type_t
internal_type::get_parent()
{
    return parent;
}


internal_value::internal_value(type::type_t type):
    type(type)
{}

type::type_t
internal_value::get_type()
{
    return this->type;
}

bundle::bundle(const char *const name): name(name)
{}

bundle::bundle(type::string_t name): name(name)
{}

void
bundle::register_class(type::type_t ce)
{
    this->classes[ce->get_name()] = ce;
}

type::type_t
bundle::get_class(type::string_t name)
{
    return this->classes[name];
}

void
bundle::merge(type::string_t, type::bundle_t sub_bundle)
{
    this->classes.insert(sub_bundle->classes.begin(), sub_bundle->classes.end());
}

type::string_t
bundle::get_name()
{
    return this->name;
}

template<>
inline type::value_t
bundle::invoke<short>(type::string_t &name, short value)
{
    auto type = this->get_class(name);

    type::value_t item(new internal_value(type));
    long var = static_cast<long>(value);
    item->set<long>(var);

    return item;
}

template<>
inline type::value_t
bundle::invoke<int>(type::string_t &name, int value)
{
    auto type = this->get_class(name);

    type::value_t item(new internal_value(type));
    long var = static_cast<long>(value);
    item->set<long>(var);

    return item;
}

template<>
inline type::value_t
bundle::invoke<float>(type::string_t &name, float value)
{
    auto type = this->get_class(name);

    type::value_t item(new internal_value(type));
    double var = static_cast<double>(value);
    item->set<double>(var);

    return item;
}

std::map<type::operator_t,string> engine::operators_map = {
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