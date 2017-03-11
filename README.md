# klibcpp

klibcpp (Kedixa's Cplusplus Library)

# Content

- timer: 计时器
- multiarray: 多维数组
- unsigned_bigint: 无符号大整数类
- bigint: 带符号大整数类

# Compile and Run

1. 编译环境：需要带有g++(4.8或更高版本)编译器的编译环境，例如Linux、"Windows Subsystem for Linux"、MinGW、Cygwin等。  
注：代码暂时仅在WSL下编译通过，MinGW的某些版本会报"to_string is not a member of std"，这是MinGW的一个BUG。  

2. 将代码克隆到本地，并将路径切换到klibcpp目录下，执行以下命令：  
    ```
    make
    ```
该命令用于编译类库和函数库，并在lib目录下生成libklibcpp.a 和 libklibcpp.so。默认情况下，使用g++编译器来执行编译，如果系统中包含多个版本的编译器，可以借助以下方式更换编译器版本（例如g++-6）：  
    ```
    make CXX=g++-6
    ```

3. 代码测试：进入test文件夹，执行以下命令(第二条表示指定编译器版本的命令)：  
    ```
    make check
    make CXX=g++-6 check
    ```
即使check成功，也只能说明在有限的测试样例下代码无误，若读者发现代码中有错误或不足，欢迎指出或探讨。  

4. 应用：阅读doc目录下的说明可以发现相应类和函数的功能，查看test目录下的代码可以快速理解如何使用类和函数。  
将类和函数应用到自己的代码中，需要包含相应的头文件，并在编译指令中采用以下方式：  
    ```
    g++ -std=c++11 -I klibcpp/include main.cpp klibcpp/lib/klibcpp.a -o main.out
    ```

# LICENSE

Apache License 2.0
