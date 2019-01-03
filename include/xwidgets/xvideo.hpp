/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_VIDEO_HPP
#define XWIDGETS_VIDEO_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "xmaterialize.hpp"
#include "xmedia.hpp"

namespace xw
{
    /*********************
     * video declaration *
     *********************/

    template <class D>
    class xvideo : public xmedia<D>
    {
    public:

        using base_type = xmedia<D>;
        using derived_type = D;

        void serialize_state(xeus::xjson& state, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);

        XPROPERTY(std::string, derived_type, format, "mp4");
        XPROPERTY(std::string, derived_type, width, "");
        XPROPERTY(std::string, derived_type, height, "");
        XPROPERTY(bool, derived_type, autoplay, true);
        XPROPERTY(bool, derived_type, loop, true);
        XPROPERTY(bool, derived_type, controls, true);

    protected:

        xvideo();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using video = xmaterialize<xvideo>;

    using video_generator = xgenerator<xvideo>;

    /*************************
     * xvideo implementation *
     *************************/

    template <class D>
    inline void xvideo<D>::serialize_state(xeus::xjson& state, xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        set_patch_from_property("format", format(), state, buffers);
        set_patch_from_property("width", width(), state, buffers);
        set_patch_from_property("height", height(), state, buffers);
        set_patch_from_property("autoplay", autoplay(), state, buffers);
        set_patch_from_property("loop", loop(), state, buffers);
        set_patch_from_property("controls", controls(), state, buffers);
    }

    template <class D>
    inline void xvideo<D>::apply_patch(const xeus::xjson& patch, const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        set_property_from_patch(format, patch, buffers);
        set_property_from_patch(width, patch, buffers);
        set_property_from_patch(height, patch, buffers);
        set_property_from_patch(autoplay, patch, buffers);
        set_property_from_patch(loop, patch, buffers);
        set_property_from_patch(controls, patch, buffers);
    }

    template <class D>
    inline xvideo<D>::xvideo()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xvideo<D>::set_defaults()
    {
        this->_model_name() = "VideoModel";
        this->_view_name() = "VideoView";
    }

    /**************************
     * video from file or url *
     **************************/

    inline video_generator video_from_file(const std::string& filename)
    {
        return video_generator().value(read_file(filename));
    }

    inline video_generator video_from_url(const std::string& url)
    {
        std::vector<char> value(url.cbegin(), url.cend());
        return video_generator().value(value).format("url");
    }

    /*********************
     * precompiled types *
     *********************/

#ifndef _WIN32
    extern template class xmaterialize<xvideo>;
    extern template xmaterialize<xvideo>::xmaterialize();
    extern template class xtransport<xmaterialize<xvideo>>;
    extern template class xgenerator<xvideo>;
    extern template xgenerator<xvideo>::xgenerator();
    extern template class xtransport<xgenerator<xvideo>>;
#endif
}
#endif
