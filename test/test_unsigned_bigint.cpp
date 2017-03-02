#include <iostream>
#include <cassert>
#include <string>
#include <random>
#include <unordered_set>
#include <exception>
#include "unsigned_bigint.h"
using namespace std;
using ubig = kedixa::unsigned_bigint;

ubig zero, one(1), two(2), ten(10), hundred(100);
ubig b1(123456789), b2(987654321), b3(4294967295);
random_device rd;

void check();

ubig fast_pow(const ubig &b, int p)
{
    ubig x(1), y(b);
    while(p>0)
    {
        if(p&1) x*=y;
        y *= y;
        p>>=1;
    }
    return x;
}
int main()
{
    check();
    return 0;
}

void check_to_string()
{
    assert(zero.to_string() == "0");
    assert(one.to_string() == "1");
    assert(two.to_string() == "2");
    assert(ten.to_string() == "10");
    assert(hundred.to_string() == "100");
    for(size_t i = 0; i < 100; ++i)
    {
        unsigned tmp = rd();
        assert(ubig(tmp).to_string() == to_string(tmp));
    }
    assert(b1.to_string() == "123456789");
    assert(b3.to_string() == "4294967295");
    assert(ubig(vector<unsigned>({0x1AD98412, 0x329087EB})).to_string() == "3643561542380258322");
    assert(ubig(string(100, '8')).to_string() == string(100, '8'));
}

void check_relation()
{
    assert(zero == zero);
    assert(zero < one);
    assert(123 > hundred);
    assert(hundred == hundred);
    assert(b3 >= b2);
    assert(b1 <= b3);
    assert(b1 != 666);
    assert(hundred.compare(ten) > 0);
    assert(hundred.compare(hundred) == 0);
}

void check_add_sub()
{
    assert((zero + zero).to_string() == "0");
    assert((zero + one).to_string() == "1");
    assert((one + one).to_string() == "2");
    assert((one + ten).to_string() == "11");
    assert((ten + hundred).to_string() == "110");
    assert((b1 + b2).to_string() == "1111111110");
    assert((b1 + b3).to_string() == "4418424084");
    ubig ub1("1234567890987654321"), ub2("9876543210123456789"), ub3("11111111101111111110");
    assert(ub1 + ub2 == ub3);
    assert(ub3 - ub1 == ub2);
    assert(ub3 - ub2 == ub1);
    assert(ub3 - hundred == ub3 - 100);
    assert(hundred - ten == 90);
    ubig ub4(12345);
    assert(++ub4 == 12346);
    assert(ub4-- == 12346);
    assert(ub4++ == 12345);
    assert(--ub4 == 12345);
    bool e = false;
    try { ubig ub5 = ten - hundred; }
    catch(exception &ex) { e = true; }
    assert(e);
}

void check_multi_div()
{
    ubig ub1 = b1 * b2 * b3;
    assert(ub1.to_string() == "523696662822067941618527355");
    assert(ub1 / b1 == b2 * b3);
    assert(ub1 / b2 == b1 * b3);
    assert(ub1 / b3 == b1 * b2);
    assert(ub1 * ub1 / ub1 == ub1);
    assert(ub1 * b1 == b1 * b1 * b2 * b3);
    assert(ub1 / ub1 == 1);
    bool e = false;
    try { ubig ub2 = ub1 / zero; }
    catch(exception &ex) { e = true; }
    assert(e);
    ubig ub3 = one;
    for(size_t i = 0; i < 100; ++i)
        ub3 *= 1234567;
    assert(ub3 == fast_pow(ubig(1234567), 100));
    ubig ub4(string(12345, '4')), ub5(string(1234, '5'));
    assert(ub5 * ub4 / ub5 == ub4);
}

void check_hash()
{
    unordered_set<ubig> st;
    st.insert(zero);
    st.insert(one);
    st.insert(two);
    st.insert(ten);
    st.insert(hundred);
    assert(st.find(zero) != st.end());
}

void check_convert()
{
    assert(hundred.to_uint() == 100);
    unsigned long long ull = 31415926535897ULL;
    ubig ub1(std::to_string(ull));
    assert(ub1.to_ull() == ull);

    bool e = false;
    try { ub1.to_uint(); }
    catch(exception &ex) { e = true; }
    assert(e);

    long double d = ub1.to_ld();
    static_cast<void>(d);

    e = false;
    int ldsz = std::numeric_limits<long double>::max_exponent + 2;
    ubig ub2 = ubig(1u) << ldsz;
    try { ub2.to_ld(); }
    catch(exception &ex) { e = true; }
    assert(e);

    assert(abs(ub1.to_ld()-ull) < 1e-7);
}

void check_others()
{
    assert((one ^ hundred) == (1 ^ 100));
    assert((ten | hundred) == (10 | 100));
    assert((two & ten) == (2 & 10));
    assert((b3 << 5).to_string() == to_string(4294967295LL << 5));
    assert(b3 >> 5 == 4294967295U >> 5);
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

