/*
 * This file is part of klibcpp.
 * bigint.cpp - Signed big int class implement.
 * 
 * License: Apache 2.0
 * Read the Readme.md here for more infomation:
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2016-2017 kedixa(kedixa@outlook.com)
 *
 */

#include "bigint.h"
#include <cassert>
#include <stdexcept>

namespace kedixa {

// constructors
bigint::bigint(const int_type number)
{
    llint_type num = number;
    sign = (number < 0);
    ubig = unsigned_bigint(sign ? uint_type(-num) : uint_type(num));
}
bigint::bigint(const unsigned_bigint &ubig, bool sign)
{
    this->ubig = ubig;
    this->sign = sign;
}
bigint::bigint(unsigned_bigint &&ubig, bool sign)
{
    this->ubig = std::move(ubig);
    this->sign = sign;
}
bigint::bigint(const std::string &str)
{
    size_type len = str.length();
    if(len == 0)
    {
        ubig = unsigned_bigint(0u);
        sign = false;
    }
    else if(str[0] == '+' || str[0] == '-')
    {
        ubig = unsigned_bigint(str.substr(1, len - 1));
        sign = (str[0] == '-');
    }
    else
    {
        ubig = unsigned_bigint(str);
        sign = false;
    }
}
bigint::bigint(const char *ch)
    : bigint(std::string(ch)) {}
bigint::bigint(const bigint &big)
{
    this->sign = big.sign;
    this->ubig = big.ubig;
}
bigint::bigint(bigint &&big)
{
    this->sign = big.sign;
    this->ubig = std::move(big.ubig);
}

// assignment operators
bigint& bigint::operator=(const bigint &big)
{
    if(this != &big)
    {
        this->sign = big.sign;
        this->ubig = big.ubig;
    }
    return *this;
}
bigint& bigint::operator=(bigint &&big)
{
    if(this != &big)
    {
        this->sign = big.sign;
        this->ubig = std::move(big.ubig);
    }
    return *this;
}
bigint& bigint::operator+=(const int_type number)
{ return this->add_eq(number); }
bigint& bigint::operator+=(const bigint &big)
{ return this->add_eq(big); }
bigint& bigint::operator-=(const int_type number)
{ return this->sub_eq(number); }
bigint& bigint::operator-=(const bigint &big)
{ return this->sub_eq(big); }
bigint& bigint::operator*=(const int_type number)
{ return this->multi_eq(number); }
bigint& bigint::operator*=(const bigint &big)
{ return this->multi_eq(big); }
bigint& bigint::operator/=(const int_type number)
{ return this->div_eq(number); }
bigint& bigint::operator/=(const bigint &big)
{ return this->div_eq(big); }
bigint& bigint::operator%=(const int_type number)
{ return this->mod_eq(number); }
bigint& bigint::operator%=(const bigint &big)
{ return this->mod_eq(big); }

// conversion operators
bigint::operator bool() const noexcept
{ return *this != 0; }
bigint::operator int_type() const noexcept
{
    auto i = static_cast<unsigned_bigint::uint_type>(this->ubig);
    int_type result = static_cast<int_type>(i);
    if(this->sign) return -result;
    else return result;
}
bigint::operator llint_type() const noexcept
{
    auto i = static_cast<unsigned_bigint::ull_type>(this->ubig);
    llint_type result = static_cast<llint_type>(i);
    if(this->sign) return -result;
    else return result;
}
bigint::operator unsigned_bigint() const noexcept
{ return this->ubig; }

// conversion functions
bigint::int_type bigint::to_int() const
{
    auto i = static_cast<int_type>(*this);
    if(*this == bigint(std::to_string(i)))
        return i;
    else
    {
        throw std::runtime_error("bigint too large to convert to int.");
        return 0;
    }
}
bigint::llint_type bigint::to_ll() const
{
    auto i = static_cast<llint_type>(*this);
    if(*this == bigint(std::to_string(i)))
        return i;
    else
    {
        throw std::runtime_error("bigint too large to convert to long long.");
        return 0LL;
    }
}
long double bigint::to_ld() const
{
    auto d = this->ubig.to_ld();
    if(this->sign) return -d;
    else return d;
}
unsigned_bigint bigint::to_ubigint() const
{ return this->ubig; }

// arithmetic operators
bigint operator+(const bigint &lhs, const bigint &rhs)
{ return std::move(lhs.add(rhs)); }
bigint operator-(const bigint &lhs, const bigint &rhs)
{ return std::move(lhs.sub(rhs)); }
bigint operator*(const bigint &lhs, const bigint &rhs)
{ return std::move(lhs.multi(rhs)); }
bigint operator/(const bigint &lhs, const bigint &rhs)
{ return std::move(lhs.div(rhs)); }
bigint operator%(const bigint &lhs, const bigint &rhs)
{ return std::move(lhs.mod(rhs)); }

bigint& bigint::operator++()
{
    int cmp = this->compare(bigint(-1));
    if(cmp < 0) --this->ubig;
    else if(cmp > 0) ++this->ubig;
    else *this = bigint(0);
    return *this;
}
bigint& bigint::operator--()
{
    int cmp = this->compare(bigint(0));
    if(cmp > 0) --this->ubig;
    else if(cmp < 0) ++this->ubig;
    else *this = bigint(-1);
    return *this;
}
bigint bigint::operator++(int)
{
    bigint tmp = *this;
    ++*this;
    return std::move(tmp);
}
bigint bigint::operator--(int)
{
    bigint tmp = *this;
    --*this;
    return std::move(tmp);
}
bigint bigint::operator+ () const
{ return *this; }
bigint bigint::operator- () const
{
    bigint tmp = *this;
    tmp.inverse();
    return std::move(tmp);
}

// bitwise operators
bigint operator<<(const bigint &big, bigint::size_type sz)
{
    bigint tmp = big;
    tmp.ubig <<= sz;
    return std::move(tmp);
}
bigint operator>>(const bigint &big, bigint::size_type sz)
{
    if(sz == 0) return big;
    bigint tmp = big;
    if(tmp.sign == false)
        tmp.ubig >>= sz;
    // right shift for negative number
    else if(tmp.sign == true)
    {
        auto x = (unsigned_bigint(1u) << sz) - 1;
        x &= tmp.ubig; // lower sz bits of tmp.ubig
        tmp.ubig >>= sz;
        // if x is not 0, the answer should add by 1
        // for example, -9 >> 2 = -3, -8 >> 2 = -2
        if(x != unsigned_bigint(0u))
            ++tmp.ubig;
    }
    return std::move(tmp);
}

// relational operators
bool operator< (const bigint &lhs, const bigint &rhs) noexcept
{ return lhs.compare(rhs) < 0; }
bool operator==(const bigint &lhs, const bigint &rhs) noexcept
{ return lhs.compare(rhs) == 0; }
bool operator<=(const bigint &lhs, const bigint &rhs) noexcept
{ return !(rhs < lhs); }
bool operator> (const bigint &lhs, const bigint &rhs) noexcept
{ return rhs < lhs; }
bool operator>=(const bigint &lhs, const bigint &rhs) noexcept
{ return !(lhs < rhs); }
bool operator!=(const bigint &lhs, const bigint &rhs) noexcept
{ return !(lhs == rhs); }

// others
int bigint::compare(const bigint &big) const noexcept
{
    if(this->sign == big.sign)
    {
        int cmp = this->ubig.compare(big.ubig);
        if(this->sign == 0) return cmp;
        else return -cmp;
    }
    else if(this->sign == 0) return 1;
    else return -1;
}

bigint bigint::add(const bigint &big) const
{
    bigint result;
    if(this->sign == big.sign)
        result = bigint(this->ubig + big.ubig, this->sign);
    else
    {
        int cmp = this->ubig.compare(big.ubig);
        if(cmp > 0)
            result = bigint(this->ubig - big.ubig, this->sign);
        else if(cmp < 0)
            result = bigint(big.ubig - this->ubig, !this->sign);
        else result = bigint(0);
    }
    return std::move(result);
}
bigint& bigint::add_eq(const bigint &big)
{
    if(this->sign == big.sign)
        this->ubig += big.ubig;
    else
    {
        int cmp = this->ubig.compare(big.ubig);
        if(cmp > 0)
            this->ubig -= big.ubig;
        else if(cmp < 0)
            *this = bigint(big.ubig - this->ubig, !this->sign);
        else *this = bigint(0);
    }
    return *this;
}
bigint bigint::sub(const bigint &big) const
{
    bigint result;
    if(this->sign != big.sign)
        result = bigint(this->ubig + big.ubig, this->sign);
    else
    {
        int cmp = this->ubig.compare(big.ubig);
        if(cmp > 0)
            result = bigint(this->ubig - big.ubig, this->sign);
        else if(cmp < 0)
            result = bigint(big.ubig - this->ubig, !this->sign);
        else result = bigint(0);
    }
    return std::move(result);
}
bigint& bigint::sub_eq(const bigint &big)
{
    if(this->sign != big.sign)
        this->ubig += big.ubig;
    else
    {
        int cmp = this->ubig.compare(big.ubig);
        if(cmp > 0)
            this->ubig -= big.ubig;
        else if(cmp < 0)
            *this = bigint(big.ubig - this->ubig, !this->sign);
        else *this = bigint(0);
    }
    return *this;
}
bigint bigint::multi(const bigint &big) const
{
    if(this->ubig == 0 || big.ubig == 0) return bigint(0);
    return bigint(this->ubig * big.ubig, this->sign ^ big.sign);
}
bigint& bigint::multi_eq(const bigint &big)
{
    if(this->ubig == 0 || big.ubig == 0) *this = bigint(0);
    else this->ubig *= big.ubig, this->sign ^= big.sign;
    return *this;
}
bigint bigint::div(const bigint &big) const
{ return std::move(this->div_mod(big).first); }
bigint& bigint::div_eq(const bigint &big)
{ return *this = this->div_mod(big).first; }
bigint bigint::mod(const bigint &big) const
{ return std::move(this->div_mod(big).second); }
bigint& bigint::mod_eq(const bigint &big)
{ return *this = this->div_mod(big).second; }

std::pair<bigint, bigint> bigint::div_mod(const bigint &big) const
{
    auto result = this->ubig.div_mod(big.ubig);
    if(this->sign == big.sign)
        return std::make_pair(bigint(std::move(result.first)),
                              bigint(std::move(result.second)));
    else
    {
        // if they have different sign, adjust the result as follow.
        // for example, -11/7 = -2 ... 3, and 11 / -7 = -2 ... -3
        // but 11 / 7 = 1 ... 4
        if(result.second != 0u)
        {
            ++result.first;
            result.second = big.ubig - result.second;
            return std::make_pair(bigint(std::move(result.first), 1),
                        bigint(std::move(result.second), big.sign));
        }
        else
            return std::make_pair(bigint(std::move(result.first), 1),
                        bigint(0));
    }
}
void bigint::inverse() noexcept
{ if(*this != bigint(0)) this->sign = !this->sign; }
void bigint::swap(bigint  &big) noexcept
{
    std::swap(this->sign, big.sign);
    this->ubig.swap(big.ubig);
}
bigint::size_type bigint::size() const noexcept
{ return this->ubig.size(); }
std::string bigint::to_string(bool reverse) const
{
    std::string str = this->ubig.to_string(true);
    if(this->sign) str.push_back('-');
    if(!reverse)
    {
        auto first = str.begin(), last = str.end();
        while((first != last) && (first != --last))
            std::iter_swap(first, last), ++first;
    }
    return std::move(str);
}
bigint::~bigint() noexcept
{ }

// non-member function
void swap(bigint &big1, bigint &big2) noexcept
{
    big1.swap(big2);
    return;
}

} // namespace kedixa
