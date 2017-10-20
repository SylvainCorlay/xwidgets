/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include "xwidgets/xany.hpp"

namespace xw
{
    template <class D>
    class xbase
    {
    public:

        using derived_type = D;

        derived_type& derived_cast() & noexcept
        {
            return *static_cast<derived_type*>(this);
        }

        const derived_type& derived_cast() const & noexcept
        {
            return *static_cast<const derived_type*>(this);
        }

        derived_type derived_cast() && noexcept
        {
            return *static_cast<derived_type*>(this);
        }

    protected:

        xbase() = default;
        ~xbase() = default;

        xbase(const xbase&) = default;
        xbase& operator=(const xbase&) = default;

        xbase(xbase&&) = default;
        xbase& operator=(xbase&&) = default;
    };

    class xderived : public xbase<xderived>
    {
    public:

        using base_type = xbase<xderived>;

        inline xderived() : m_value() {}
        inline xderived(double v) : m_value(v) {}
        inline double value() const { return m_value; }

        double m_value;
    };

    inline void from_json(const xeus::xjson& j, xderived& o)
    {
        o.m_value = j;
    }

    inline void to_json(xeus::xjson& j, const xderived&o)
    {
        j = o.m_value;
    }

    class xother : public xbase<xother>
    {
    public:

        using base_type = xbase<xother>;

        inline xother() : m_value() {}
        inline xother(int v) : m_value(v) {}
        inline int value() const { return m_value; }

        int m_value;
    };

    inline void from_json(const xeus::xjson& j, xother& o)
    {
        o.m_value = j;
    }

    inline void to_json(xeus::xjson& j, const xother& o)
    {
        j = o.m_value;
    }

    TEST(xany, assignment)
    {
	xderived d(1.0);
        xother   o(5);

        xany<xbase> holder = d;

        auto& l = holder.get<xderived>();
        ASSERT_EQ(l.value(), 1.0);

        holder = o;
        auto& h = holder.get<xother>();
        ASSERT_EQ(h.value(), 5);
    }
}
