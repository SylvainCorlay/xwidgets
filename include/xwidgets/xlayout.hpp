/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_LAYOUT_HPP
#define XWIDGETS_LAYOUT_HPP

#include <string>

#include "xeither.hpp"
#include "xmaterialize.hpp"
#include "xobject.hpp"

namespace xw
{
    /**********************
     * layout declaration *
     **********************/

    template <class D>
    class xlayout : public xobject<D>
    {
    public:

        using base_type = xobject<D>;
        using derived_type = D;

        void serialize_state(xeus::xjson&, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);

        XTRAIT(xtl::xoptional<std::string>, derived_type, align_content, {}, XEITHER_OPTIONAL("flex-start", "flex-end", "center", "space-between", "space-around", "space-evenly", "stretch", "inherit", "inital", "unset"));
        XTRAIT(xtl::xoptional<std::string>, derived_type, align_items, {}, XEITHER_OPTIONAL("flex-start", "flex-end", "center", "baseline", "stretch", "inherit", "inital", "unset"));
        XTRAIT(xtl::xoptional<std::string>, derived_type, align_self, {}, XEITHER_OPTIONAL("auto", "flex-start", "flex-end", "center", "baseline", "stretch", "inherit", "inital", "unset"));

        XTRAIT(xtl::xoptional<std::string>, derived_type, bottom);
        XTRAIT(xtl::xoptional<std::string>, derived_type, border);
        XTRAIT(xtl::xoptional<std::string>, derived_type, display);
        XTRAIT(xtl::xoptional<std::string>, derived_type, flex);
        XTRAIT(xtl::xoptional<std::string>, derived_type, flex_flow);
        XTRAIT(xtl::xoptional<std::string>, derived_type, height);
        XTRAIT(xtl::xoptional<std::string>, derived_type, justify_content, {}, XEITHER_OPTIONAL("flex-start", "flex-end", "center", "space-between", "space-around", "inherit", "inital", "unset"));
        XTRAIT(xtl::xoptional<std::string>, derived_type, left);
        XTRAIT(xtl::xoptional<std::string>, derived_type, margin);
        XTRAIT(xtl::xoptional<std::string>, derived_type, max_height);
        XTRAIT(xtl::xoptional<std::string>, derived_type, max_width);
        XTRAIT(xtl::xoptional<std::string>, derived_type, min_height);
        XTRAIT(xtl::xoptional<std::string>, derived_type, min_width);
        XTRAIT(xtl::xoptional<std::string>, derived_type, overflow, {}, XEITHER_OPTIONAL("visible", "hidden", "scroll", "auto", "inherit", "inital", "unset"));
        XTRAIT(xtl::xoptional<std::string>, derived_type, overflow_x, {}, XEITHER_OPTIONAL("visible", "hidden", "scroll", "auto", "inherit", "inital", "unset"));
        XTRAIT(xtl::xoptional<std::string>, derived_type, overflow_y, {}, XEITHER_OPTIONAL("visible", "hidden", "scroll", "auto", "inherit", "inital", "unset"));
        XTRAIT(xtl::xoptional<std::string>, derived_type, order);
        XTRAIT(xtl::xoptional<std::string>, derived_type, padding);
        XTRAIT(xtl::xoptional<std::string>, derived_type, right);
        XTRAIT(xtl::xoptional<std::string>, derived_type, top);
        XTRAIT(xtl::xoptional<std::string>, derived_type, visibility);
        XTRAIT(xtl::xoptional<std::string>, derived_type, width);

    protected:

        xlayout();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using layout = xmaterialize<xlayout>;

    using layout_generator = xgenerator<xlayout>;

    /*************************
     * layout implementation *
     *************************/

    template <class D>
    inline void xlayout<D>::serialize_state(xeus::xjson& state, xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        set_patch_from_property(align_content, state, buffers);
        set_patch_from_property(align_items, state, buffers);
        set_patch_from_property(align_self, state, buffers);
        set_patch_from_property(bottom, state, buffers);
        set_patch_from_property(border, state, buffers);
        set_patch_from_property(display, state, buffers);
        set_patch_from_property(flex, state, buffers);
        set_patch_from_property(flex_flow, state, buffers);
        set_patch_from_property(height, state, buffers);
        set_patch_from_property(justify_content, state, buffers);
        set_patch_from_property(left, state, buffers);
        set_patch_from_property(margin, state, buffers);
        set_patch_from_property(max_height, state, buffers);
        set_patch_from_property(max_width, state, buffers);
        set_patch_from_property(min_height, state, buffers);
        set_patch_from_property(min_width, state, buffers);
        set_patch_from_property(overflow, state, buffers);
        set_patch_from_property(overflow_x, state, buffers);
        set_patch_from_property(overflow_y, state, buffers);
        set_patch_from_property(order, state, buffers);
        set_patch_from_property(padding, state, buffers);
        set_patch_from_property(right, state, buffers);
        set_patch_from_property(top, state, buffers);
        set_patch_from_property(width, state, buffers);
    }

    template <class D>
    inline void xlayout<D>::apply_patch(const xeus::xjson& patch, const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        set_property_from_patch(align_content, patch, buffers);
        set_property_from_patch(align_items, patch, buffers);
        set_property_from_patch(align_self, patch, buffers);
        set_property_from_patch(bottom, patch, buffers);
        set_property_from_patch(border, patch, buffers);
        set_property_from_patch(display, patch, buffers);
        set_property_from_patch(flex, patch, buffers);
        set_property_from_patch(flex_flow, patch, buffers);
        set_property_from_patch(height, patch, buffers);
        set_property_from_patch(justify_content, patch, buffers);
        set_property_from_patch(left, patch, buffers);
        set_property_from_patch(margin, patch, buffers);
        set_property_from_patch(max_height, patch, buffers);
        set_property_from_patch(max_width, patch, buffers);
        set_property_from_patch(overflow, patch, buffers);
        set_property_from_patch(overflow_x, patch, buffers);
        set_property_from_patch(overflow_y, patch, buffers);
        set_property_from_patch(order, patch, buffers);
        set_property_from_patch(padding, patch, buffers);
        set_property_from_patch(right, patch, buffers);
        set_property_from_patch(top, patch, buffers);
        set_property_from_patch(width, patch, buffers);
    }

    template <class D>
    inline xlayout<D>::xlayout()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xlayout<D>::set_defaults()
    {
        this->_model_module() = "@jupyter-widgets/base";
        this->_view_module() = "@jupyter-widgets/base";
        this->_model_name() = "LayoutModel";
        this->_view_name() = "LayoutView";
    }

    /*********************
     * precompiled types *
     *********************/

#ifndef _WIN32
    extern template class xmaterialize<xlayout>;
    extern template xmaterialize<xlayout>::xmaterialize();
    extern template class xtransport<xmaterialize<xlayout>>;
    extern template class xgenerator<xlayout>;
    extern template xgenerator<xlayout>::xgenerator();
    extern template class xtransport<xgenerator<xlayout>>;
#endif
}
#endif
