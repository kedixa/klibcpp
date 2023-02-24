#include <iostream>
#include <cassert>
#include <string>
#include <random>
#include <unordered_set>
#include <exception>

#include "kedixa/bigint.h"

using namespace std;
using bigint = kedixa::bigint;

bigint zero, one(1), ten(10), neg_one(-1), neg_ten(-10);
bigint three(3), neg_three(-3);
bigint int_max(2147483647), int_min(-2147483648);
random_device rd;

void check();

int main()
{
    check();
    return 0;
}

void check_to_string()
{
    assert(zero.to_string() == "0");
    assert(three.to_string() == "3");
    assert(neg_three.to_string() == "-3");
    assert(int_max.to_string() == "2147483647");
    assert(int_min.to_string() == "-2147483648");
    string s = "-2347298572398475918743982758937429576298437529834";
    assert(bigint(s).to_string() == s);
}

void check_relation()
{
    assert(zero == zero);
    assert(zero < one);
    assert(neg_one < one);
    assert(neg_one > neg_ten);
    assert(int_max != int_min);
    assert(three <= three);
    assert(ten >= int_min);
}

void check_add_sub()
{
    assert(zero + zero == zero);
    assert(three + neg_three == zero);
    assert(int_max + int_min == neg_one);
    assert(three - neg_three == 6);
    bigint b1 = neg_one;
    assert(++b1 == zero);
    assert(--b1 == neg_one);
    assert(--b1 == -2);
    b1 = zero;
    assert(++b1 == one);
}

void check_multi_div()
{
    assert(neg_one * zero == zero);
    assert(zero * ten == 0);
    assert(ten * neg_three == -30);
    assert(neg_three * neg_one == three);
    assert(ten / three == three);
    assert(ten % three == one);
    assert(neg_ten / neg_one == ten);
    assert(neg_ten % neg_one == zero);
    assert(ten / neg_three == -4);
    assert(ten % neg_three == -2);
    assert(int_min / neg_one == int_max + 1);
}

void check_hash()
{
    unordered_set<bigint> st;
    st.insert(zero);
    st.insert(neg_one);
    st.insert(three);
    st.insert(neg_ten);
    st.insert(int_max);
    assert(st.find(zero) != st.end());
}

void check_convert()
{
    assert(neg_one.to_int() == -1);
    assert(int_max.to_int() == 2147483647);
    assert(int_min.to_int() == -2147483648);
    long double d = int_min.to_ld();
    static_cast<void>(d);
    assert(neg_one.to_ubigint() == 1u);
}

void check_others()
{
    assert((one << 4) == 16);
    assert((neg_one << 4) == -16);
    assert((neg_ten >> 12) == neg_one);
    assert((ten >> 12) == zero);
    assert(bigint(-7) >> 1 == -4);
    assert(bigint(-9) >> 2 == -3);
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
