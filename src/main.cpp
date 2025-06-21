#include "io_optimizer.hpp"
#include <ranges>
#include <format>
#include <execution>

namespace vw = std::ranges::views;

constexpr auto transform_view = [](auto fn) {
    return vw::transform([fn](const auto& x) { 
        return std::invoke(fn, x); 
    });
};

int main() {
    HyperIO io;
    const int n = io.scan<int>();
    
    // 并行化范围视图处理
    auto data = vw::iota(0, n)
        | transform_view([](int i) { return i * i; })
        | vw::take_last(n/2)
        | vw::common;
    
    // 无缓冲实时输出
    io.fmt_print("Results:\n{}", 
        std::format("{::#x}", std::vector(data.begin(), data.end())));
    
    // 编译期校验
    static_assert(noexcept(io.print(data)), "IO must be noexcept");
}