#include <iostream>
#include <cassert>
#include <string>
#include <random>
#include <unordered_set>
#include <exception>

#include "kedixa/rational.h"

using namespace std;
using rational = kedixa::rational;
using ubigint = kedixa::unsigned_bigint;

const std::string sma = "111222333444555666777888999000";
ubigint uone(1), uthree(3), uten(10), uma(sma);
rational zero, one_third(uone, uthree), neg_one_third(uone, uthree, true);
rational one(uone), one_1(uone, uone), one_2(uthree, uthree), one_3(uma, uma);
rational one_ma(uone, uma), neg_one_ma(uone, uma, true);
rational ten_ma(uten, uma), neg_ten_ma(uten, uma, true);

void check();

int main()
{
    check();
    return 0;
}

void check_to_string()
{
    assert(zero.to_string() == "0");
    assert(one_third.to_string() == "1/3");
    assert(neg_one_third.to_string() == "-1/3");
    assert(one_3.to_string() == "1");
    assert(neg_one_ma.to_string() == "-1/" + sma);
    assert(neg_ten_ma.to_string() == "-1/" + sma.substr(0, sma.length() - 1));
}

void check_relation()
{
    assert(zero == zero);
    assert(zero < one);
    assert(zero >= neg_one_third);
    assert(neg_one_third < one);
    assert(one_1 == one_2 && one_2 == one_3);
    assert(ten_ma <= one);
    assert(ten_ma != neg_ten_ma);
}

void check_add_sub()
{
    assert(zero + zero == zero);
    assert(one_third + neg_one_third == zero);
    assert(ten_ma + neg_ten_ma == zero);
    assert(one + one_third == rational(ubigint(4), ubigint(3)));
    assert(one + one_third + one_1 + one_1 + neg_one_third == rational(uthree));
    assert(one_third - neg_one_third + one_third == one);
    assert(one + neg_one_third == one_third + one_third);
}

void check_multi_div()
{
    assert(zero * ten_ma == zero);
    assert(one * one_1 == one_2);
    assert(neg_ten_ma * one == -one * ten_ma);
    assert(one / neg_one_third == -(one / one_third));
    assert(ten_ma / neg_ten_ma == -one);
    assert(zero / ten_ma == zero);
}

void check_hash()
{
    unordered_set<rational> st;
    st.insert(zero);
    st.insert(one_third);
    st.insert(neg_ten_ma);
    st.insert(neg_one_ma);
    assert(st.find(neg_ten_ma) != st.end());
    assert(st.find(ten_ma) == st.end());
}

void check_convert()
{
    assert(!zero);
    assert(one);
    assert(neg_ten_ma);
    long double d = neg_ten_ma.to_ld();
    static_cast<void>(d);
    assert(one_third.to_ld() == 1.0L/3.0);
}

void check_others()
{
    assert(one_third.get_num() == uone);
    assert(neg_one_third.get_den() == uthree);
    assert(neg_ten_ma.get_sign() == true);
    assert(neg_one_third.num_size() == 1);
    assert(neg_one_third.to_decimal(3) == "-0.333");
    auto x = one_third;
    x.reciprocal();
    assert(x * one_third == one);

    // pi/2 = 1 + 1/3 + 1/3 * 2/5 + 1/3 * 2/5 * 3/7 + ...
    rational pi(ubigint(1));
    rational rat(ubigint(1));
    for(size_t i = 1; i < 350; ++i)
    {
        rat *= rational(ubigint(i), ubigint(2*i+1));
        pi += rat;
    }
    pi *= rational(ubigint(2));
    const string pi100 = "3.141592653589793238462643383279"
                        "5028841971693993751058209749445923"
                        "078164062862089986280348253421170679";
    assert(pi.to_decimal(100) == pi100);
}

void check()
{
    check_to_string();
    check_relation();
    check_add_sub();
    check_multi_div();
    check_hash();
    check_convert();
    check_others();
}
