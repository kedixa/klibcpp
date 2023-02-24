#include <iostream>
#include "kedixa/timer.h"

using namespace std;

int main()
{
    kedixa::timer t;
    for(int i = 0; i < 100000000; ++i);
    t.pause();
    // do some other things
    t.start();
    for(int i = 0; i < 100000000; ++i);
    cout << "It takes " << t.stop() << " seconds.\n";

    double d = kedixa::timer::timeit([](int n){
        for(int i = 0; i < n; i++) {}
    }, 1000);
    cout << "It takes " << d << " seconds.\n";
    return 0;
}
