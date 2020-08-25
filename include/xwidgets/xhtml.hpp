/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_HTML_HPP
#define XWIDGETS_HTML_HPP

#include "xmaterialize.hpp"
#include "xstring.hpp"

namespace xw
{
    /********************
     * html declaration *
     ********************/

    template <class D>
    class xhtml : public xstring<D>
    {
    public:

        using base_type = xstring<D>;
        using derived_type = D;

    protected:

        xhtml();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using html = xmaterialize<xhtml>;

    /************************
     * xhtml implementation *
     ************************/

    template <class D>
    inline xhtml<D>::xhtml()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xhtml<D>::set_defaults()
    {
        this->_model_name() = "HTMLModel";
        this->_view_name() = "HTMLView";
    }

    /*********************
     * precompiled types *
     *********************/

    extern template class xmaterialize<xhtml>;
    extern template class xtransport<xmaterialize<xhtml>>;
}

#endif
