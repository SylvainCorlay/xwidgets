/***************************************************************************
* Copyright (c) 2018, Sylvain Corlay, Johan Mabille an                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XWIDGETS_MEDIA_HPP
#define XWIDGETS_MEDIA_HPP

#include <fstream>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>

#include "xmaterialize.hpp"
#include "xwidget.hpp"

namespace xw
{
    class media_buffer_type : private std::vector<char>
    {
    public:

        using base_type = std::vector<char>;

        media_buffer_type(const base_type& v): base_type(v) {}
        media_buffer_type(base_type&& v): base_type(std::move(v)) {}

        operator const base_type&() const & { return *this; }
        operator base_type&() & { return *this; }
        operator base_type&&() && { return std::move(*this); }
        operator const base_type&&() const && { return std::move(*this); }

        using base_type::base_type;

        using value_type = base_type::value_type;
        using allocator_type = base_type::allocator_type;
        using size_type = base_type::size_type;
        using difference_type = base_type::difference_type;
        using reference = base_type::reference;
        using const_reference = base_type::const_reference;
        using pointer = base_type::pointer;
        using const_pointer = base_type::const_pointer;
        using iterator = base_type::iterator;
        using const_iterator = base_type::const_iterator;
        using reverse_iterator = base_type::reverse_iterator;
        using const_reverse_iterator = base_type::const_reverse_iterator;
 
        using base_type::operator=;
        using base_type::assign;
        using base_type::get_allocator;
        using base_type::at;
        using base_type::operator[];
        using base_type::front;
        using base_type::back;
        using base_type::data;
        using base_type::begin;
        using base_type::cbegin;
        using base_type::end;
        using base_type::cend;
        using base_type::rbegin;
        using base_type::crbegin;
        using base_type::rend;
        using base_type::crend;
        using base_type::empty;
        using base_type::size;
        using base_type::max_size;
        using base_type::reserve;
        using base_type::capacity;
        using base_type::shrink_to_fit;
        using base_type::clear;
        using base_type::insert;
        using base_type::emplace;
        using base_type::erase;
        using base_type::push_back;
        using base_type::emplace_back;
        using base_type::pop_back;
        using base_type::resize;
        using base_type::swap;
    };

    /*********************
     * media declaration *
     *********************/

    template <class D>
    class xmedia : public xwidget<D>
    {
    public:

        using base_type = xwidget<D>;
        using derived_type = D;

        using value_type = media_buffer_type;

        void serialize_state(xeus::xjson& state, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);

        XPROPERTY(value_type, derived_type, value);

        const std::vector<xjson_path_type>& buffer_paths() const;

    protected:

        xmedia();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using media = xmaterialize<xmedia>;

    using media_generator = xgenerator<xmedia>;

    /*************************
     * xmedia implementation *
     *************************/

    template <class D>
    inline void xmedia<D>::serialize_state(xeus::xjson& state, xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        set_patch_from_property("value", value(), state, buffers);
    }

    template <class D>
    inline void xmedia<D>::apply_patch(const xeus::xjson& patch, const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        set_property_from_patch(value, patch, buffers);
    }

    template <class D>
    inline const std::vector<xjson_path_type>& xmedia<D>::buffer_paths() const
    {
        static const std::vector<xjson_path_type> default_buffer_paths = {{"value"}};
        return default_buffer_paths;
    }

    template <class D>
    inline xmedia<D>::xmedia()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xmedia<D>::set_defaults()
    {
        this->_model_module() = "@jupyter-widgets/controls";
        this->_view_module() = "@jupyter-widgets/controls";
        this->_model_module_version() = XWIDGETS_CONTROLS_VERSION;
        this->_view_module_version() = XWIDGETS_CONTROLS_VERSION;
    }

    inline std::vector<char> read_file(const std::string& filename)
    {
        const char* cstr = filename.c_str();
        std::basic_ifstream<char> file(cstr, std::ios::binary);
        return std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    /**********************
     * custom serializers *
     **********************/

    inline void xwidgets_serialize(const media_buffer_type& value, xeus::xjson& j, xeus::buffer_sequence& buffers)
    {
        j = xbuffer_reference_prefix() + std::to_string(buffers.size());
        buffers.emplace_back(value.data(), value.size());
    }

    inline void xwidgets_deserialize(media_buffer_type& value, const xeus::xjson& j, const xeus::buffer_sequence& buffers)
    {
        std::size_t index = buffer_index(j.template get<std::string>());
        const auto& value_buffer = buffers[index];
        const char* value_buf = value_buffer.data<const char>();
        value = media_buffer_type(value_buf, value_buf + value_buffer.size());
    }

    /*********************
     * precompiled types *
     *********************/

#ifndef _WIN32
    extern template class xmaterialize<xmedia>;
    extern template xmaterialize<xmedia>::xmaterialize();
    extern template class xtransport<xmaterialize<xmedia>>;
    extern template class xgenerator<xmedia>;
    extern template xgenerator<xmedia>::xgenerator();
    extern template class xtransport<xgenerator<xmedia>>;
#endif
}
#endif
