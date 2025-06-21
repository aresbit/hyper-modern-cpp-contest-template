#!/bin/bash

# 激进编译参数
FLAGS="-std=c++20 -O3 -march=native -fno-exceptions -fno-rtti"

# 检测编译器
if command -v clang++-18 &> /dev/null; then
    COMPILER="clang++-18 -stdlib=libc++"
elif command -v g++-14 &> /dev/null; then
    COMPILER="g++-14"
else
    echo "ERROR: Requires Clang 18+ or GCC 14+"
    exit 1
fi

# 创建输出目录
mkdir -p bin

# 编译主程序
$COMPILER $FLAGS -I./include src/main.cpp -o bin/solver

# 编译性能测试
$COMPILER $FLAGS -I./include test/benchmark.cpp -o bin/benchmark

echo "Build complete! Executables in bin/"