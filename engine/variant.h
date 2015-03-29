/**
 * Copyright (C) 2013 Jarryd Beck
 * Copyright (C) 2015 Grégory Planchat
 *
 * https://github.com/jarro2783/thenewcpp/tree/master/juice
 */

#ifndef REPHP_ENGINE_VARIANT_H
#define REPHP_ENGINE_VARIANT_H

#include <type_traits>
#include <cstdlib>
#include <cassert>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

namespace rephp { namespace engine { namespace type {

namespace helper {

template<template<typename> class Size, typename Current, typename... Args >
class max_helper;

template<template<typename> class Size, typename Current>
class max_helper<Size, Current>
{
public:
    static constexpr decltype(Size<Current>::value) value = Size<Current>::value;
    typedef Current type;
};

template<template<typename> class Size, typename Current, typename Next, typename... Args>
class max_helper<Size, Current, Next, Args...>
{
private:
    typedef typename std::conditional<(Size<Next>::value > Size<Current>::value), max_helper<Size, Next, Args...>, max_helper<Size, Current, Args...>>::type m_next;

public:
    static constexpr decltype(Size<Current>::value) value = m_next::value;

    typedef typename m_next::type type;
};

}; // namespace helper

template<template<typename> class Size, typename... Args>
class max;

template<template<typename> class Size, typename First, typename... Args>
class max<Size, First, Args...>
{
private:
    typedef decltype(Size<First>::value) size_type;

    typedef helper::max_helper<Size, First, Args...> helper;

public:
    static constexpr size_type value = helper::value;
    typedef typename helper::type type;
};

namespace mpl
{
    class true_ {};
    class false_ {};
}

template<typename Result = void>
class static_visitor
{
public:
    typedef Result result_type;

    //so that it can't be instantiated
protected:
    ~static_visitor() = default;
};

template<typename Type>
class recursive_wrapper
{
private:
    Type* payload;

    template<typename Payload>
    void
    assign(Payload&& value)
    {
        *payload = std::forward<Payload>(value);
    }

public:
    ~recursive_wrapper()
    {
        delete payload;
    }

    template<typename Payload, typename Dummy = typename std::enable_if<std::is_convertible<Payload, Type>::value, Payload>::type>
    recursive_wrapper(const Payload& value): payload(new Type(value))
    {}

    template<typename Payload, typename Dummy = typename std::enable_if<std::is_convertible<Payload, Type>::value, Payload>::type>
    recursive_wrapper(Payload&& value): payload(new Type(std::forward<Payload>(value)))
    {}

    recursive_wrapper(const recursive_wrapper& rhs): payload(new Type(rhs.get()))
    {}

    recursive_wrapper(recursive_wrapper&& rhs): payload(rhs.payload)
    {
        rhs.payload = nullptr;
    }

    recursive_wrapper&
    operator= (const recursive_wrapper& rhs)
    {
        assign(rhs.get());
        return *this;
    }

    recursive_wrapper&
    operator= (recursive_wrapper&& rhs)
    {
        delete payload;
        payload = rhs.payload;
        rhs.payload = nullptr;
        return *this;
    }

    recursive_wrapper&
    operator= (const Type& value)
    {
        assign(value);
        return *this;
    }

    recursive_wrapper&
    operator= (Type&& value)
    {
        assign(std::move(value));
        return *this;
    }

    bool
    operator== (const recursive_wrapper& rhs) const
    {
        return *payload == *rhs.payload;
    }

    bool
    operator!= (const recursive_wrapper& rhs) const
    {
        return *payload != *rhs.payload;
    }

    Type& get()
    {
        return *payload;
    }

    const Type& get() const
    {
        return *payload;
    }
};

namespace detail
{
    template<typename Type, typename Internal>
    Type&
    get_value(Type& value, const Internal&)
    {
        return value;
    }

    template<typename Type>
    Type&
    get_value(recursive_wrapper<Type>& value, const mpl::false_&)
    {
        return value.get();
    }

    template<typename Type>
    const Type&
    get_value(const recursive_wrapper<Type>& value, const mpl::false_&)
    {
        return value.get();
    }

    template<typename Visitor, typename Visitable>
    class binary_visitor
    {
    private:
      Visitor& visitor;
      Visitable& visitable;

    public:
        typedef typename std::remove_reference<Visitor>::type::result_type result_type;

        binary_visitor(Visitor&& visitor, Visitable&& visitable): visitor(visitor), visitable(visitable)
        {}

        template<typename Type>
        result_type
        operator()(const Type& value)
        {
            return apply_visitor(visitor, visitable, value);
        }
    };

}; // namespace detail

template<typename Internal, typename Type, typename Storage, typename Visitor, typename... Args>
typename std::remove_reference<Visitor>::type::result_type
visitor_caller(Internal&& internal, Storage&& storage, Visitor&& visitor, Args&&... args)
{
    typedef typename std::conditional<
        std::is_const<typename std::remove_pointer<typename std::remove_reference<Storage>::type>::type>::value,
        const Type, Type
    >::type ConstType;

    return visitor(detail::get_value(*reinterpret_cast<ConstType*>(storage), internal), std::forward<Args>(args)...);
}

template<typename First, typename... Types>
class infinite_variant
{
private:
    template<typename... AllTypes>
    class do_visit
    {
    public:
        template<typename Internal, typename VoidPtrCV, typename Visitor, typename... Args>
        typename std::remove_reference<Visitor>::type::result_type
        operator() (Internal&& internal, size_t which, VoidPtrCV&& storage, Visitor&& visitor, Args&&... args)
        {
            typedef typename std::remove_reference<Visitor>::type::result_type(*which_caller)(Internal&&, VoidPtrCV&&, Visitor&&, Args&&...);

            static which_caller callers[sizeof...(AllTypes)] = {
                &visitor_caller<Internal&&, AllTypes, VoidPtrCV&&, Visitor, Args&&...>...
            };

            assert(which >= 0 && which < sizeof...(AllTypes));

            return (*callers[which])(
                std::forward<Internal>(internal),
                std::forward<VoidPtrCV>(storage),
                std::forward<Visitor>(visitor),
                std::forward<Args>(args)...
            );
        }
    };

    template<typename Type>
    class size_of
    {
    public:
        static constexpr size_t value = sizeof(Type);
    };

    template<typename Type>
    class align_of
    {
    public:
        static constexpr size_t value = alignof(Type);
    };

    //size = max of size of each thing
    static constexpr size_t m_size = max<size_of, First, Types...>::value;

    class constructor
    {
    private:
        infinite_variant& m_self;

    public:
        typedef void result_type;

        constructor(infinite_variant& self): m_self(self)
        {}

        template<typename Type>
        void
        operator() (const Type& rhs) const
        {
            m_self.construct(rhs);
        }
    };

    class move_constructor
    {
    private:
        infinite_variant& m_self;

    public:
        typedef void result_type;

        move_constructor(infinite_variant& self): m_self(self)
        {}

        template<typename Type>
        void
        operator() (Type& rhs) const
        {
            m_self.construct(std::move(rhs));
        }

    };

    class assigner
    {
    private:
        infinite_variant& m_self;
        int m_rhs_which;

    public:
        typedef void result_type;

        assigner(infinite_variant& self, int rhs_which): m_self(self), m_rhs_which(rhs_which)
        {}

        template<typename Rhs>
        void
        operator()(const Rhs& rhs) const
        {
            if (m_self.which() == m_rhs_which) {
                //the types are the same, so just assign into the lhs
                *reinterpret_cast<Rhs*>(m_self.address()) = rhs;
            } else {
              Rhs tmp(rhs);
              m_self.destroy(); //nothrow
              m_self.construct(std::move(tmp)); //nothrow (please)
            }
        }
    };

    class move_assigner
    {
    private:
        infinite_variant& m_self;
        int m_rhs_which;

    public:
        typedef void result_type;

        move_assigner(infinite_variant& self, int rhs_which): m_self(self), m_rhs_which(rhs_which)
        {}

        template<typename Rhs>
        void
        operator() (Rhs& rhs) const
        {
            typedef typename std::remove_const<Rhs>::type rhs_no_const;
            if (m_self.which() == m_rhs_which) {
                //the types are the same, so just assign into the lhs
                *reinterpret_cast<rhs_no_const*>(m_self.address()) = std::move(rhs);
            } else {
                m_self.destroy(); //nothrow
                m_self.construct(std::move(rhs)); //nothrow (please)
            }
        }
    };

    class equality
    {
    private:
        const infinite_variant& m_self;

    public:
        typedef bool result_type;

        equality(const infinite_variant& self): m_self(self)
        {}

        template<typename Rhs>
        bool
        operator()(Rhs& rhs) const
        {
            return *reinterpret_cast<Rhs*>(m_self.address()) == rhs;
        }
    };

    class destroyer
    {
    public:
        typedef void result_type;

        template<typename Type>
        void
        operator()(Type& value) const
        {
            value.~Type();
        }
    };

    template<size_t Which, typename... MyTypes>
    class initialiser;

    template<size_t Which, typename Current, typename... MyTypes>
    class initialiser<Which, Current, MyTypes...>: public initialiser<Which+1, MyTypes...>
    {
    public:
        typedef initialiser<Which+1, MyTypes...> base;
        using base::initialise;

        static void
        initialise(infinite_variant& visitor, Current&& current)
        {
            visitor.construct(std::move(current));
            visitor.indicate_which(Which);
        }

        static void
        initialise(infinite_variant& visitor, const Current& current)
        {
            visitor.construct(current);
            visitor.indicate_which(Which);
        }
    };

    template<size_t Which>
    class initialiser<Which>
    {
    public:
        //this should never match
        void initialise();
    };

public:
    infinite_variant()
    {
        //try to construct First
        //if this fails then First is not default constructible
        construct(First());
        indicate_which(0);
    }

    ~infinite_variant()
    {
        destroy();
    }

    //enable_if disables this function if we are constructing with a infinite_variant.
    //Unfortunately, this becomes infinite_variant(infinite_variant&) which is a better match
    //than infinite_variant(const infinite_variant& rhs), so it is chosen. Therefore, we disable
    //it.
    template<typename Type, typename Dummy = typename std::enable_if<!std::is_same<
        typename std::remove_reference<infinite_variant<First, Types...>>::type,
        typename std::remove_reference<Type>::type
    >::value, Type>::type>
    infinite_variant(Type&& value)
    {
        static_assert(
            !std::is_same<infinite_variant<First, Typeypes...>&, Type>::value,
            "why is infinite_variant(Type&&) instantiated with a infinite_variant?"
        );

        //compile error here means that Type is not unambiguously convertible to
        //any of the types in (First, Types...)
        initialiser<0, First, Types...>::initialise(*this, std::forward<Type>(value));
    }

    infinite_variant(const infinite_variant& rhs)
    {
        rhs.apply_visitor_internal(constructor(*this));
        indicate_which(rhs.which());
    }

    infinite_variant(infinite_variant&& rhs)
    {
        rhs.apply_visitor_internal(move_constructor(*this));
        indicate_which(rhs.which());
    }

    infinite_variant& operator= (const infinite_variant& rhs)
    {
        if (this != &rhs) {
            rhs.apply_visitor_internal(assigner(*this, rhs.which()));
            indicate_which(rhs.which());
        }

        return *this;
    }

    infinite_variant& operator= (infinite_variant&& rhs)
    {
        if (this != &rhs) {
            rhs.apply_visitor_internal(move_assigner(*this, rhs.which()));
            indicate_which(rhs.which());
        }

        return *this;
    }

    bool
    operator== (const infinite_variant& rhs) const
    {
        if (which() != rhs.which()) {
            return false;
        }

        return rhs.apply_visitor_internal(equality(*this));
    }

    int which() const
    {
        return m_which;
    }

    template<typename Internal, typename Visitor, typename... Args>
    typename std::remove_reference<Visitor>::type::result_type
    apply_visitor(Visitor&& visitor, Args&&... args)
    {
        return do_visit<First, Types...>()(Internal(), m_which, &m_storage,
            std::forward<Visitor>(visitor), std::forward<Args>(args)...);
    }

    template<typename Internal, typename Visitor, typename... Args>
    typename std::remove_reference<Visitor>::type::result_type
    apply_visitor(Visitor&& visitor, Args&&... args) const
    {
        return do_visit<First, Types...>()(Internal(), m_which, &m_storage,
            std::forward<Visitor>(visitor), std::forward<Args>(args)...);
    }

private:
    typename std::aligned_storage<m_size, max<align_of, First, Types...>::value>::type m_storage;

    int m_which;

    static std::function<void(void*)> m_handlers[1 + sizeof...(Types)];

    void indicate_which(int which)
    {
        m_which = which;
    }

    void* address()
    {
        return &m_storage;
    }

    const void* address() const
    {
        return &m_storage;
    }

    template<typename Visitor>
    typename Visitor::result_type
    apply_visitor_internal(Visitor&& visitor)
    {
        return apply_visitor<mpl::true_, Visitor>(std::forward<Visitor>(visitor));
    }

    template<typename Visitor>
    typename Visitor::result_type
    apply_visitor_internal(Visitor&& visitor) const
    {
        return apply_visitor<mpl::true_, Visitor>(std::forward<Visitor>(visitor));
    }

    void
    destroy()
    {
        apply_visitor_internal(destroyer());
    }

    template<typename Type>
    void
    construct(Type&& value)
    {
        typedef typename std::remove_reference<Type>::type type;
        new(&m_storage) type(std::forward<Type>(value));
    }
}; // class infinite_variant

class bad_get: public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "bad_get";
    }
};

template<typename Type>
class get_visitor
{
public:
    typedef Type* result_type;

    result_type
    operator()(Type& val) const
    {
        return &val;
    }

    template<typename Other>
    result_type
    operator()(const Other&) const
    {
        return nullptr;
    }
};

template<typename Visitor, typename Visitable, typename... Args>
typename std::remove_reference<Visitor>::type::result_type
apply_visitor(Visitor&& visitor, Visitable& visitable, Args&&... args)
{
    return visitable.template apply_visitor<mpl::false_>
        (std::forward<Visitor>(visitor), std::forward<Args>(args)...);
}

template<typename Type, typename First, typename... Types>
Type*
get(infinite_variant<First, Types...>* var)
{
    return apply_visitor(get_visitor<Type>(), *var);
}

template<typename Type, typename First, typename... Types>
const Type*
get(const infinite_variant<First, Types...>* var)
{
    return apply_visitor(get_visitor<const Type>(), *var);
}

template<typename Type, typename First, typename... Types>
Type&
get (infinite_variant<First, Types...>& var)
{
    Type* value = apply_visitor(get_visitor<Type>(), var);
    if (value == nullptr){throw bad_get();}

    return *value;
}

template<typename Type, typename First, typename... Types>
const Type&
get (const infinite_variant<First, Types...>& var)
{
    const Type* value = apply_visitor(get_visitor<const Type>(), var);
    if (value == nullptr) {throw bad_get();}

    return *value;
}

class visitor_applier
{
public:
    template<typename Visitor, typename Visitable, typename... Args>
    auto
    operator()(Visitor&& visitor, Visitable&& visitable, Args&&... args)
    -> decltype(
        apply_visitor(
            std::forward<Visitor>(visitor),
            std::forward<Visitable>(visitable),
            std::forward<Args>(args)...
        )
    )
    {
        return apply_visitor(
            std::forward<Visitor>(visitor),
            std::forward<Visitable>(visitable),
            std::forward<Args>(args)...
        );
    }
};

template<typename Type, typename Visitor>
bool
variant_is_type(const Visitor& visitor)
{
    return get<Type>(&visitor) != nullptr;
}

template<typename Visitor, typename Visitable1, typename Visitable2>
typename std::remove_reference<Visitor>::type::result_type
apply_visitor_binary(Visitor&& visitor, Visitable1&& v1, Visitable2&& v2)
{
    detail::binary_visitor<Visitor, Visitable1> visitor{
        std::forward<Visitor>(visitor),
        std::forward<Visitable1>(v1)
    };

    return apply_visitor(visitor, std::forward<Visitable2>(v2));
}

}; }; };

#endif