# Introduction

Experiment in OS course and my OS homework

## file tree

Experiment 1

- Experiment 1.1 [version 1](1_1v1.cpp) [version 2](1_1v2.cpp)
- [Experiment 1.2](1_2v1.cpp)
- [Experiment 1.3](1_3v1.cpp)
- [Experiment 1.4](1_4v1.cpp)

Experiment 2

- [Experiment 2.1](/2_1)
- [Experiment 2.2](/2_2)
- [Experiment 2.3](/2_3)

HomeWork

- [HomeWork1](HomeWork/hw1.cpp)
- [HomeWork2](HomeWork/hw2.cpp)
- [HomeWork3](HomeWork/hw3.cpp)

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
