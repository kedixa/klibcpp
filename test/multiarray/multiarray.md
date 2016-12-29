# 多维数组 multi array

## 简介

multiarray 是一个多维数组，位于 src/multiarray.h ，用于方便地定义和使用多维度的动态数组，
数组的每个维度的长度是在运行时确定的，一旦确定便不可更改（否则就成了std::vector），
且数组的维度必须是一个编译器常量。

## multiarray
```cpp
template<typename T, unsigned N, typename Alloc = std::allocator<T>>  
class multiarray;
```

### 模板参数
- T: 数组要保存的元素的类型
- N: 数组的维度，无符号整数
- Alloc: 空间分配器，默认为 std::allocator<T>

### 成员函数
#### 构造函数
```cpp
template<typename... Args>
multiarray(size_type sz, Args&&... args);

multiarray(multiarray&& ma);

multiarray(const multiarray&) = delete;
```
1. 使用每个维度的长度构造数组，剩余的参数用于构造对象；
2. 使用另一个多维数组进行移动构造；
3. 不支持拷贝构造。

Example:
```cpp
// 构造一个 2 * 3 * 4 的整型数组，并用 9 初始化
multiarray<int, 3> int_array(2, 3, 4, 9);

// 构造一个 100 的双精度浮点数数组，并用 3.14 初始化
multiarray<double, 1> double_array(100, 3.14);
```

#### operator=
```cpp
multiarray& operator=(multiarray&& ma);
multiarray& operator=(const multiarray&) = delete;
```
1. 使用另一个多维数组进行移动赋值；
2. 不支持拷贝赋值。

Example:
```cpp
multiarray<int, 2> array1(3, 3);
multiarray<int, 2> array2(0, 0);
array2 = std::move(array1);
// array1 no longer usable.
```

#### 迭代器
```cpp
begin() // 第一个元素的迭代器
end() // 最后一个元素之后的迭代器
cbegin() // 常量迭代器
cend() // 常量迭代器

// Example:
multiarray<int, 1> int_array(100);
for(auto &i : int_array)
    i = 0;
```

#### 容量
```cpp
constexpr unsigned dimension() const; // 返回当前数组的维度
size_type size() const; // 返回当前维度的长度

Example:
multiarray<int, 3> int_array(4, 5, 6);
int_array.dimension() == 3;
int_array.size() == 4;

int_array[0].dimension() == 2;
int_array[0].size() == 5;
```

#### 析构函数
```cpp
~multiarray() // 析构所有对象并释放空间
```
