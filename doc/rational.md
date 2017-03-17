# 有理数类 rational

## 简介
有理数类是一个基于无符号大整数类实现的有理数运算类，由于每个有理数恰好可以表示为一个最简分数，因此采用两个无符号大整数以及一个符号位即可表示有理数。 rational 有理数类支持四则运算，关系运算等。 各种运算的含义与分数的运算一致。

## rational

### 类型定义
```cpp
using size_type = unsigned_bigint::size_type;
using uint_type = unsigned_bigint::uint_type;
using int_type  = unsigned_bigint::int_type;
using ull_type  = unsigned_bigint::ull_type;
using llint_type= unsigned_bigint::llint_type;
using ubigint   = unsigned_bigint;
```

### 构造函数
```cpp
rational(const ubigint &n = ubigint(0u), const ubigint &d = ubigint(1u), bool sign = false);
rational(ubigint&&, ubigint&&, bool);
rational(const rational&);
rational(rational&&);
```

### 赋值运算符
```cpp
rational& operator= (const rational&);
rational& operator= (rational&&);
rational& operator+=(const rational&);
rational& operator-=(const rational&);
rational& operator*=(const rational&);
rational& operator/=(const rational&);
```
注：当除数为零时，抛出std::runtime_error异常。

### 类型转换函数
```cpp
explicit operator bool() const noexcept;
long double to_ld() const;
```
注：当有理数为零时， operator bool()返回false, 否则返回true。 to_ld() 返回有理数的浮点数表示，若分母或分子不能表示成long double 类型，抛出 std::runtime_error异常。

### 算术运算符
```cpp
friend rational operator+ (const rational&, const rational&);
friend rational operator- (const rational&, const rational&);
friend rational operator* (const rational&, const rational&);
friend rational operator/ (const rational&, const rational&);

rational operator+ () const;
rational operator- () const;
```
注：当除数为零时，抛出std::runtime_error异常。

### 关系运算符
包括大于、大于等于、小于、小于等于、等于、不等于六种关系运算符，与分数的关系运算具有相同的定义。

### 其它函数
```cpp
int compare(const rational&) const;

rational  add   (const rational&) const;
rational& add_eq(const rational&);
rational  sub   (const rational&) const;
rational& sub_eq(const rational&);
rational  multi (const rational&) const;
rational& multi_eq(const rational&);
rational  div   (const rational&) const;
rational& div_eq(const rational&);

ubigint get_num() const; 
ubigint get_den() const; 
bool    get_sign() const; 
void approximate(size_type hint = 0);
void opposite() noexcept;  // *this = -*this
void reciprocal(); // *this = this->den/this->num
void swap(rational&) noexcept;
size_type num_size() const noexcept;
size_type den_size() const noexcept;
std::string to_string(bool reverse = false) const;
std::string to_decimal(size_type decimal_digits = 16, bool reverse = false) const;
```
- compare 比较两个有理数，返回值<0、==0、>0分别表示*this <、==、 >另一个有理数。  
- swap 用于交换两个有理数，时间复杂度为常数。  
- get_num 获取分子。  
- get_den 获取分母。  
- get_sign 获取符号位。  
- approximate 将分子分母同时至少缩小hint * pow(2, 8*sizeof(unsigned))倍， 获得其近似表示。  
- opposite 将*this 转换成相反数。  
- reciprocal 转换成倒数，若数为零则抛出std::runtime_error异常。  
- num_size 返回分子的size。  
- den_size 返回分母的size。  
- to_string 将有理数转换成分数表示的字符串，如1/2, -3/2, -7等。  
- to_decimal 将分数化为有decimal_digits 个小数位的小数表示，位数不足补零。  

### 析构函数
```cpp
~rational() noexcept;
```
### 哈希函数
可以通过C++提供的哈希类获取一个有理数的哈希值，以便于将有理数放置于标准库中的无序容器中。
