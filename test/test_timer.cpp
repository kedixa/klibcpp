#include <iostream>
#include "kedixa/timer.h"

constexpr int N = 10000000;

void test01() {
    long long sum = 0;

    kedixa::timer t;
    for(int i = 0; i < N/2; ++i)
        sum += i;
    t.pause();

    // do some other things

    t.start();
    for(int i = N/2; i < N; ++i)
        sum += i;

    std::cout << "Sum = " << sum << std::endl;
    std::cout << "It takes " << t.stop() << " seconds," << std::endl;
    std::cout << "which is " << t.as_nsec() << " nanoseconds." << std::endl;
}

void test02() {
    long long sum = 0;

    int64_t nano = kedixa::timer::timeit([&sum](int n){
        for(int i = 0; i < n; i++)
            sum += i;
    }, N);

    std::cout << "Sum = " << sum << std::endl;
    std::cout << "It takes " << nano << " nanoseconds." << std::endl;
}

int main() {

    test01();
    test02();
    return 0;
}
