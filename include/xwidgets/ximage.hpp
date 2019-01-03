/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_IMAGE_HPP
#define XWIDGETS_IMAGE_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "xmaterialize.hpp"
#include "xmedia.hpp"

namespace xw
{
    /*********************
     * image declaration *
     *********************/

    template <class D>
    class ximage : public xmedia<D>
    {
    public:

        using base_type = xmedia<D>;
        using derived_type = D;

        void serialize_state(xeus::xjson& state, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);

        XPROPERTY(std::string, derived_type, format, "png");
        XPROPERTY(std::string, derived_type, width, "");
        XPROPERTY(std::string, derived_type, height, "");

    protected:

        ximage();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using image = xmaterialize<ximage>;

    using image_generator = xgenerator<ximage>;

    /*************************
     * ximage implementation *
     *************************/

    template <class D>
    inline void ximage<D>::serialize_state(xeus::xjson& state, xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        set_patch_from_property("format", format(), state, buffers);
        set_patch_from_property("width", width(), state, buffers);
        set_patch_from_property("height", height(), state, buffers);
    }

    template <class D>
    inline void ximage<D>::apply_patch(const xeus::xjson& patch, const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        set_property_from_patch(format, patch, buffers);
        set_property_from_patch(width, patch, buffers);
        set_property_from_patch(height, patch, buffers);
    }

    template <class D>
    inline ximage<D>::ximage()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void ximage<D>::set_defaults()
    {
        this->_model_name() = "ImageModel";
        this->_view_name() = "ImageView";
    }

    /**************************
     * image from file or url *
     **************************/

    inline image_generator image_from_file(const std::string& filename)
    {
        return image_generator().value(read_file(filename));
    }

    inline image_generator image_from_url(const std::string& url)
    {
        std::vector<char> value(url.cbegin(), url.cend());
        return image_generator().value(value).format("url");
    }

    /*********************
     * precompiled types *
     *********************/

#ifndef _WIN32
    extern template class xmaterialize<ximage>;
    extern template xmaterialize<ximage>::xmaterialize();
    extern template class xtransport<xmaterialize<ximage>>;
    extern template class xgenerator<ximage>;
    extern template xgenerator<ximage>::xgenerator();
    extern template class xtransport<xgenerator<ximage>>;
#endif
}
#endif
