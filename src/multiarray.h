/*
 * This file is part of klibcpp.
 * multiarray.h - An multi dimension array.
 * 
 * License: Read the Readme.md here
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2016 kedixa(kedixa@outlook.com)
 *
 */
#ifndef MULTI_ARRAY_H
#define MULTI_ARRAY_H

#include <cstddef>
#include <new>
#include <memory>

#if __cplusplus < 201103L
#error kedixa::multiarray requires ISO C++ 2011 standard.
#else

namespace kedixa {

// Forward declaration for multiarray.
template<typename, unsigned, typename>
class multiarray;

// Help multiarray derive subarray type.
template<typename T, unsigned N, typename Alloc>
struct _subarray_type
{
    using value_type = multiarray<T, N-1, Alloc>;
};
template<typename T, typename Alloc>
struct _subarray_type<T, 1, Alloc>
{
    using value_type = T;
};
template<typename T, typename Alloc>
struct _subarray_type<T, 0, Alloc>
{ };

template<typename T, unsigned N, typename Alloc = std::allocator<T> >
class multiarray
{
public:
    using size_type = size_t;
    using value_type = typename _subarray_type<T, N, Alloc>::value_type;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using allocator_type = typename Alloc::template rebind<value_type>::other;
private:
    struct _multiarray_data : allocator_type
    {
        pointer value;
        size_type array_size;
    };

    _multiarray_data _data;
public:
    template<typename... Args>
    multiarray(size_type sz, Args&&... args) noexcept
    {
        static_assert(sizeof...(args) >= N-1, "Number of parameters for kedixa::multiarray incorrect.");

        // Note: sz may be zero
        _data.array_size = sz;
        _data.value = _data.allocate(sz);
        for(size_t i = 0; i < _data.array_size; ++i)
            _data.construct(_data.value + i, std::forward<Args>(args)...);
    }
    multiarray(const multiarray&) = delete;
    multiarray(multiarray&& ma) noexcept
    {
        _data.value = nullptr;
        _data.array_size = 0;
        std::swap(_data, ma._data);
    }

    multiarray& operator=(const multiarray&) = delete;
    multiarray& operator=(multiarray&& ma) noexcept
    {
        if(this != &ma) std::swap(_data, ma._data); 
        return *this;
    }

    constexpr unsigned dimension() const noexcept
    { return N; }

    size_type size() const noexcept
    { return _data.array_size; }

    pointer begin() noexcept
    { return _data.value; }

    const_pointer cbegin() const noexcept
    { return _data.value; }

    pointer end() noexcept
    { return _data.value + _data.array_size; }

    const_pointer cend() const noexcept
    { return _data.value + _data.array_size; }

    reference operator[](size_type pos) noexcept
    { return _data.value[pos]; }

    const_reference operator[](size_type pos) const noexcept
    { return _data.value[pos]; }

    ~multiarray() noexcept
    {
        for(size_t i = 0; i < _data.array_size; ++i)
            _data.destroy(_data.value + i);
        _data.deallocate(_data.value, _data.array_size);
    }
};
template<typename T>
class multiarray<T, 0>
{
public:
    // Help user find zero dimension error, rather than tons of template error.
    template<typename... Args>
    multiarray(Args&&... args) = delete;
};
}

#endif // C++11
#endif // MULTI_ARRAY_H
