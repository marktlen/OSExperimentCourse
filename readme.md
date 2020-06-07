# Introduction

Experiment in OS course and my OS homework

## file tree

Experiment 1

- Experiment 1.1 [version 1](1_1v1.cpp) [version 2](1_1v2.cpp)
  - 将一个类的一个对象序列化到文件
- [Experiment 1.2](1_2v1.cpp)
  - 一个类的多个对象序列化到文件
- [Experiment 1.3](1_3v1.cpp)
  - 两个不同类的多个对象序列化到文件
  - 中间变量
- [Experiment 1.4](1_4v1.cpp)
  - 不同类的多个对象序列化到文件
  - 接口类实现

Experiment 2

- [Experiment 2.1](/2_1)
  - 使用动态链接库实现打印功能
- [Experiment 2.2](/2_2)
  - 一种打印功能用一个动态链接库（插件）实现
  - 将这些 插件放置在固定的目录中，例如当前的plugin目录
  - 遍历这个目录，获取所有动态链接库
- [Experiment 2.3](/2_3)
  - 通过命令行方式：./a.out help，输出所有插件实现的功能ID，以及该功能ID对应的功能描述
  - 通过命令行方式：./a.out FuncID，调用具体打印功能
- [Experiment 2.4](/2_4)
  - 将三个导出函数都放在一个类中，让插件外部获取该类的对象
- [Experiment 2.5](/2_5)
  - 用虚接口实现插件调用和实例化

HomeWork

- [HomeWork1](HomeWork/hw1.cpp)
  - 当设定O_APPEND标志后，能否使用lseek函数修改当前文件偏移量？若能修改，读写操作发生在文件何处
- [HomeWork2](HomeWork/hw2.cpp)
  - 给定一个目录文件，打印该目录下所有子目录和文件的名称，并不断递归访问子目录，并打印其属子目录和文件名称
- [HomeWork3](HomeWork/hw3.cpp)
  - 实现“ls -l”的基本功能，至少能输出文件类型、9个权限位信息、文件大小、文件名称

## Common console commands

动态库的编写

```bash
g++ -fpic -shared -o libtest.so a1.cpp a2.cpp
```

生成libtest.so文件
动态库使用者的编译

```bash
g++ -o test test.cpp –ldl
```
