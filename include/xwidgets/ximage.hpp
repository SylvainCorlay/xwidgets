/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_IMAGE_HPP
#define XWIDGETS_IMAGE_HPP

#include <string>

#include "xmaterialize.hpp"
#include "xwidget.hpp"

std::string base64encode(const std::string &in)
{
    std::string out;

    int val=0, valb=-6;
    for (unsigned char c : in)
    {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0)
        {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb-=6;
        }
    }
    if (valb>-6)
    {
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    }
    while (out.size()%4) out.push_back('=');
    return out;
}

std::string base64decode(const std::string &in)
{
    std::string out;
    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++)
    {
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }

    int val=0, valb=-8;
    for (unsigned char c : in)
    {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0)
        {
            out.push_back(char((val>>valb)&0xFF));
            valb-=8;
        }
    }
    return out;
}

namespace xw
{

    class bstring
    {
    public:

        inline bstring() : m_value() {}
        inline bstring(const std::string& value) : m_value(value) {}
        inline bstring(std::string&& value) : m_value(std::move(value)) {}

        inline operator std::string() const noexcept
        {
            return m_value;
        }

    private:

        std::string m_value;
    };

    inline void to_json(xeus::xjson& j, const bstring& o)
    {
        j = base64encode(o);
    }

    inline void from_json(const xeus::xjson& j, bstring& o)
    {
        o = base64decode(j.get<std::string>());
    }

    /*********************
     * image declaration *
     *********************/

    template <class D>
    class ximage : public xwidget<D>
    {
    public:

        using base_type = xwidget<D>;
        using derived_type = D;

        ximage();
        xeus::xjson get_state() const;
        void apply_patch(const xeus::xjson& patch);

        XPROPERTY(std::string, derived_type, format, "png");
        XPROPERTY(std::string, derived_type, width);
        XPROPERTY(std::string, derived_type, height);
        XPROPERTY(bstring, derived_type, value);

    private:

        void set_defaults();
    };

    using image = xmaterialize<ximage>;

    /*************************
     * ximage implementation *
     *************************/

    template <class D>
    inline ximage<D>::ximage()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline xeus::xjson ximage<D>::get_state() const
    {
        xeus::xjson state = base_type::get_state();

        XOBJECT_SET_PATCH_FROM_PROPERTY(format, state);
        XOBJECT_SET_PATCH_FROM_PROPERTY(width, state);
        XOBJECT_SET_PATCH_FROM_PROPERTY(height, state);
        XOBJECT_SET_PATCH_FROM_PROPERTY(value, state);

        return state;
    }

    template <class D>
    inline void ximage<D>::apply_patch(const xeus::xjson& patch)
    {
        base_type::apply_patch(patch);

        XOBJECT_SET_PROPERTY_FROM_PATCH(format, patch)
        XOBJECT_SET_PROPERTY_FROM_PATCH(width, patch)
        XOBJECT_SET_PROPERTY_FROM_PATCH(height, patch)
        XOBJECT_SET_PROPERTY_FROM_PATCH(value, patch)
    }

    template <class D>
    inline void ximage<D>::set_defaults()
    {
        this->_model_module() = "@jupyter-widgets/controls";
        this->_view_module() = "@jupyter-widgets/controls";
        this->_model_name() = "ImageModel";
        this->_view_name() = "ImageView";
    }
}

#endif
