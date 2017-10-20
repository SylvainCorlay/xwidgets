/***************************************************************************
* Copyright (c) 2017, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XTENSOR_ANY_HPP
#define XTENSOR_ANY_HPP

#include <stdexcept>
#include <utility>

#include "xtl/xany.hpp"
#include "xtl/xclosure.hpp"

#include "xeus/xjson.hpp"

namespace xw
{
    namespace detail
    {
        template <template <class> class CRTP>
        class xany_impl;
    }

    /********************
     * xany declaration *
     ********************/

    template <template <class> class CRTP>
    class xany
    {
    public:

        using implementation_type = detail::xany_impl<CRTP>;

        xany();
        ~xany();
        xany(const xany& rhs);
        xany(xany&& rhs);
        template <class D>
        xany(const CRTP<D>& rhs);
        template <class D>
        xany(CRTP<D>&& rhs);
        xany(implementation_type* holder);
        xany& operator=(const xany& rhs);
        xany& operator=(xany&& rhs);
        template <class D>
        xany& operator=(const CRTP<D>& rhs);
        template <class D>
        xany& operator=(CRTP<D>&& rhs);
        void swap(xany& rhs);

        xtl::any value() &;
        const xtl::any value() const &;
        xtl::any value() &&;

        xeus::xjson to_json() const;
        void from_json(const xeus::xjson&);

        template <class D>
        D& get() &;

        template <class D>
        const D& get() const &;

        template <class D>
        D get() &&;

    private:

        void check_holder() const;

        implementation_type* p_holder;
    };

    /***********************
     * xany implementation *
     ***********************/

    namespace detail
    {
        template <template <class> class CRTP>
        class xany_impl
        {
        public:
      
            xany_impl() = default;
            xany_impl(xany_impl&&) = delete;
            xany_impl& operator=(const xany_impl&) = delete;
            xany_impl& operator=(xany_impl&&) = delete;
            virtual xany_impl* clone() const = 0;
            virtual ~xany_impl() = default;

            virtual xtl::any value() & = 0;
            virtual const xtl::any value() const & = 0;
            virtual xtl::any value() && = 0;

            virtual xeus::xjson to_json() const = 0;
            virtual void from_json(const xeus::xjson& j) = 0;
        
        protected:

            xany_impl(const xany_impl&) = default;
        };

        template <template <class> class CRTP, class D>
        class xany_owning : public xany_impl<CRTP>
        {
        public:
        
            using base_type = xany_impl<CRTP>;

            xany_owning(const CRTP<D>& value)
                : base_type(),
                  m_value(value.derived_cast())
            {
            }

            xany_owning(CRTP<D>&& value)
                : base_type(),
                  m_value(std::move(value.derived_cast()))
            {
            }

            virtual ~xany_owning()
            {
            }

            virtual base_type* clone() const override
            {
                return new xany_owning(*this);
            }

            virtual xtl::any value() & override
            {
                return xtl::closure(m_value);
            }

            virtual const xtl::any value() const & override
            {
                return xtl::closure(m_value);
            }

            virtual xtl::any value() && override
            {
                return xtl::closure(std::move(m_value));
            }

            virtual xeus::xjson to_json() const
            {
                return m_value; // if D is json serializable
            }

            virtual void from_json(const xeus::xjson& j)
            {
                m_value = j.get<D>(); // if D can be deserialized from json
            }

        private:

            xany_owning(const xany_owning&) = default;
            xany_owning(xany_owning&&) = default;
            xany_owning& operator=(const xany_owning&) = default;
            xany_owning& operator=(xany_owning&&) = default;

            D m_value;
        };
    }

    /*************************************
     * to_json and from_json declaration *
     *************************************/

    template <template <class> class CRTP>
    void to_json(xeus::xjson& j, const xany<CRTP>& o);

    template <template <class> class CRTP>
    void from_json(const xeus::xjson& j, xany<CRTP>& o);

    /***********************
     * xany implementation *
     ***********************/

    template <template <class> class CRTP>
    xany<CRTP>::xany()
        : p_holder(nullptr)
    {
    }

    template <template <class> class CRTP>
    xany<CRTP>::xany(detail::xany_impl<CRTP>* holder)
        : p_holder(holder)
    {
    }

    template <template <class> class CRTP>
    xany<CRTP>::~xany()
    {
        delete p_holder;
    }

    template <template <class> class CRTP>
    xany<CRTP>::xany(const xany& rhs)
        : p_holder(rhs.p_holder ? rhs.p_holder->clone() : nullptr)
    {
    }

    template <template <class> class CRTP>
    template <class D>
    xany<CRTP>::xany(const CRTP<D>& rhs)
        : xany(new detail::xany_owning<CRTP, D>(rhs))
    {
    }

    template <template <class> class CRTP>
    template <class D>
    xany<CRTP>::xany(CRTP<D>&& rhs)
        : xany(new detail::xany_owning<CRTP, D>(std::move(rhs)))
    {
    }

    template <template <class> class CRTP>
    xany<CRTP>::xany(xany&& rhs)
        : p_holder(rhs.p_holder)
    {
        rhs.p_holder = nullptr;
    }

    template <template <class> class CRTP>
    xany<CRTP>& xany<CRTP>::operator=(const xany& rhs)
    {
        using std::swap;
        xany tmp(rhs);
        swap(*this, tmp);
        return *this;
    }

    template <template <class> class CRTP>
    xany<CRTP>& xany<CRTP>::operator=(xany&& rhs)
    {
        using std::swap;
        xany tmp(std::move(rhs));
        swap(*this, tmp);
        return *this;
    }

    template <template <class> class CRTP>
    template <class D>
    xany<CRTP>& xany<CRTP>::operator=(const CRTP<D>& rhs)
    {
        using std::swap;
        xany<CRTP> tmp(rhs);
        swap(tmp, *this);
        return *this;
    }

    template <template <class> class CRTP>
    template <class D>
    xany<CRTP>& xany<CRTP>::operator=(CRTP<D>&& rhs)
    {
        using std::swap;
        xany<CRTP> tmp(rhs);
        swap(tmp, *this);
        return *this;
    }

    template <template <class> class CRTP>
    void xany<CRTP>::swap(xany& rhs)
    {
        std::swap(p_holder, rhs.p_holder);
    }

    template <template <class> class CRTP>
    xtl::any xany<CRTP>::value() &
    {
        check_holder();
        return p_holder->value();
    }

    template <template <class> class CRTP>
    const xtl::any xany<CRTP>::value() const &
    {
        check_holder();
        return p_holder->value();
    }

    template <template <class> class CRTP>
    inline xtl::any xany<CRTP>::value() &&
    {
        check_holder();
        return p_holder->value();
    }

    template <template <class> class CRTP>
    inline xeus::xjson xany<CRTP>::to_json() const
    {
        check_holder();
        return p_holder->to_json();
    }

    template <template <class> class CRTP>
    inline void xany<CRTP>::from_json(const xeus::xjson& j)
    {
        check_holder();
        p_holder->from_json(j);
    }

    template <template <class> class CRTP>
    template <class D>
    D& xany<CRTP>::get() &
    {
        return xtl::any_cast<xtl::closure_wrapper<D&>>(this->value()).get();
    }

    template <template <class> class CRTP>
    template <class D>
    const D& xany<CRTP>::get() const &
    {
        return xtl::any_cast<xtl::closure_wrapper<const D&>>(this->value()).get();
    }

    template <template <class> class CRTP>
    template <class D>
    D xany<CRTP>::get() &&
    {
        return xtl::any_cast<xtl::closure_wrapper<const D&>>(this->value()).get();
    }

    template <template <class> class CRTP>
    void xany<CRTP>::check_holder() const
    {
        if (p_holder == nullptr)
        {
            throw std::runtime_error("The holder does not contain an item");
        }
    }

    template <template <class> class CRTP>
    inline void swap(xany<CRTP>& lhs, xany<CRTP>& rhs)
    {
        lhs.swap(rhs);
    }

    /****************************************
     * to_json and from_json implementation *
     ****************************************/

    template <template <class> class CRTP>
    inline void to_json(xeus::xjson& j, const xany<CRTP>& o)
    {
        j = o.to_json();
    }

    template <template <class> class CRTP>
    void from_json(const xeus::xjson& j, xany<CRTP>& o)
    {
        o.from_json(j);
    }
}
#endif
