# 计时器 timer

用于计算程序中某些步骤耗费的时间。

## 静态函数
- int64_t timeit(f, args...) 计算函数f的运行时间，返回使用的纳秒数
- int64_t monotonic_nsec()
- int64_t monotonic_usec()
- int64_t monotonic_msec()

## 成员函数
- timer() 构造计时器对象，并开始计时
- void start() 开始计时，或重新开始计时
- void pause() 暂停计时
- void reset() 重置计时器，与构造时状态一致
- double stop()  停止计时，并返回当前累计的时间（秒）
- int64_t as_nsec() 返回当前累计的纳秒数
- int64_t as_usec() 返回当前累计的微秒数
- int64_t as_msec() 返回当前累计的毫秒数
