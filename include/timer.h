/*
 * This file is part of klibcpp.
 * timer.h - A High-precision timer.
 * 
 * License: Read the Readme.md here
 * https://github.com/kedixa/klibcpp/blob/master/README.md
 * 
 * Copyright (c) 2016 kedixa(kedixa@outlook.com)
 *
 */
#ifndef KEDIXA_TIMER_H
#define KEDIXA_TIMER_H

#include <chrono>
namespace kedixa {

class timer
{
    typedef std::chrono::steady_clock::time_point   tp;
    typedef std::chrono::duration<double>           dd;
    typedef std::chrono::steady_clock               sc;
public:
    template<typename F, typename... Args>
    static double timeit(F&& f, Args&&... args) {
        timer t;
        f(args...);
        return t.stop();
    }
private:
    tp _begin;
    dd _span;
public:
    timer()
        : _begin(sc::now()), _span(dd(0)){}
    void start()
    {
        _begin = sc::now();
    }
    void pause()
    {
        tp _end = sc::now();
        _span += std::chrono::duration_cast<dd>(_end - _begin);
    }
    void reset()
    {
        _begin = sc::now();
        _span  = dd(0);
    }
    double stop()
    {
        tp _end = sc::now();
        _span += std::chrono::duration_cast<dd>(_end - _begin);
        double time = _span.count();
        _span = dd(0);
        return time;
    }
    ~timer() {}
};

} // namespace kedixa
#endif // KEDIXA_TIMER_H
