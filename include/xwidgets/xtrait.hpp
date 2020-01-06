#ifndef XWIDGETS_TRAIT
#define XWIDGETS_TRAIT

#include <iostream>
#include <string>
#include <functional>

#include "xmaterialize.hpp"

template <class T>
class trait
{
public:

    using value_type = T;

    trait(const std::string& name, const T& init) : 
        m_name(name),
        m_data(init)
    {
    }

    template <class IV, class X>
    trait(const std::string& name,
          IV&& init,
          X* parent
        ) :
        m_name(name),
        m_data(std::forward<IV>(init)),
        m_observer({[parent](const trait<T>& t){ return parent->notify(t); }})
    {
    }

    template <class IV, class V, class X>
    trait(const std::string& name,
          IV&& init,
          V&& validator,
          X* parent
        ) :
        m_name(name),
        m_data(std::forward<IV>(init)),
        m_validator({validator}),
        m_observer({[parent](const trait<T>& t){ return parent->notify(t); }})
    {
    }

    const std::string& name() const
    {
        return m_name;
    }

    T& ref() noexcept
    {
        return m_data;
    }

    const T& ref() const noexcept
    {
        return m_data;
    }

    operator T&() noexcept
    {
        return m_data;
    }

    operator const T&() const noexcept
    {
        return m_data;
    }

    const T& operator()() const noexcept
    {
        return m_data;
    }

    T& operator()() noexcept
    {
        return m_data;
    }

    void set(const T& val) noexcept
    {
        m_data = val;
    }

    template <class X>
    trait<T>& operator=(X&& new_data)
    {
        T proposal(std::forward<X>(new_data));
        if (m_validator.size())
        {
            for (auto& v : m_validator)
            {
                v(proposal);
            }
        }
        m_data = std::move(proposal);
        if (m_observer.size()) {
            for (auto& o : m_observer) {
                o(*this);
            }
        }
        return *this;
    }

    template <class X>
    void add_observer(X&& func)
    {
        m_observer.push_back(std::forward<X>(func));
    }

    template <class X>
    void add_observer(void(X::*func)(const trait<T>&), X* parent)
    {
        m_observer.push_back(std::bind(func, parent, std::placeholders::_1));
    }

    template <class X>
    void add_validator(X&& func)
    {
        m_validator.push_back(std::forward<X>(func));
    }

    template <class X>
    void add_validator(void(X::*func)(const trait<T>&), X* parent)
    {
        m_validator.push_back(std::bind(func, parent, std::placeholders::_1));
    }

private:

    std::string m_name;
    T m_data;

    std::vector<std::function<void(T&)>> m_validator;
    std::vector<std::function<void(const trait<T>&)>> m_observer;
};

template <class O, class T>
class super_trait : public trait<T>
{
public:

    template <class IV, class X>
    super_trait(const std::string& name,
          IV&& init,
          X* parent
        ) :
        trait<T>(name, std::forward<IV>(init), parent),
        m_owner(&(parent->derived_cast()))
    {
    }

    template <class IV, class V, class X>
    super_trait(const std::string& name,
          IV&& init,
          V&& validator,
          X* parent
    ) :
        trait<T>(name, std::forward<IV>(init), std::forward<V>(validator), parent),
        m_owner(&(parent->derived_cast()))
    {
    }

    template <class X>
    super_trait<O, T>& operator=(X&& new_data)
    {
        trait<T>::operator=(std::forward<X>(new_data));
        return *this;
    }

    O& operator()(const T& other)
    {
        trait<T>::set(other);
        return *m_owner;
    }

    T& operator()()
    {
        return this->ref();
    }

    const T& operator()() const
    {
        return this->ref();
    }

private:

    O* m_owner;
};

#define XTRAIT_NODEFAULT(T, D, N) \
    super_trait<D, T> N = super_trait<D, T>(#N, T(), this);

#define XTRAIT_DEFAULT(T, D, N, V) \
    super_trait<D, T> N = super_trait<D, T>(#N, T(V), this);

#define XTRAIT_GENERAL(T, D, N, V, Validator) \
    super_trait<D, T> N = super_trait<D, T>(#N, T(V), Validator, this);

#define XTRAIT_OVERLOAD(_1, _2, _3, _4, _5, NAME, ...) NAME

#ifdef _MSC_VER
// Workaround for MSVC not expanding macros
#define XTRAIT_EXPAND(x) x
#define XTRAIT(...) XTRAIT_EXPAND(XTRAIT_OVERLOAD(__VA_ARGS__, XTRAIT_GENERAL, XTRAIT_DEFAULT, XTRAIT_NODEFAULT)(__VA_ARGS__))
#else
#define XTRAIT(...) XTRAIT_OVERLOAD(__VA_ARGS__, XTRAIT_GENERAL, XTRAIT_DEFAULT, XTRAIT_NODEFAULT)(__VA_ARGS__)
#endif

#endif
