/*
 * test_multiarray.cpp - Test multiarray.
 *
 * Copyright (c) 2016 kedixa(kedixa@outlook.com)
 *
 */
#include <iostream>
#include <cassert>
#include "multiarray.h"
using namespace std;
int count_A = 0;
class A{
public:
    double d;
    A(double _d = 0.0)
    {
        d = _d;
        cout << this << " A()\n";
        ++count_A;
    }
    ~A()
    {
        cout << this << " ~A()\n";
        --count_A;
    }
};

template<unsigned N>
using int_array = kedixa::multiarray<int, N>;
template<unsigned N>
using A_array = kedixa::multiarray<A, N>;

void test_int()
{
    int_array<2> arr(3, 4);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 4; j++)
            arr[i][j] = i + j;
    assert(arr.size() == 3);
    assert(arr.dimension() == 2);
    
    auto &arr_ref = arr[0];
    assert(arr_ref.dimension() == 1);

    for(size_t i = 0; i < arr.size(); i++)
        assert(arr[i].size() == 4);
    for(size_t i = 0; i < arr.size(); ++i)
        for(size_t j = 0; j < arr[i].size(); ++j)
            assert(arr[i][j] == signed(i + j));
}

void test_A()
{
    {
        // Construct and destroy 3 * 2 * 1 times.
        cout << "Construct A_array(3, 2, 1).\n";
        A_array<3> arr(3, 2, 1, 0.5);
        assert(count_A == 6);
        assert(arr[2][1][0].d == 0.5);
        cout << "Destroy A_array(3, 2, 1).\n";
    }
    assert(count_A == 0);
    cout << endl;
    {
        // Move copy.
        cout << "Move construct A_array(2, 1).\n";
        auto f = [](){return A_array<2>(2, 1);};
        auto arr2 = f();
        assert(count_A == 2);
        assert(arr2.size() == 2);
        assert(arr2.dimension() == 2);

        cout << "Move assign A_array(2, 1).\n";
        decltype(arr2) arr3(0, 0);
        arr3 = std::move(arr2);
        assert(count_A == 2);
    }
    assert(count_A == 0);
    cout << endl;
}

int main()
{
    test_int();
    test_A();
    return 0;
}
