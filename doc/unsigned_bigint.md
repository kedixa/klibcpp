# 无符号大整数 unsigned_bigint

## 简介
无符号大整数类是一个支持无符号大整数运算的类，主要支持与十进制字符串之间的转换，加、减、乘、除、模等四则运算，逻辑移位、与、或、异或等位操作，由于按位取反在无符号大整数中没有实际意义，因此没有实现。各种运算的含义均与unsigned int的运算具有相似的意义，略有不同的地方将在下文指出。

## unsigned_bigint

### 类型定义
```cpp
using size_type   = size_t;
using uint_type   = uint32_t;
using ull_type    = uint64_t;
using llint_type  = int64_t;
```

### 构造函数
```cpp
unsigned_bigint(const uint_type number = 0 );
unsigned_bigint(const vector<uint_type>&   );
unsigned_bigint(vector<uint_type>&&        );
unsigned_bigint(const std::string&         );
unsigned_bigint(const char*                );
unsigned_bigint(const unsigned_bigint&     );
unsigned_bigint(unsigned_bigint&&          );
```

### 赋值运算符
```cpp
unsigned_bigint& operator=  (const unsigned_bigint& );
unsigned_bigint& operator=  (unsigned_bigint&&      );
unsigned_bigint& operator+= (const uint_type        );
unsigned_bigint& operator+= (const unsigned_bigint& );
unsigned_bigint& operator-= (const uint_type        );
unsigned_bigint& operator-= (const unsigned_bigint& );
unsigned_bigint& operator*= (const uint_type        );
unsigned_bigint& operator*= (const unsigned_bigint& );
unsigned_bigint& operator/= (const uint_type        );
unsigned_bigint& operator/= (const unsigned_bigint& );
unsigned_bigint& operator%= (const uint_type        );
unsigned_bigint& operator%= (const unsigned_bigint& );
unsigned_bigint& operator&= (const unsigned_bigint& );
unsigned_bigint& operator|= (const unsigned_bigint& );
unsigned_bigint& operator^= (const unsigned_bigint& );
unsigned_bigint& operator<<=(const size_type        );
unsigned_bigint& operator>>=(const size_type)noexcept;
```
注：当减法向下溢出时，抛出std::underflow_error 异常；当除数为零时，抛出std::runtime_error 异常。当减法向下溢出时，被减数的状态是未知的。  

### 类型转换函数
```cpp
explicit operator bool()      const noexcept;
explicit operator uint_type() const noexcept;
explicit operator ull_type()  const noexcept;

uint_type to_uint() const;
ull_type  to_ull()  const;
long double to_ld() const;
```
当数为零时，operator bool()返回假，否则返回真。operator uint_type() 返回与 unsigned_bigint::uint_type 字长相同的整数，当溢出时整数被截断，operator ull_type() 返回与unsigned_bigint::ull_type 字长相同的整数，当溢出时截断。  
to_uint() 返回与unsigned_bigint::uint_type 字长相同的整数，当溢出时抛出 std::runtime_error 异常，to_ull() 返回与unsigned_bigint::ull_type 字长相同的整数，当溢出时抛出 std::runtime_error 异常，to_ld() 返回大整数的长浮点数表示，当溢出时抛出 std::runtime_error 异常。

### 算术运算符
加、减、乘、除、模等运算，包括无符号大整数与无符号大整数、无符号大整数与C++内置无符号整数之间的运算，以加法为例：  
```cpp
friend unsigned_bigint operator+ (const unsigned_bigint&, const unsigned_bigint& );
friend unsigned_bigint operator+ (const unsigned_bigint&, unsigned_bigint&&      );
friend unsigned_bigint operator+ (unsigned_bigint&&,      const unsigned_bigint& );
friend unsigned_bigint operator+ (unsigned_bigint&&,      unsigned_bigint&&      );
friend unsigned_bigint operator+ (const unsigned_bigint&, const uint_type        );
friend unsigned_bigint operator+ (const uint_type,        const unsigned_bigint& );
friend unsigned_bigint operator+ (unsigned_bigint&&,      const uint_type        );
friend unsigned_bigint operator+ (const uint_type,        unsigned_bigint&&      );
```
注：当减法向下溢出时，抛出std::underflow_error 异常；当除数为零时，抛出std::runtime_error 异常。  
还包括前置自增自减以及后置自增自减运算符。  

### 位运算
```cpp
friend unsigned_bigint operator<<(const unsigned_bigint&, size_type              );
friend unsigned_bigint operator<<(unsigned_bigint&&,      size_type              );
friend unsigned_bigint operator>>(const unsigned_bigint&, size_type              );
friend unsigned_bigint operator>>(unsigned_bigint&&,      size_type              );
friend unsigned_bigint operator& (const unsigned_bigint&, const unsigned_bigint& );
friend unsigned_bigint operator& (const unsigned_bigint&, unsigned_bigint&&      );
friend unsigned_bigint operator& (unsigned_bigint&&,      const unsigned_bigint& );
friend unsigned_bigint operator& (unsigned_bigint&&,      unsigned_bigint&&      );
friend unsigned_bigint operator| (const unsigned_bigint&, const unsigned_bigint& );
friend unsigned_bigint operator| (const unsigned_bigint&, unsigned_bigint&&      );
friend unsigned_bigint operator| (unsigned_bigint&&,      const unsigned_bigint& );
friend unsigned_bigint operator| (unsigned_bigint&&,      unsigned_bigint&&      );
friend unsigned_bigint operator^ (const unsigned_bigint&, const unsigned_bigint& );
friend unsigned_bigint operator^ (const unsigned_bigint&, unsigned_bigint&&      );
friend unsigned_bigint operator^ (unsigned_bigint&&,      const unsigned_bigint& );
friend unsigned_bigint operator^ (unsigned_bigint&&,      unsigned_bigint&&      );
```
位运算的结果与内置无符号整数位运算具有相同的定义。

### 关系运算符
包括大于、大于等于、小于、小于等于、等于、不等于六种关系运算符，与内置无符号整数的关系运算具有相同的定义。

### 其它函数
```cpp
int compare(const unsigned_bigint&) const noexcept;

unsigned_bigint  add(const unsigned_bigint&      ) const;
unsigned_bigint  add(const uint_type             ) const;
unsigned_bigint& add_eq(const unsigned_bigint&   );
unsigned_bigint& add_eq(const uint_type          );

unsigned_bigint  sub(const unsigned_bigint&      ) const;
unsigned_bigint  sub(const uint_type             ) const;
unsigned_bigint& sub_eq(const unsigned_bigint&   );
unsigned_bigint& sub_eq(const uint_type          );

unsigned_bigint  multi(const unsigned_bigint&    ) const;
unsigned_bigint  multi(const uint_type           ) const;
unsigned_bigint& multi_eq(const unsigned_bigint& );
unsigned_bigint& multi_eq(const uint_type        );

unsigned_bigint  div(const unsigned_bigint&      ) const;
unsigned_bigint  div(const uint_type             ) const;
unsigned_bigint& div_eq(const unsigned_bigint&   );
unsigned_bigint& div_eq(const uint_type          );

unsigned_bigint  mod(const unsigned_bigint&      ) const;
unsigned_bigint  mod(const uint_type             ) const;
unsigned_bigint& mod_eq(const unsigned_bigint&   );
unsigned_bigint& mod_eq(const uint_type          );

std::pair<unsigned_bigint, unsigned_bigint> div_mod(const unsigned_bigint&) const;
std::pair<unsinged_bigint, unsigned_bigint> div_mod(const uint_type       ) const;

void swap(unsigned_bigint&) noexcept;
size_t size() const noexcept;
std::string to_string() const;
```
- compare函数将无符号大整数与另一个无符号大整数相比较，返回值<0、==0、>0分别表示*this <、==、>另一个无符号大整数。  
- add、sub、multi、div等函数与四则运算相同，div_mod用于同时返回商和余数。  
- swap函数交换两个无符号大整数，时间复杂度为常数。  
- size返回内部std::vector的size。  
- to_string将无符号大整数转换成字符串。  

### 析构函数
```cpp
~unsigned_bigint() noexcept;
```

### 哈希函数
可以通过C++提供的哈希类获取一个无符号大整数的哈希值，以便于将无符号大整数放置于标准库中的无序容器中。
