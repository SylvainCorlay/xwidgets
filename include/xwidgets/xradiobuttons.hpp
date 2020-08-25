/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_RADIOBUTTONS_HPP
#define XWIDGETS_RADIOBUTTONS_HPP

#include <utility>

#include "xmaterialize.hpp"
#include "xselection.hpp"

namespace xw
{
    /****************************
     * radiobuttons declaration *
     ****************************/

    template <class D>
    class xradiobuttons : public xselection<D>
    {
    public:

        using base_type = xselection<D>;
        using derived_type = D;
        using options_type = typename base_type::options_type;

    protected:

        xradiobuttons();

        template <class O, class T>
        xradiobuttons(O&& options, T&& value);

        using base_type::base_type;

    private:

        void set_defaults();
    };

    using radiobuttons = xmaterialize<xradiobuttons>;

    /********************************
     * xradiobuttons implementation *
     ********************************/

    template <class D>
    inline xradiobuttons<D>::xradiobuttons()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    template <class O, class T>
    inline xradiobuttons<D>::xradiobuttons(O&& options, T&& value)
        : base_type(std::forward<O>(options), std::forward<T>(value))
    {
        set_defaults();
    }

    template <class D>
    inline void xradiobuttons<D>::set_defaults()
    {
        this->_model_name() = "RadioButtonsModel";
        this->_view_name() = "RadioButtonsView";
        this->_model_module() = "@jupyter-widgets/controls";
        this->_view_module() = "@jupyter-widgets/controls";
        this->_model_module_version() = XWIDGETS_CONTROLS_VERSION;
        this->_view_module_version() = XWIDGETS_CONTROLS_VERSION;
    }

    /*********************
     * precompiled types *
     *********************/

    extern template class xmaterialize<xradiobuttons>;
    extern template class xtransport<xmaterialize<xradiobuttons>>;
}
#endif
