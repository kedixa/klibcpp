#include <iostream>
#include "timer.h"
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
    return 0;
}
