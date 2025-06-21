#include "io_optimizer.hpp"
#include <chrono>
#include <vector>
#include <ranges>

using namespace std::chrono;

void run_benchmark() {
    constexpr size_t N = 10'000'000;
    std::vector<int> data(N);
    
    // 填充测试数据
    auto gen = [i = 0]() mutable { return i++ % 100; };
    std::ranges::generate(data, gen);
    
    HyperIO io;
    
    // I/O性能测试
    auto t1 = high_resolution_clock::now();
    io.read_range(data);
    auto t2 = high_resolution_clock::now();
    
    // 范围视图性能测试
    auto t3 = high_resolution_clock::now();
    auto result = data | std::views::transform([](int x) { return x*x; })
                      | std::views::filter([](int x) { return x % 2 == 0; });
    auto t4 = high_resolution_clock::now();
    
    // 强制计算视图结果
    volatile auto force_eval = std::ranges::distance(result);
    
    // 输出结果
    io.fmt_print("Benchmark Results:\n"
                 "- I/O Throughput: {:.2f} MB/s\n"
                 "- Range View Latency: {} ms\n",
                 (N * sizeof(int)) / (duration_cast<microseconds>(t2-t1).count() / 1e6) / 1e6,
                 duration_cast<milliseconds>(t4-t3).count());
}

int main() {
    run_benchmark();
    return 0;
}