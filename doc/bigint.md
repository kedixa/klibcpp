# 带符号大整数 bigint

## 简介 
带符号大整数类是一个基于无符号大整数类实现的大整数运算类，主要支持与十进制字符串之间的转换，加、减、乘、除、模等运算，移位操作，前（后）置自增运算符。由于几种位运算暂时没有实际意义，且不经常使用（如果有这样的需求，std::bitset更合适），暂时未实现。各种运算的含义基本与int类型一致。

## bigint

### 类型定义
```cpp
using size_type  = size_t;
using uint_type  = uint32_t;
using int_type   = int32_t;
using ull_type   = uint64_t;
using llint_type = int64_t;
```

### 构造函数
```cpp
bigint(const int_type number = 0                 );
bigint(const unsigned_bigint&, bool sign = false );
bigint(unsigned_bigint&&, bool sign = false      );
bigint(const std::string&                        );
bigint(const char*                               );
bigint(const bigint&                             );
bigint(bigint&&                                  );
```

### 赋值运算符
```cpp
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
```
注：当除数为零时，抛出std::runtime_error异常。

### 类型转换函数
```cpp
explicit operator bool()            const noexcept;
explicit operator int_type()        const noexcept;
explicit operator llint_type()      const noexcept;
explicit operator unsigned_bigint() const noexcept;

int_type to_int()            const ;
llint_type  to_ll()          const ;
long double to_ld()          const ;
unsigned_bigint to_ubigint() const ;
```
注：当数为零时，operator bool()返回false, 否则返回true。当数确实为一个int或long long可以容纳的数时，operator int_type() 和 operator llint_type() 返回正确的数，否则为未定义行为，而转换失败时 to_int() 和 to_ll() 则抛出std::runtime_error异常。operator unsigned_bigint() 和to_ubigint() 返回与数的绝对值相同的无符号整数。 to_ld() 返回大整数的浮点数表示，溢出时抛出 std::runtime_error异常。

### 算术运算符
```cpp
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
```
注：当除数为零时，抛出 std::runtime_error异常。

### 关系运算符
包括大于、大于等于、小于、小于等于、等于、不等于六种关系运算符，与内置整数的关系运算具有相同的定义。

### 其它函数
```cpp
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

void opposite()     noexcept;
void swap(bigint &) noexcept;
size_t size() const noexcept;
std::string to_string(bool reverse = false) const;
```
- compare 比较两个大整数，返回值<0、==0、>0分别表示*this <、==、 >另一个大整数。  
- swap 用于交换两个大整数，时间复杂度为常数。  
- size 返回无符号大整数的size()。   
- to_string 将大整数转换成字符串。  
- opposite 将*this 转换成相反数。

### 析构函数
```cpp
~bigint() noexcept;
```

### 哈希函数
可以通过C++提供的哈希类获取一个大整数的哈希值，以便于将大整数放置于标准库中的无序容器中。
