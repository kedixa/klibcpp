/*
 * This file is part of klibcpp.
 * rational.h - Rational class.
 * 
 * License: Apache 2.0
 * Read the Readme.md here for more infomation:
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2017 kedixa(kedixa@outlook.com)
 *
 */

#ifndef KEDIXA_RATIONAL_H
#define KEDIXA_RATIONAL_H

#include "unsigned_bigint.h"
namespace kedixa {

class rational
{
public:
    using size_type = unsigned_bigint::size_type;
    using uint_type = unsigned_bigint::uint_type;
    using int_type  = unsigned_bigint::int_type;
    using ull_type  = unsigned_bigint::ull_type;
    using llint_type= unsigned_bigint::llint_type;
    using ubigint   = unsigned_bigint;
private:
    ubigint num; // numerator
    ubigint den; // denominator
    bool sign;   // 0 for +, 1 for -

    static ubigint gcd(ubigint x, ubigint y);
    void reduce(); // reduce the fraction

public:
    // constructors
    rational(const ubigint &n = ubigint(0u), const ubigint &d = ubigint(1u), bool sign = false);
    rational(ubigint&&, ubigint&&, bool);
    rational(const rational&);
    rational(rational&&);

    // assignment operators
    rational& operator= (const rational&);
    rational& operator= (rational&&);
    rational& operator+=(const rational&);
    rational& operator-=(const rational&);
    rational& operator*=(const rational&);
    rational& operator/=(const rational&);

    // conversion operators
    explicit operator bool() const noexcept;

    // conversion functions
    long double to_ld() const;
    
    // arithmetic operators
    friend rational operator+ (const rational&, const rational&);
    friend rational operator- (const rational&, const rational&);
    friend rational operator* (const rational&, const rational&);
    friend rational operator/ (const rational&, const rational&);

    rational operator+ () const;
    rational operator- () const;

    // relational operators
    friend bool operator< (const rational&, const rational&);
    friend bool operator==(const rational&, const rational&) noexcept;
    friend bool operator<=(const rational&, const rational&);
    friend bool operator> (const rational&, const rational&);
    friend bool operator>=(const rational&, const rational&);
    friend bool operator!=(const rational&, const rational&) noexcept;

    // others
    int compare(const rational&) const;

    rational  add   (const rational&) const;
    rational& add_eq(const rational&);
    rational  sub   (const rational&) const;
    rational& sub_eq(const rational&);
    rational  multi (const rational&) const;
    rational& multi_eq(const rational&);
    rational  div   (const rational&) const;
    rational& div_eq(const rational&);

    ubigint get_num() const; // return numerator
    ubigint get_den() const; // return denominator
    bool    get_sign() const; // return sign
    // reduce the rational to a approximate one, to save space and time,
    // DON'T use it if you need a exact number.
    void approximate(size_type hint = 0);
    void opposite() noexcept;  // *this = -*this
    void reciprocal(); // *this = this->den/this->num
    void swap(rational&) noexcept;
    size_type num_size() const noexcept;
    size_type den_size() const noexcept;
    std::string to_string(bool reverse = false) const;
    std::string to_decimal(size_type decimal_digits = 16, bool reverse = false) const;
    ~rational() noexcept;

    friend std::hash<rational>;

}; // class rational

void swap(rational&, rational&) noexcept;

} // namespace kedixa

namespace std {
template<> struct hash<kedixa::rational>
{
    size_t operator()(const kedixa::rational &rat) const
    {
        auto h = hash<kedixa::unsigned_bigint>();
        size_t hash_code = h(rat.num) ^ h(rat.den);
        if(rat.sign) hash_code = ~hash_code, ++hash_code;
        return hash_code;
    }
};
}

#endif // KEDIXA_RATIONAL_H
