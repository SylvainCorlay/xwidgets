/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_TAB_HPP
#define XWIDGETS_TAB_HPP

#include "xmaterialize.hpp"
#include "xselection_container.hpp"

namespace xw
{
    /*************************
     * accordion declaration *
     *************************/

    template <class D>
    class xtab : public xselection_container<D>
    {
    public:

        using base_type = xselection_container<D>;
        using derived_type = D;

    protected:

        xtab();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using tab = xmaterialize<xtab>;

    /***********************
     * xtab implementation *
     ***********************/

    template <class D>
    inline xtab<D>::xtab()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xtab<D>::set_defaults()
    {
        this->_model_name() = "TabModel";
        this->_view_name() = "TabView";
    }

    /*********************
     * precompiled types *
     *********************/

    extern template class xmaterialize<xtab>;
    extern template class xtransport<xmaterialize<xtab>>;
}

#endif
