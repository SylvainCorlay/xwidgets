/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_STYLE_HPP
#define XWIDGETS_STYLE_HPP

#include "xobject.hpp"

namespace xw
{
    /***************************
     * base xstyle declaration *
     ***************************/

    template <class D>
    class xstyle : public xobject<D>
    {
    public:

        using base_type = xobject<D>;
        using derived_type = D;

    protected:

        xstyle();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    /******************************
     * base xstyle implementation *
     ******************************/

    template <class D>
    inline xstyle<D>::xstyle()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xstyle<D>::set_defaults()
    {
        this->_model_module() = "@jupyter-widgets/base";
        this->_view_module() = "@jupyter-widgets/base";
        this->_model_name() = "StyleModel";
        this->_view_name() = "StyleView";
    }
}
#endif
