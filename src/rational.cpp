/*
 * This file is part of klibcpp.
 * rational.cpp - Rational class implement.
 * 
 * License: Apache 2.0
 * Read the Readme.md here for more infomation:
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2017 kedixa(kedixa@outlook.com)
 *
 */

#include "rational.h"
#include <stdexcept>

namespace kedixa {

// constructors
rational::rational(const ubigint &n, const ubigint &d, bool sign)
{
    // Each rational number must have only one representation,
    // avoid n/0, 2/4, -0/1, 4/2 ...
    this->num  = n;
    this->den  = d;
    this->sign = sign;
    this->reduce();
}
rational::rational(ubigint &&n, ubigint &&d, bool sign)
{
    this->num  = std::move(n);
    this->den  = std::move(d);
    this->sign = sign;
    this->reduce();
}
rational::rational(const rational &rat)
{
    this->num  = rat.num;
    this->den  = rat.den;
    this->sign = rat.sign;
}
rational::rational(rational &&rat)
{
    this->num  = std::move(rat.num);
    this->den  = std::move(rat.den);
    this->sign = rat.sign;
}

// assignment operators
rational& rational::operator= (const rational &rat)
{
    if(this == &rat) return *this;
    this->num  = rat.num;
    this->den  = rat.den;
    this->sign = rat.sign;
    return *this;
}
rational& rational::operator= (rational &&rat)
{
    if(this == &rat) return *this;
    this->num  = std::move(rat.num);
    this->den  = std::move(rat.den);
    this->sign = rat.sign;
    return *this;
}
rational& rational::operator+=(const rational &rat)
{ return this->add_eq(rat); }
rational& rational::operator-=(const rational &rat)
{ return this->sub_eq(rat); }
rational& rational::operator*=(const rational &rat)
{ return this->multi_eq(rat); }
rational& rational::operator/=(const rational &rat)
{ return this->div_eq(rat); }

// conversion operators
rational::operator bool() const noexcept
{ return this->num != 0u; }

// conversion functions
long double rational::to_ld() const
{
    long double n = this->num.to_ld();
    long double d = this->den.to_ld();
    return sign ? -n/d : n/d;
}

//arithmetic operators
rational operator+ (const rational &lhs, const rational &rhs)
{ return lhs.add(rhs); }
rational operator- (const rational &lhs, const rational &rhs)
{ return lhs.sub(rhs); }
rational operator* (const rational &lhs, const rational &rhs)
{ return lhs.multi(rhs); }
rational operator/ (const rational &lhs, const rational &rhs)
{ return lhs.div(rhs); }

rational rational::operator+ () const
{ return *this; }
rational rational::operator- () const
{
    rational tmp = *this;
    tmp.opposite();
    return tmp;
}

// relational operators
bool operator< (const rational &lhs, const rational &rhs)
{ return lhs.compare(rhs) < 0; }
bool operator==(const rational &lhs, const rational &rhs) noexcept
{ return lhs.sign==rhs.sign && lhs.num==rhs.num && lhs.den==rhs.den; }
bool operator<=(const rational &lhs, const rational &rhs)
{ return !(rhs < lhs); }
bool operator> (const rational &lhs, const rational &rhs)
{ return rhs < lhs; }
bool operator>=(const rational &lhs, const rational &rhs)
{ return !(lhs < rhs); }
bool operator!=(const rational &lhs, const rational &rhs) noexcept
{ return !(lhs == rhs); }

// others
int rational::compare(const rational &rat) const
{
    int cmp;
    // same sign
    if(this->sign != rat.sign)
        cmp = (this->sign == false) ? 1 : -1;
    // different sign
    else
    {
        // same denominator
        if(this->den == rat.den)
            cmp = this->num.compare(rat.num);
        // different denominator
        else
            cmp = (this->num*rat.den).compare(this->den * rat.num);
        // -cmp for negative number
        if(this->sign) cmp = -cmp;
    }
    return cmp;
}

rational rational::add (const rational &rat) const
{
    ubigint g = gcd(this->den, rat.den); // great common divisor
    ubigint ag = this->den / g, cg = rat.den / g; // den / gcd

    // using common method for fraction plus
    // b/a + d/c = (bc+ad)/(ac) = (bc/g + da/g)/(ac/g)
    rational result;
    const ubigint &b = this->num, &d = rat.num;
    result.den = ag * rat.den;
    if(this->sign == rat.sign)
    {
        result.num = b * cg + d * ag;
        result.sign = this->sign;
    }
    else
    {
        ubigint bcg = b * cg, dag = d * ag;
        int cmp = bcg.compare(dag);
        if(cmp >= 0)
        {
            result.num = std::move(bcg) - dag;
            result.sign = this->sign;
        }
        else
        {
            result.num = std::move(dag) - bcg;
            result.sign = !this->sign;
        }
    }
    result.reduce();
    return result;
}
rational& rational::add_eq(const rational &rat)
{ return *this = this->add(rat); }
rational  rational::sub (const rational &rat) const
{
    ubigint g = gcd(this->den, rat.den); // great common divisor
    ubigint ag = this->den / g, cg = rat.den / g; // den / gcd

    // using common method for fraction minus
    // b/a - d/c = (bc-ad)/(ac) = (bc/g + da/g)/(ac/g)
    rational result;
    const ubigint &b = this->num, &d = rat.num;
    result.den = ag * rat.den;
    if(this->sign == rat.sign)
    {
        ubigint bcg = b * cg, dag = d * ag;
        int cmp = bcg.compare(dag);
        if(cmp >= 0)
        {
            result = std::move(bcg) - dag;
            result.sign = this->sign;
        }
        else
        {
            result = std::move(dag) - bcg;
            result.sign = !this->sign;
        }
    }
    else
    {
        result.num = b * cg + d * ag;
        result.sign = this->sign;
    }
    result.reduce();
    return result;
}
rational& rational::sub_eq(const rational &rat)
{ return *this = this->sub(rat); }
rational  rational::multi(const rational &rat) const
{
    rational result;
    // special case
    if(this->num == ubigint(0u) || rat.num == ubigint(0u))
        result = rational();
    // common method for fraction multiply
    else
    {
        const ubigint &a = this->den, &b = this->num, &c = rat.den, &d = rat.num;
        ubigint g1 = gcd(a, d), g2 = gcd(c, b);
        result.num = (b/g2) * (d/g1);
        result.den = (a/g1) * (c/g2);
        result.sign= this->sign ^ rat.sign;
    }
    return result;
}
rational& rational::multi_eq(const rational &rat)
{ return *this = this->multi(rat); }
rational  rational::div(const rational &rat) const
{
    rational result;
    if(this->num == ubigint(0u))
        result = rational();
    else if(rat.num == 0)
        throw std::runtime_error("kedixa::rational: divide by zero.");
    else
    {
        // b/a / d/c = b/a * c/d
        const ubigint &a = this->den, &b = this->num, &c = rat.den, &d = rat.num;
        ubigint g1 = gcd(a, c), g2 = gcd(d, b);
        result.num = (b/g2) * (c/g1);
        result.den = (a/g1) * (d/g2);
        result.sign = this->sign ^ rat.sign;
    }
    return result;
}
rational& rational::div_eq(const rational &rat)
{ return *this = this->div(rat); }

rational::ubigint rational::get_num() const
{ return this->num; }
rational::ubigint rational::get_den() const
{ return this->den; }
bool rational::get_sign() const
{ return this->sign; }
void rational::approximate(rational::size_type hint)
{
    size_type sz = this->den.size();
    if(sz == 1) return;
    if(hint == 0) hint = sz/2;
    else if(hint >= sz) hint = sz - 1;
    this->num >>= (hint * ubigint::UINT_LEN);
    this->den >>= (hint * ubigint::UINT_LEN);
    this->reduce();
    return;
}
void rational::opposite() noexcept
{ 
    if(this->num != ubigint(0u))
        this->sign = !this->sign;
    return;
}
void rational::reciprocal()
{
    if(this->num == 0)
        throw std::runtime_error("kedixa::rational: numerator is zero.");
    else this->num.swap(this->den);
    return;
}
void rational::swap(rational &rat) noexcept
{
    this->num.swap(rat.num);
    this->den.swap(rat.den);
    std::swap(this->sign, rat.sign);
    return;
}
rational::size_type rational::num_size() const noexcept
{ return this->num.size(); }
rational::size_type rational::den_size() const noexcept
{ return this->den.size(); }
std::string rational::to_string(bool reverse) const
{
    std::string str;
    if(this->den != ubigint(1u)) 
    {
        str += this->den.to_string(true);
        str += '/';
    }
    str += this->num.to_string(true);
    if(this->sign == true) str += '-';
    if(!reverse)
    {
        auto first = str.begin(), last = str.end();
        while((first != last) && (first != --last))
            std::iter_swap(first, last), ++first;
    }
    return str;
}
std::string rational::to_decimal(rational::size_type digital_digits, bool reverse) const
{
    auto qr = this->num.div_mod(this->den);
    std::string str;
    if(digital_digits > 0)
    {
        std::string s = "1" + std::string(digital_digits, '0');
        str += (qr.second * ubigint(s) / this->den).to_string(true);
        str += std::string(digital_digits - str.length(), '0');
        str += ".";
    }
    str += qr.first.to_string(true);
    if(this->sign && this->num != ubigint(0u))
        str += "-";
    if(!reverse)
    {
        auto first = str.begin(), last = str.end();
        while((first != last) && (first != --last))
            std::iter_swap(first, last), ++first;
    }
    return str;
}
rational::~rational() noexcept
{}

// private
rational::ubigint rational::gcd(rational::ubigint x, rational::ubigint y)
{
    if(x > y) x.swap(y);
    while(x > 0)
    {
        ubigint q = y % x;
        y = std::move(x);
        x = std::move(q);
    }
    return y;
}
void rational::reduce()
{
    if(this->den == 0u)
        throw std::runtime_error("kedixa::rational: denominator is zero.");
    if(this->num == 0u)
    {
        this->sign = false;
        this->den  = ubigint(1);
    }
    else
    {
        ubigint g = gcd(this->num, this->den);
        this->num /= g;
        this->den /= g;
    }
    return;
}

} // namespace kedixa
