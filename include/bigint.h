/*
 * This file is part of klibcpp.
 * bigint.h - Signed big int class.
 * 
 * License: Apache 2.0
 * Read the Readme.md here for more infomation:
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2016-2017 kedixa(kedixa@outlook.com)
 *
 */

#ifndef KEDIXA_BIGINT_H
#define KEDIXA_BIGINT_H

#include "unsigned_bigint.h"

namespace kedixa {

class bigint
{
public:
    using size_type  = size_t;
    using uint_type  = uint32_t;
    using int_type   = int32_t;
    using ull_type   = uint64_t;
    using llint_type = int64_t;
private:
    unsigned_bigint ubig;
    bool sign;              // 0 for +, 1 for -

public:
    // constructors
    bigint(const int_type number = 0                 );
    bigint(const unsigned_bigint&, bool sign = false );
    bigint(unsigned_bigint&&, bool sign = false      );
    explicit
    bigint(const std::string&                        );
    explicit
    bigint(const char*                               );
    bigint(const bigint&                             );
    bigint(bigint&&                                  );

    // assignment operators
    bigint& operator=  (const bigint&  );
    bigint& operator=  (bigint&&       );
    bigint& operator+= (const int_type );
    bigint& operator+= (const bigint&  );
    bigint& operator-= (const int_type );
    bigint& operator-= (const bigint&  );
    bigint& operator*= (const int_type );
    bigint& operator*= (const bigint&  );
    bigint& operator/= (const int_type );
    bigint& operator/= (const bigint&  );
    bigint& operator%= (const int_type );
    bigint& operator%= (const bigint&  );

    // conversion operators
    explicit operator bool()            const noexcept;
    explicit operator int_type()        const noexcept;
    explicit operator llint_type()      const noexcept;
    explicit operator unsigned_bigint() const noexcept;

    // conversion functions
    int_type to_int()            const ;
    llint_type  to_ll()          const ;
    long double to_ld()          const ;
    unsigned_bigint to_ubigint() const ;

    // arithmetic operators
    friend bigint operator+ (const bigint&, const bigint&);
    friend bigint operator- (const bigint&, const bigint&);
    friend bigint operator* (const bigint&, const bigint&);
    friend bigint operator/ (const bigint&, const bigint&);
    friend bigint operator% (const bigint&, const bigint&);

    bigint& operator++();
    bigint& operator--();
    bigint  operator++(int);
    bigint  operator--(int);
    bigint  operator+ () const;
    bigint  operator- () const;

    // bitwise operators
    friend bigint operator<<(const bigint&, size_type);
    friend bigint operator>>(const bigint&, size_type);

    // relational operators
    friend bool operator< (const bigint&, const bigint&) noexcept;
    friend bool operator==(const bigint&, const bigint&) noexcept;
    friend bool operator<=(const bigint&, const bigint&) noexcept;
    friend bool operator> (const bigint&, const bigint&) noexcept;
    friend bool operator>=(const bigint&, const bigint&) noexcept;
    friend bool operator!=(const bigint&, const bigint&) noexcept;

    // others
    int compare(const bigint&      ) const noexcept;

    bigint  add(const bigint&      ) const;
    bigint& add_eq(const bigint&   ) ;
    bigint  sub(const bigint&      ) const;
    bigint& sub_eq(const bigint&   ) ;
    bigint  multi(const bigint&    ) const;
    bigint& multi_eq(const bigint& ) ;
    bigint  div(const bigint&      ) const;
    bigint& div_eq(const bigint&   ) ;
    bigint  mod(const bigint&      ) const;
    bigint& mod_eq(const bigint&   ) ;

    std::pair<bigint, bigint> div_mod(const bigint&) const;

    void inverse()      noexcept; // *this = - *this;
    void swap(bigint &) noexcept;
    size_t size() const noexcept;
    std::string to_string(bool reverse = false) const;
    ~bigint()           noexcept;

    friend std::hash<bigint>;
}; // class bigint

void swap(bigint&, bigint&) noexcept;

} // namespace kedixa

// specialize std::hash<kedixa::bigint>
namespace std {
template<> struct hash<kedixa::bigint>
{
    size_t operator()(const kedixa::bigint &big) const
    {
        size_t hash_code = hash<kedixa::unsigned_bigint>()(big.ubig);
        hash_code ^= 0xAAAA5555;
        return hash_code;
    }
};

} // namespace std

 #endif // KEDIXA_BIGINT_H
