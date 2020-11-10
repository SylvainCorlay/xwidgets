/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_TEXT_HPP
#define XWIDGETS_TEXT_HPP

#include "xmaterialize.hpp"
#include "xstring.hpp"

namespace xw
{
    /********************
     * text declaration *
     ********************/

    template <class D>
    class xtext : public xstring<D>
    {
    public:

        using base_type = xstring<D>;
        using derived_type = D;

        using submit_callback_type = std::function<void()>;

        void serialize_state(nl::json&, xeus::buffer_sequence&) const;
        void apply_patch(const nl::json&, const xeus::buffer_sequence&);

        void on_submit(submit_callback_type);

        XPROPERTY(bool, derived_type, disabled);
        XPROPERTY(bool, derived_type, continuous_update, true);

        void handle_custom_message(const nl::json&);

    protected:

        xtext();
        using base_type::base_type;

    private:

        void set_defaults();

        std::list<submit_callback_type> m_submit_callbacks;
    };

    using text = xmaterialize<xtext>;

    /************************
     * xtext implementation *
     ************************/

    template <class D>
    inline void xtext<D>::serialize_state(nl::json& state, xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        xwidgets_serialize(disabled(), state["disabled"], buffers);
        xwidgets_serialize(continuous_update(), state["continuous_update"], buffers);
    }

    template <class D>
    inline void xtext<D>::apply_patch(const nl::json& patch, const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        set_property_from_patch(disabled, patch, buffers);
        set_property_from_patch(continuous_update, patch, buffers);
    }

    template <class D>
    inline void xtext<D>::on_submit(submit_callback_type cb)
    {
        m_submit_callbacks.emplace_back(std::move(cb));
    }

    template <class D>
    inline xtext<D>::xtext()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xtext<D>::set_defaults()
    {
        this->_model_name() = "TextModel";
        this->_view_name() = "TextView";
    }

    template <class D>
    inline void xtext<D>::handle_custom_message(const nl::json& content)
    {
        auto it = content.find("event");
        if (it != content.end() && it.value() == "submit")
        {
            for (auto it = m_submit_callbacks.begin(); it != m_submit_callbacks.end(); ++it)
            {
                it->operator()();
            }
        }
    }

    /*********************
     * precompiled types *
     *********************/

    extern template class xmaterialize<xtext>;
    extern template class xtransport<xmaterialize<xtext>>;
    extern template nl::json mime_bundle_repr<xtext>(const xmaterialize<xtext>&);
}

#endif
