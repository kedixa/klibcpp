/*
 * This file is part of klibcpp.
 * unsigned_bigint.h - Unsigned big int class.
 * 
 * License: Apache 2.0
 * Read the Readme.md here for more infomation:
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2016-2017 kedixa(kedixa@outlook.com)
 *
 */

#ifndef KEDIXA_UNSIGNED_BIGINT_H
#define KEDIXA_UNSIGNED_BIGINT_H

#include <cstdint>
#include <vector>
#include <string>
#include <utility>
#include <functional>
namespace kedixa {

namespace _k_help {
template<typename T, unsigned N>
struct NPOW_OF_TEN
{ static const T value = NPOW_OF_TEN<T, N-1>::value * 10; };

template<typename T>
struct NPOW_OF_TEN<T, 0>
{ static const T value = 1; };
} // namespace _k_help

class unsigned_bigint
{
    template<typename T>
    using vector      = std::vector<T>;
    template<typename T, unsigned N>
    using NPOW_OF_TEN = _k_help::NPOW_OF_TEN<T, N>;
public:
    using size_type   = size_t;
    using uint_type   = uint32_t;
    using int_type    = int32_t;
    using ull_type    = uint64_t;
    using llint_type  = int64_t;

    static const uint_type UINT_TYPE_MAX = uint_type(-1);
    static const size_type UINT_LEN      = sizeof(uint_type) * 8;
    static const size_type SUBSTR_LEN    = UINT_LEN / 4;
    static const ull_type  TENS_MASK     = NPOW_OF_TEN<ull_type, SUBSTR_LEN>::value;

private:
    static const size_type MULTI_HINT    = 130;
    static constexpr double TOSTR_HINT   = 0.302 * UINT_LEN / SUBSTR_LEN; // 0.302 ~= log10(2)

    vector<uint_type> digits;
    void strip() noexcept;
public:
    // constructors
    explicit
    unsigned_bigint(const uint_type number = 0 );
    explicit
    unsigned_bigint(const vector<uint_type>&   );
    explicit
    unsigned_bigint(vector<uint_type>&&        );
    explicit
    unsigned_bigint(const std::string&         );
    explicit
    unsigned_bigint(const char*                );
    unsigned_bigint(const unsigned_bigint&     );
    unsigned_bigint(unsigned_bigint&&          );

    // assignment operators
    unsigned_bigint& operator=  (const unsigned_bigint& );
    unsigned_bigint& operator=  (unsigned_bigint&&      );
    unsigned_bigint& operator+= (const uint_type        );
    unsigned_bigint& operator+= (const unsigned_bigint& );
    unsigned_bigint& operator-= (const uint_type        );
    unsigned_bigint& operator-= (const unsigned_bigint& );
    unsigned_bigint& operator*= (const uint_type        );
    unsigned_bigint& operator*= (const unsigned_bigint& );
    unsigned_bigint& operator/= (const uint_type        );
    unsigned_bigint& operator/= (const unsigned_bigint& );
    unsigned_bigint& operator%= (const uint_type        );
    unsigned_bigint& operator%= (const unsigned_bigint& );
    unsigned_bigint& operator&= (const unsigned_bigint& );
    unsigned_bigint& operator|= (const unsigned_bigint& );
    unsigned_bigint& operator^= (const unsigned_bigint& );
    unsigned_bigint& operator<<=(const size_type        );
    unsigned_bigint& operator>>=(const size_type)noexcept;

    // conversion operators
    explicit operator bool()      const noexcept;
    explicit operator uint_type() const noexcept;
    explicit operator ull_type()  const noexcept;

    // conversion functions
    uint_type to_uint() const;
    ull_type  to_ull()  const;
    long double to_ld() const;

    // arithmetic operators
    friend unsigned_bigint operator+ (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator+ (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator+ (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator+ (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator+ (const unsigned_bigint&, const uint_type        );
    friend unsigned_bigint operator+ (const uint_type,        const unsigned_bigint& );
    friend unsigned_bigint operator+ (unsigned_bigint&&,      const uint_type        );
    friend unsigned_bigint operator+ (const uint_type,        unsigned_bigint&&      );

    friend unsigned_bigint operator- (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator- (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator- (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator- (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator- (const unsigned_bigint&, const uint_type        );
    friend unsigned_bigint operator- (const uint_type,        const unsigned_bigint& );
    friend unsigned_bigint operator- (unsigned_bigint&&,      const uint_type        );
    friend unsigned_bigint operator- (const uint_type,        unsigned_bigint&&      );

    friend unsigned_bigint operator* (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator* (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator* (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator* (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator* (const unsigned_bigint&, const uint_type        );
    friend unsigned_bigint operator* (const uint_type,        const unsigned_bigint& );
    friend unsigned_bigint operator* (unsigned_bigint&&,      const uint_type        );
    friend unsigned_bigint operator* (const uint_type,        unsigned_bigint&&      );

    friend unsigned_bigint operator/ (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator/ (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator/ (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator/ (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator/ (const unsigned_bigint&, const uint_type        );
    friend unsigned_bigint operator/ (const uint_type,        const unsigned_bigint& );
    friend unsigned_bigint operator/ (unsigned_bigint&&,      const uint_type        );
    friend unsigned_bigint operator/ (const uint_type,        unsigned_bigint&&      );


    friend unsigned_bigint operator% (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator% (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator% (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator% (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator% (const unsigned_bigint&, const uint_type        );
    friend unsigned_bigint operator% (const uint_type,        const unsigned_bigint& );
    friend unsigned_bigint operator% (unsigned_bigint&&,      const uint_type        );
    friend unsigned_bigint operator% (const uint_type,        unsigned_bigint&&      );

    unsigned_bigint& operator++();
    unsigned_bigint& operator--();
    unsigned_bigint  operator++(int);
    unsigned_bigint  operator--(int);
    unsigned_bigint  operator+ () const;
    unsigned_bigint  operator- () const = delete;

    // bitwise operators
    friend unsigned_bigint operator<<(const unsigned_bigint&, size_type              );
    friend unsigned_bigint operator<<(unsigned_bigint&&,      size_type              );
    friend unsigned_bigint operator>>(const unsigned_bigint&, size_type              );
    friend unsigned_bigint operator>>(unsigned_bigint&&,      size_type              );
    friend unsigned_bigint operator& (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator& (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator& (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator& (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator| (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator| (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator| (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator| (unsigned_bigint&&,      unsigned_bigint&&      );
    friend unsigned_bigint operator^ (const unsigned_bigint&, const unsigned_bigint& );
    friend unsigned_bigint operator^ (const unsigned_bigint&, unsigned_bigint&&      );
    friend unsigned_bigint operator^ (unsigned_bigint&&,      const unsigned_bigint& );
    friend unsigned_bigint operator^ (unsigned_bigint&&,      unsigned_bigint&&      );

    // relational operators
    friend bool operator< (const unsigned_bigint&, const uint_type        ) noexcept;
    friend bool operator< (const uint_type,        const unsigned_bigint& ) noexcept;
    friend bool operator< (const unsigned_bigint&, const unsigned_bigint& ) noexcept;
    friend bool operator==(const unsigned_bigint&, const uint_type        ) noexcept;
    friend bool operator==(const uint_type,        const unsigned_bigint& ) noexcept;
    friend bool operator==(const unsigned_bigint&, const unsigned_bigint& ) noexcept;
    friend bool operator<=(const unsigned_bigint&, const uint_type        ) noexcept;
    friend bool operator<=(const uint_type,        const unsigned_bigint& ) noexcept;
    friend bool operator<=(const unsigned_bigint&, const unsigned_bigint& ) noexcept;
    friend bool operator> (const unsigned_bigint&, const uint_type        ) noexcept;
    friend bool operator> (const uint_type,        const unsigned_bigint& ) noexcept;
    friend bool operator> (const unsigned_bigint&, const unsigned_bigint& ) noexcept;
    friend bool operator>=(const unsigned_bigint&, const uint_type        ) noexcept;
    friend bool operator>=(const uint_type,        const unsigned_bigint& ) noexcept;
    friend bool operator>=(const unsigned_bigint&, const unsigned_bigint& ) noexcept;
    friend bool operator!=(const unsigned_bigint&, const uint_type        ) noexcept;
    friend bool operator!=(const uint_type,        const unsigned_bigint& ) noexcept;
    friend bool operator!=(const unsigned_bigint&, const unsigned_bigint& ) noexcept;

    // others
    int compare(const unsigned_bigint&) const noexcept;

    unsigned_bigint  add(const unsigned_bigint&      ) const;
    unsigned_bigint  add(const uint_type             ) const;
    unsigned_bigint& add_eq(const unsigned_bigint&   );
    unsigned_bigint& add_eq(const uint_type          );

    unsigned_bigint  sub(const unsigned_bigint&      ) const;
    unsigned_bigint  sub(const uint_type             ) const;
    unsigned_bigint& sub_eq(const unsigned_bigint&   );
    unsigned_bigint& sub_eq(const uint_type          );

    unsigned_bigint  multi(const unsigned_bigint&    ) const;
    unsigned_bigint  multi(const uint_type           ) const;
    unsigned_bigint& multi_eq(const unsigned_bigint& );
    unsigned_bigint& multi_eq(const uint_type        );

    unsigned_bigint  div(const unsigned_bigint&      ) const;
    unsigned_bigint  div(const uint_type             ) const;
    unsigned_bigint& div_eq(const unsigned_bigint&   );
    unsigned_bigint& div_eq(const uint_type          );

    unsigned_bigint  mod(const unsigned_bigint&      ) const;
    unsigned_bigint  mod(const uint_type             ) const;
    unsigned_bigint& mod_eq(const unsigned_bigint&   );
    unsigned_bigint& mod_eq(const uint_type          );

    std::pair<unsigned_bigint, unsigned_bigint> div_mod(const unsigned_bigint&) const;
    std::pair<unsigned_bigint, unsigned_bigint> div_mod(const uint_type)        const;

    void swap(unsigned_bigint&) noexcept;
    size_type size() const noexcept;
    std::string to_string(bool reverse = false) const;
    ~unsigned_bigint() noexcept;

    friend std::hash<unsigned_bigint>;
}; // unsigned_bigint

void swap(unsigned_bigint&, unsigned_bigint&) noexcept;

} // namespace kedixa

// specialize std::hash<kedixa::unsigned_bigint>
namespace std {
template<> struct hash<kedixa::unsigned_bigint>
{
    size_t operator()(const kedixa::unsigned_bigint &ubigint) const
    {
        size_t hash_code = 0;
        auto h = std::hash<kedixa::unsigned_bigint::uint_type>();
        for(auto &x : ubigint.digits)
            hash_code ^= h(x);
        return hash_code;
    }
};

} // namespace std

#endif // KEDIXA_UNSIGNED_BIGINT_H
