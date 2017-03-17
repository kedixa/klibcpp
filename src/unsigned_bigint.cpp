
/*
 * This file is part of klibcpp.
 * unsigned_bigint.cpp - unsigned_bigint class implement.
 * 
 * License: Apache 2.0
 * Read the Readme.md here for more infomation:
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2016-2017 kedixa(kedixa@outlook.com)
 *
 */

#include "unsigned_bigint.h"
#include <cassert>
#include <stdexcept>
#include <cmath>

namespace kedixa {

// constructors
unsigned_bigint::unsigned_bigint(const uint_type number)
{ digits.resize(1, number); }
unsigned_bigint::unsigned_bigint(const vector<uint_type> &_digits)
{ 
    if(_digits.empty()) digits.assign(1, uint_type(0));
    else digits = _digits;
    this->strip();
}
unsigned_bigint::unsigned_bigint(vector<uint_type> &&_digits)
{
    if(_digits.empty()) digits.assign(1, uint_type(0));
    else digits.swap(_digits);
    this->strip();
}
unsigned_bigint::unsigned_bigint(const std::string& str)
{
    if(str.empty())
    {
        this->digits.assign(1, uint_type(0));
        return;
    }
    // string to uint_type
    auto stouint = [&str](size_type first, size_type last) {
        uint_type result = 0;
        while(first != last)
        {
            result *= 10;
            result += str[first] - '0';
            ++first;
        }
        return result;
    };
    size_type len = str.length();
    size_type pos = len % SUBSTR_LEN;
    if(pos == 0) pos = SUBSTR_LEN;
    unsigned_bigint tmp(stouint(0, pos));
    for(; pos < len; pos += SUBSTR_LEN)
    {
        tmp *= TENS_MASK;
        tmp += uint_type(stouint(pos, pos + SUBSTR_LEN));
    }
    this->digits.swap(tmp.digits);
}
unsigned_bigint::unsigned_bigint(const char *ch)
    : unsigned_bigint(std::string(ch)) { }
unsigned_bigint::unsigned_bigint(const unsigned_bigint &ubigint)
{ this->digits = ubigint.digits; }
unsigned_bigint::unsigned_bigint(unsigned_bigint &&ubigint)
{ this->digits.swap(ubigint.digits); }

// assignment operators
unsigned_bigint& unsigned_bigint::operator=(const unsigned_bigint &ubigint)
{
    if(this != &ubigint)
        this->digits = ubigint.digits;
    return *this;
}
unsigned_bigint& unsigned_bigint::operator=(unsigned_bigint &&ubigint)
{
    this->digits.swap(ubigint.digits);
    return *this;
}

unsigned_bigint& unsigned_bigint::operator+=(const uint_type number)
{ return this->add_eq(number); }
unsigned_bigint& unsigned_bigint::operator+=(const unsigned_bigint &ubigint)
{ return this->add_eq(ubigint); }

unsigned_bigint& unsigned_bigint::operator-=(const uint_type number)
{ return this->sub_eq(number); }
unsigned_bigint& unsigned_bigint::operator-=(const unsigned_bigint &ubigint)
{ return this->sub_eq(ubigint); }

unsigned_bigint& unsigned_bigint::operator*=(const uint_type number)
{ return this->multi_eq(number); }
unsigned_bigint& unsigned_bigint::operator*=(const unsigned_bigint &ubigint)
{ return this->multi_eq(ubigint); }

unsigned_bigint& unsigned_bigint::operator/=(const uint_type number)
{ return this->div_eq(number); }
unsigned_bigint& unsigned_bigint::operator/=(const unsigned_bigint &ubigint)
{ return this->div_eq(ubigint); }

unsigned_bigint& unsigned_bigint::operator%=(const uint_type number)
{ return this->mod_eq(number); }
unsigned_bigint& unsigned_bigint::operator%=(const unsigned_bigint &ubigint)
{ return this->mod_eq(ubigint); }

unsigned_bigint& unsigned_bigint::operator&=(const unsigned_bigint &ubigint)
{
    auto &a = this->digits;
    const auto &b = ubigint.digits;
    if(a.size() > b.size()) a.resize(b.size());
    for(size_type i = 0; i < a.size(); ++i)
        a[i] &= b[i];
    this->strip();
    return *this;
}

unsigned_bigint& unsigned_bigint::operator|=(const unsigned_bigint &ubigint)
{
    auto &a = this->digits;
    const auto &b = ubigint.digits;
    if(a.size() < b.size()) a.resize(b.size(), 0);
    for(size_type i = 0; i < a.size(); ++i)
        a[i] |= b[i];
    return *this;
}

unsigned_bigint& unsigned_bigint::operator^=(const unsigned_bigint &ubigint)
{
    auto &a = this->digits;
    const auto &b = ubigint.digits;
    if(a.size() < b.size()) a.resize(b.size(), 0);
    for(size_type i = 0; i < a.size(); ++i)
        a[i] ^= b[i];
    this->strip();
    return *this;
}

unsigned_bigint& unsigned_bigint::operator<<=(const size_type k)
{
    if(*this != 0)
    {
        vector<uint_type> &a = this->digits;
        size_type r = k % UINT_LEN;
        if(r != 0)
        {
            ull_type s = 0;
            for(size_type i = 0; i < a.size(); ++i)
            {
                s |= (ull_type(a[i]) << r);
                a[i] = uint_type(s);
                s >>= UINT_LEN;
            }
            if(s) a.push_back(uint_type(s));
        }
        a.insert(a.begin(), k/UINT_LEN, 0);
    }
    return *this;
}

unsigned_bigint& unsigned_bigint::operator>>=(const size_type k) noexcept
{
    vector<uint_type> &a = this->digits;
    if(k/UINT_LEN >= a.size()) a.assign({0});
    else
    {
        a.erase(a.begin(), a.begin()+k/UINT_LEN);
        size_type r = k % UINT_LEN;
        if(r != 0)
        {
            ull_type s = 0;
            for(size_type i = a.size() - 1; i != (size_type) -1; --i)
            {
                s |= ull_type(a[i]);
                a[i] = uint_type(s >> r);
                s <<= UINT_LEN;
            }
        }
        this->strip();
    }
    return *this;
}

// conversion operators
unsigned_bigint::operator bool() const noexcept
{ return *this != 0; }
unsigned_bigint::operator uint_type() const noexcept
{ return this->digits[0]; }
unsigned_bigint::operator ull_type() const noexcept
{
    if(this->digits.size() == 1) return ull_type(this->digits[0]);
    else return (ull_type(this->digits[1]) << UINT_LEN) | this->digits[0];
}

// conversion functions
unsigned_bigint::uint_type unsigned_bigint::to_uint() const
{
    if(this->digits.size() > 1)
        throw std::runtime_error("unsigned_bigint too large to convert to uint.");
    return static_cast<uint_type>(*this);
}
unsigned_bigint::ull_type unsigned_bigint::to_ull() const
{
    if(this->digits.size() > 2)
        throw std::runtime_error("unsigned_bigint too large to convert to ull.");
    return static_cast<ull_type>(*this);
}
long double unsigned_bigint::to_ld() const
{
    long double d = 0;
    long double base = ull_type(1) << UINT_LEN;
    for(auto it = this->digits.crbegin(); it != this->digits.crend(); ++it)
    {
        d *= base;
        d += (long double)(*it);
    }
    if(std::isinf(d))
    {
        throw std::runtime_error("unsigned_bigint too large to convert to long double.");
        return 0;
    }
    return d;
}

// arthmetic operators
unsigned_bigint operator+ (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.add(rhs)); }
unsigned_bigint operator+ (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(rhs.add_eq(lhs)); }
unsigned_bigint operator+ (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.add_eq(rhs)); }
unsigned_bigint operator+ (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.add_eq(rhs)); }
unsigned_bigint operator+ (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.add(rhs)); }
unsigned_bigint operator+ (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs)
{ return std::move(rhs.add(lhs)); }
unsigned_bigint operator+ (unsigned_bigint &&lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.add_eq(rhs)); }
unsigned_bigint operator+ (const unsigned_bigint::uint_type lhs, unsigned_bigint &&rhs)
{ return std::move(rhs.add_eq(lhs)); }

unsigned_bigint operator- (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.sub(rhs)); }
unsigned_bigint operator- (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.sub(rhs)); }
unsigned_bigint operator- (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.sub_eq(rhs)); }
unsigned_bigint operator- (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.sub_eq(rhs)); }
unsigned_bigint operator- (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.sub(rhs)); }
unsigned_bigint operator- (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs)
{ return std::move(unsigned_bigint(lhs).sub_eq(rhs)); }
unsigned_bigint operator- (unsigned_bigint &&lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.sub_eq(rhs)); }
unsigned_bigint operator- (const unsigned_bigint::uint_type lhs, unsigned_bigint &&rhs)
{ return std::move(unsigned_bigint(lhs).sub_eq(rhs)); }

unsigned_bigint operator* (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.multi(rhs)); }
unsigned_bigint operator* (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(rhs.multi_eq(lhs)); }
unsigned_bigint operator* (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.multi_eq(rhs)); }
unsigned_bigint operator* (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.multi_eq(rhs)); }
unsigned_bigint operator* (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.multi(rhs)); }
unsigned_bigint operator* (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs)
{ return std::move(rhs.multi(lhs)); }
unsigned_bigint operator* (unsigned_bigint &&lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.multi_eq(rhs)); }
unsigned_bigint operator* (const unsigned_bigint::uint_type lhs, unsigned_bigint &&rhs)
{ return std::move(rhs.multi_eq(lhs)); }

unsigned_bigint operator/ (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.div(rhs)); }
unsigned_bigint operator/ (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.div(rhs)); }
unsigned_bigint operator/ (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.div_eq(rhs)); }
unsigned_bigint operator/ (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.div_eq(rhs)); }
unsigned_bigint operator/ (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.div(rhs)); }
unsigned_bigint operator/ (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs)
{
    unsigned_bigint tmp(lhs);
    return std::move(tmp.div_eq(rhs));
}
unsigned_bigint operator/ (unsigned_bigint &&lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.div_eq(rhs)); }
unsigned_bigint operator/ (const unsigned_bigint::uint_type lhs, unsigned_bigint &&rhs)
{
    unsigned_bigint tmp(lhs);
    return std::move(tmp.div_eq(rhs));
}

unsigned_bigint operator% (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.mod(rhs)); }
unsigned_bigint operator% (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.mod(rhs)); }
unsigned_bigint operator% (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs.mod_eq(rhs)); }
unsigned_bigint operator% (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs.mod_eq(rhs)); }
unsigned_bigint operator% (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.mod(rhs)); }
unsigned_bigint operator% (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs)
{
    unsigned_bigint tmp(lhs);
    return std::move(tmp.mod_eq(rhs));
}
unsigned_bigint operator% (unsigned_bigint &&lhs, const unsigned_bigint::uint_type rhs)
{ return std::move(lhs.mod_eq(rhs)); }
unsigned_bigint operator% (const unsigned_bigint::uint_type lhs, unsigned_bigint &&rhs)
{
    unsigned_bigint tmp(lhs);
    return std::move(tmp.mod_eq(rhs));
}

unsigned_bigint& unsigned_bigint::operator++()
{ return *this += 1; }
unsigned_bigint& unsigned_bigint::operator--()
{ return *this -= 1; }
unsigned_bigint  unsigned_bigint::operator++(int)
{
    unsigned_bigint tmp = *this;
    ++*this;
    return std::move(tmp);
}
unsigned_bigint  unsigned_bigint::operator--(int)
{
    unsigned_bigint tmp = *this;
    --*this;
    return std::move(tmp);
}
unsigned_bigint  unsigned_bigint::operator+ () const
{
    unsigned_bigint tmp = *this;
    return std::move(tmp);
}

// bitwise operators
unsigned_bigint operator<<(const unsigned_bigint &lhs, const unsigned_bigint::size_type k)
{
    unsigned_bigint tmp = lhs;
    return std::move(tmp <<= k);
}
unsigned_bigint operator<<(unsigned_bigint &&lhs, const unsigned_bigint::size_type k)
{ return std::move(lhs <<= k); }
unsigned_bigint operator>>(const unsigned_bigint &lhs, const unsigned_bigint::size_type k)
{
    unsigned_bigint tmp = lhs;
    return std::move(tmp >>= k);
}
unsigned_bigint operator>>(unsigned_bigint &&lhs, const unsigned_bigint::size_type k)
{ return std::move(lhs >>= k); }


unsigned_bigint operator& (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{
    unsigned_bigint tmp = lhs;
    return std::move(tmp &= rhs);
}
unsigned_bigint operator& (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(rhs &= lhs); }
unsigned_bigint operator& (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs &= rhs); }
unsigned_bigint operator& (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs &= rhs); }

unsigned_bigint operator| (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{
    unsigned_bigint tmp = lhs;
    return std::move(tmp |= rhs);
}
unsigned_bigint operator| (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(rhs |= lhs); }
unsigned_bigint operator| (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs |= rhs); }
unsigned_bigint operator| (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs |= rhs); }

unsigned_bigint operator^ (const unsigned_bigint &lhs, const unsigned_bigint &rhs)
{
    unsigned_bigint tmp = lhs;
    return std::move(tmp ^= rhs);
}
unsigned_bigint operator^ (const unsigned_bigint &lhs, unsigned_bigint &&rhs)
{ return std::move(rhs ^= lhs); }
unsigned_bigint operator^ (unsigned_bigint &&lhs, const unsigned_bigint &rhs)
{ return std::move(lhs ^= rhs); }
unsigned_bigint operator^ (unsigned_bigint &&lhs, unsigned_bigint &&rhs)
{ return std::move(lhs ^= rhs); }

// relational operators
bool operator< (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs) noexcept
{ return lhs.digits.size() <= 1 && lhs.digits[0] < rhs; }
bool operator< (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs) noexcept
{ return rhs.digits.size() > 1 || lhs < rhs.digits[0]; }
bool operator< (const unsigned_bigint &lhs, const unsigned_bigint &rhs) noexcept
{ return lhs.compare(rhs) < 0; }

bool operator==(const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs) noexcept
{ return lhs.digits.size() <= 1 && lhs.digits[0] == rhs; }
bool operator==(const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs) noexcept
{ return rhs.digits.size() <= 1 && lhs == rhs.digits[0]; }
bool operator==(const unsigned_bigint &lhs, const unsigned_bigint &rhs) noexcept
{ return lhs.compare(rhs) == 0; }

bool operator<=(const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs) noexcept
{ return !(rhs < lhs); }
bool operator<=(const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs) noexcept
{ return !(rhs < lhs); }
bool operator<=(const unsigned_bigint &lhs, const unsigned_bigint &rhs) noexcept
{ return !(rhs < lhs); }

bool operator> (const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs) noexcept
{ return rhs < lhs; }
bool operator> (const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs) noexcept
{ return rhs < lhs; }
bool operator> (const unsigned_bigint &lhs, const unsigned_bigint &rhs) noexcept
{ return rhs < lhs; }

bool operator>=(const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs) noexcept
{ return !(lhs < rhs); }
bool operator>=(const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs) noexcept
{ return !(lhs < rhs); }
bool operator>=(const unsigned_bigint &lhs, const unsigned_bigint &rhs) noexcept
{ return !(lhs < rhs); }

bool operator!=(const unsigned_bigint &lhs, const unsigned_bigint::uint_type rhs) noexcept
{ return !(lhs == rhs); }
bool operator!=(const unsigned_bigint::uint_type lhs, const unsigned_bigint &rhs) noexcept
{ return !(lhs == rhs); }
bool operator!=(const unsigned_bigint &lhs, const unsigned_bigint &rhs) noexcept
{ return !(lhs == rhs); }

// others
int unsigned_bigint::compare(const unsigned_bigint &ubigint) const noexcept
{
    const auto &a = this->digits, &b = ubigint.digits;
    size_type lena = a.size(), lenb = b.size();
    if(lena > lenb) return 1;
    else if(lena < lenb) return -1;
    for(size_type i = lena - 1; i != (size_type)-1; --i)
        if(a[i] != b[i]) return (a[i] < b[i]) ? -1 : 1;
    return 0;
}

unsigned_bigint
unsigned_bigint::add(const unsigned_bigint &ubigint) const
{
    const vector<uint_type> *pa, *pb;
    if(this->digits.size() > ubigint.digits.size())
        pa = &(this->digits), pb = &(ubigint.digits);
    else pa = &(ubigint.digits), pb = &(this->digits);
    const vector<uint_type> &a = *pa, &b = *pb;
    size_type lena = a.size(), lenb = b.size();
    vector<uint_type> c(lena);
    ull_type carry = 0;
    size_type i = 0;
    for(; i < lenb; ++i)
    {
        carry += a[i];
        carry += b[i];
        c[i] = uint_type(carry);
        carry >>= UINT_LEN;
    }
    for(; i < lena; ++i)
    {
        carry += a[i];
        c[i] = uint_type(carry);
        carry >>= UINT_LEN;
    }
    if(carry) c.push_back(uint_type(carry));
    return std::move(unsigned_bigint(std::move(c)));
}
unsigned_bigint
unsigned_bigint::add(const uint_type number) const
{
    unsigned_bigint tmp = *this;
    tmp.add_eq(number);
    return std::move(tmp);
}
unsigned_bigint&
unsigned_bigint::add_eq(const unsigned_bigint &ubigint)
{
    size_type lena = this->digits.size(), lenb = ubigint.digits.size();
    if(lena < lenb) this->digits.resize(lenb, 0), lena = lenb;

    ull_type carry = 0;
    vector<uint_type> &a = this->digits;
    const vector<uint_type> &b = ubigint.digits;
    size_type i = 0;
    for(; i < lenb; ++i)
    {
        carry += ull_type(a[i]) + ull_type(b[i]);
        a[i] = uint_type(carry);
        carry >>= UINT_LEN;
    }
    for(; carry && i < lena; ++i)
    {
        if(a[i]==UINT_TYPE_MAX) a[i] = 0;
        else ++a[i], carry = 0;
    }
    if(carry) a.push_back(uint_type(carry));
    return *this;
}
unsigned_bigint&
unsigned_bigint::add_eq(const uint_type number)
{
    vector<uint_type> &a = this->digits;
    ull_type carry = ull_type(number) + ull_type(a[0]);
    a[0] = uint_type(carry);
    carry >>= UINT_LEN;
    for(size_type i = 1; carry && i < a.size(); ++i)
    {
        if(a[i]==UINT_TYPE_MAX) a[i] = 0;
        else ++a[i], carry = 0;
    }
    if(carry) a.push_back(uint_type(carry));
    return *this;
}

unsigned_bigint
unsigned_bigint::sub(const unsigned_bigint &ubigint) const
{
    unsigned_bigint tmp = *this;
    tmp.sub_eq(ubigint);
    return std::move(tmp);
}
unsigned_bigint
unsigned_bigint::sub(const uint_type number) const
{
    unsigned_bigint tmp = *this;
    tmp.sub_eq(number);
    return std::move(tmp);
}
unsigned_bigint&
unsigned_bigint::sub_eq(const unsigned_bigint &ubigint)
{
    size_type len1 = this->digits.size(), len2 = ubigint.digits.size();
    if(len1 < len2)
        throw std::underflow_error("unsigned_bigint:: subtraction underflow.");
    vector<uint_type> &a = this->digits;
    const vector<uint_type> &b = ubigint.digits;

    llint_type x = 0;
    size_type i = 0;
    for(; i < len2; ++i)
    {
        x += a[i];
        x -= b[i];
        a[i] = uint_type(x);
        x >>= UINT_LEN;
    }
    bool borrow = 0;
    if(x < 0) borrow = true;
    for(; borrow && i < len1; ++i)
    {
        if(a[i] == 0) a[i] = UINT_TYPE_MAX;
        else --a[i], borrow = false;
    }
    if(borrow)
        throw std::underflow_error("unsigned_bigint:: subtraction underflow.");
    this->strip();
    return *this;
}
unsigned_bigint&
unsigned_bigint::sub_eq(const uint_type number)
{
    vector<uint_type> &a = this->digits;
    llint_type x = llint_type(a[0]);
    x -= llint_type(number);
    bool borrow = false;
    a[0] = uint_type(x);
    if(x < 0) borrow = true;
    for(size_type i = 0; borrow && i < a.size(); ++i)
    {
        if(a[i] == 0) a[i] = UINT_TYPE_MAX;
        else --a[i], borrow = false;
    }
    if(borrow)
        throw std::underflow_error("unsigned_bigint:: subtraction underflow.");
    this->strip();
    return *this;
}

unsigned_bigint
unsigned_bigint::multi(const unsigned_bigint &ubigint) const
{
    // special case
    if(*this == 0 || ubigint == 0) return unsigned_bigint(uint_type(0));
    if(*this == 1) return ubigint;
    if(ubigint == 1) return *this;
    // special end

    const vector<uint_type> &a = this->digits, &b = ubigint.digits;
    size_type lena = a.size(), lenb = b.size();
    // if both a and b are long enough,
    // use divide and conquer algorithm
    if(lena > MULTI_HINT && lenb > MULTI_HINT)
    {
        unsigned_bigint a1, a2, b1, b2;
        vector<uint_type> &va1 = a1.digits, &va2 = a2.digits,
                          &vb1 = b1.digits, &vb2 = b2.digits;
        size_type len_max = std::max(lena, lenb);
        size_type m = len_max / 2;

        // divide a and b to two parts
        // lower m digits, and higher (lenx - m) digits
        va2.assign(a.begin(), a.begin() + std::min(m, lena));
        if(lena > m) va1.assign(a.begin() + m, a.end());
        else va1.assign(1, uint_type(0));
        vb2.assign(b.begin(), b.begin() + std::min(m, lenb));
        if(lenb > m) vb1.assign(b.begin() + m, b.end());
        else vb1.assign(1, uint_type(0));
        // conquer (x = unsigned_bigint(1u) << (m*UINT_LEN))
        //   (a1*x + a2)*(b1*x + b2)
        // = a1*b1*x^2 + (a1*b2 + a2*b1)*x + a2*b2
        // = a1*b1*x^2 + ((a1+a2)*(b1+b2)-a1*b1-a2*b2)*x + a2*b2
        unsigned_bigint a1b1 = a1 * b1, a2b2 = a2 * b2;
        unsigned_bigint a1b2_a2b1 = (a1 + a2)*(b1 + b2) - a1b1 - a2b2;
        a1b1 <<= (m * 2 * UINT_LEN);
        a1b2_a2b1 <<= (m * UINT_LEN);
        return std::move(std::move(a1b1) + a1b2_a2b1 + a2b2);
    }
    // else if they are not long
    // use ordinary method
    vector<uint_type> result(lena + lenb, 0);
    // multi b[0]
    ull_type x = ull_type(b[0]), y = 0;
    if(x != 0)
    {
        for(size_type j = 0; j < lena; ++j)
        {
            y += x * a[j];
            result[j] = uint_type(y);
            y >>= UINT_LEN;
        }
        result[lena] = uint_type(y); // carry
    }
    // multi b[1...]
    for(size_type i = 1; i < lenb; ++i)
    {
        x = ull_type(b[i]), y = 0;
        if(x == 0) continue;
        size_type j, k;
        for(j = 0, k = i; j < lena; ++j, ++k)
        {
            y += x * a[j] + result[k];
            result[k] = uint_type(y);
            y >>= UINT_LEN;
        }
        result[k] = uint_type(y);
    }
    unsigned_bigint tmp(std::move(result));
    return std::move(tmp);
}
unsigned_bigint
unsigned_bigint::multi(const uint_type number) const
{
    if(number == 0) return unsigned_bigint(uint_type(0));
    if(number == 1) return *this;
    const vector<uint_type> &a = this->digits;
    vector<uint_type> c(a.size());
    ull_type x = ull_type(number), y = 0;
    for(size_type i = 0; i < a.size(); ++i)
    {
        y += x * a[i];
        c[i] = uint_type(y);
        y >>= UINT_LEN;
    }
    if(y) c.push_back(uint_type(y));
    return unsigned_bigint(std::move(c));
}
unsigned_bigint&
unsigned_bigint::multi_eq(const unsigned_bigint &ubigint)
{
    *this = this->multi(ubigint);
    return *this;
}
unsigned_bigint&
unsigned_bigint::multi_eq(const uint_type number)
{
    if(number == 0)
    {
        this->digits.assign(1, uint_type(0));
        return *this;
    }
    if(number == 1) return *this;
    vector<uint_type> &a = this->digits;
    ull_type x = ull_type(number), y = 0;
    for(size_type i = 0; i < a.size(); ++i)
    {
        y += x * a[i];
        a[i] = uint_type(y);
        y >>= UINT_LEN;
    }
    if(y) a.push_back(uint_type(y));
    return *this;
}

unsigned_bigint
unsigned_bigint::div(const unsigned_bigint &ubigint) const
{ return std::move((this->div_mod(ubigint)).first); }
unsigned_bigint
unsigned_bigint::div(const uint_type number) const
{ return std::move((this->div_mod(number)).first); }
unsigned_bigint&
unsigned_bigint::div_eq(const unsigned_bigint &ubigint)
{
    *this = std::move((this->div_mod(ubigint)).first);
    return *this;
}
unsigned_bigint&
unsigned_bigint::div_eq(const uint_type number)
{
    if(number == 0)
        throw std::runtime_error("unsigned_bigint: divide by zero.");
    if(number == 1)
        return *this;
    vector<uint_type> &a = this->digits;
    ull_type x = ull_type(number), y = 0;
    size_type i = a.size() - 1;
    for(; i != (size_type)-1; --i)
    {
        y <<= UINT_LEN;
        y += a[i];
        a[i] = uint_type(y/x);
        y %= x;
    }
    this->strip();
    return *this;
}

unsigned_bigint
unsigned_bigint::mod(const unsigned_bigint &ubigint) const
{ return std::move((this->div_mod(ubigint)).second); }
unsigned_bigint
unsigned_bigint::mod(const uint_type number) const
{ return std::move((this->div_mod(number)).second); }
unsigned_bigint&
unsigned_bigint::mod_eq(const unsigned_bigint &ubigint)
{
    *this = std::move((this->div_mod(ubigint)).second);
    return *this;
}
unsigned_bigint&
unsigned_bigint::mod_eq(const uint_type number)
{
    if(number == 0)
        throw std::runtime_error("unsigned_bigint: divide by zero.");
    if(number == 1)
        return (*this = unsigned_bigint(uint_type(0)));
    vector<uint_type> &a = this->digits;
    ull_type x = ull_type(number), y = 0;
    size_type i = a.size() - 1;
    for(; i != (size_type)-1; --i)
    {
        y <<= UINT_LEN;
        y += a[i];
        a[i] = uint_type(y/x);
        y %= x;
    }
    *this = unsigned_bigint(y);
    return *this;
}

std::pair<unsigned_bigint, unsigned_bigint>
unsigned_bigint::div_mod(const unsigned_bigint &ubigint) const
{
    if(ubigint.digits.size() == 1)
        return this->div_mod(ubigint.digits[0]);
    // dividend less than divisor
    if(*this < ubigint)
        return std::make_pair(unsigned_bigint(uint_type(0)), unsigned_bigint(*this));

    // implement the algorithm in Knuth[The Art of Computer Programming]
    unsigned_bigint dividend = *this, divisor = ubigint, rem;
    vector<uint_type> q; // save the quot
    // 1. normalize
    ull_type base = ull_type(1) << UINT_LEN;
    size_type lshift = 0;
    uint_type back = divisor.digits.back();
    while(back < base/2) back<<=1, ++lshift;
    dividend <<= lshift, divisor <<= lshift;
    vector<uint_type> &a = dividend.digits;
    const vector<uint_type> &b = divisor.digits;
    size_type sz_b = b.size(); // size of divisor
    a.push_back(uint_type(0)); // for u0

    // 2. loop
    const uint_type &v1 = b[sz_b-1], &v2 = b[sz_b-2];
    q.resize(a.size() - b.size());
    for(size_type i = q.size() - 1; i != (size_type)-1; --i)
    {
        // get the estimated value of q
        ull_type qh;
        uint_type &u0 = a[i+sz_b], &u1 = a[i+sz_b-1], &u2 = a[i+sz_b-2];
        qh = (u0 * base + u1) / v1;
        if(qh * v2 > base * (u0 * base + u1 - qh * v1) + u2) --qh;

        // multi and subtract
        llint_type borrow = 0;
        ull_type carry = 0;
        for(size_type j = 0; j < sz_b; ++j)
        {
            carry += qh * b[j];
            borrow += a[i+j];
            borrow -= uint_type(carry);
            a[i+j] = uint_type(borrow);
            borrow >>= UINT_LEN;
            carry >>= UINT_LEN;
        }
        borrow += a[i+sz_b];
        borrow -= carry;
        a[i+sz_b] = uint_type(borrow);
        borrow >>= UINT_LEN;
        while(borrow < 0)
        {
            // add divisor back to dividend
            assert(borrow==-1);
            carry = 0;
            for(size_type j = 0; j < sz_b; ++j)
            {
                carry += a[i+j];
                carry += b[j];
                a[i+j] = uint_type(carry);
                carry >>= UINT_LEN;
            }
            carry += a[i+sz_b];
            a[i+sz_b] = uint_type(carry);
            carry >>= UINT_LEN;
            borrow += carry;
            --qh; // don't forget
        }
        assert(borrow == 0);
        q[i] = uint_type(qh);

    }
    // 3. get quotient and real remainder
    unsigned_bigint quotient(std::move(q));
    dividend.strip();
    dividend >>= lshift;
    return std::make_pair(std::move(quotient), std::move(dividend));
}
std::pair<unsigned_bigint, unsigned_bigint>
unsigned_bigint::div_mod(const uint_type number) const
{
    if(number == 0)
        throw std::runtime_error("unsigned_bigint: divide by zero.");
    if(number == 1)
        return std::make_pair(unsigned_bigint(*this), unsigned_bigint(uint_type(0)));
    unsigned_bigint tmp = *this;
    vector<uint_type> &a = tmp.digits;
    ull_type x = ull_type(number), y = 0;
    size_type i = a.size() - 1;
    for(; i != (size_type)-1; --i)
    {
        y <<= UINT_LEN;
        y += a[i];
        a[i] = uint_type(y/x);
        y %= x;
    }
    tmp.strip();
    return std::make_pair(std::move(tmp), unsigned_bigint(uint_type(y)));
}

void unsigned_bigint::swap(unsigned_bigint &ubigint) noexcept
{ this->digits.swap(ubigint.digits); }

std::string unsigned_bigint::to_string(bool reverse) const
{
    vector<uint_type> v;
    const vector<uint_type> a = this->digits;
    v.reserve(a.size() * TOSTR_HINT); // roughly allocate memory
    ull_type y;
    for(size_type i = a.size() - 1; i != size_type(-1); --i)
    {
        uint_type x = a[i];
        for(size_type j = 0; j < v.size(); ++j)
        {
            y = (ull_type(v[j]) << UINT_LEN) | x;
            x = uint_type(y / TENS_MASK);
            v[j] = uint_type(y % TENS_MASK);
        }
        while(x)
        {
            v.push_back(x % TENS_MASK);
            x /= TENS_MASK;
        }
    }
    std::string result;
    result.reserve(v.size() * SUBSTR_LEN);
    // If v.size > 1, add possible leading zeros to v[0..size-2]
    if(v.size() > 1)
    {
        for(size_type i = 0; i < v.size() - 1; ++i)
        {
            for(size_type j = 0; j < SUBSTR_LEN; ++j)
                result.push_back(v[i]%10 + '0'), v[i] /= 10;
        }
    }
    // But the last number in v don't need to
    auto &back = v.back();
    while(back)
    {
        result.push_back(back%10 + '0');
        back /= 10;
    }
    if(result.size() == 0) result.push_back('0');

    // the result itself is reversed, so if reverse is false,
    // we reverse the result.
    if(!reverse)
    {
        auto first = result.begin(), last = result.end();
        while((first != last) && (first != --last))
            std::iter_swap(first, last), ++first;
    }
    return std::move(result);
}

unsigned_bigint::size_type unsigned_bigint::size() const noexcept
{ return digits.size(); }

unsigned_bigint::~unsigned_bigint() noexcept
{ }

// private:
void unsigned_bigint::strip() noexcept
{
    vector<uint_type> &a = this->digits;
    size_type sz = a.size() - 1;
    while(sz > 0 && a[sz] == 0) --sz;
    a.resize(sz + 1);
    return;
}

// non-member function
void swap(unsigned_bigint &lhs, unsigned_bigint &rhs) noexcept
{ lhs.swap(rhs); }

} // namespace kedixa
