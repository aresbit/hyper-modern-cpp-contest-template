#pragma once
#include <iostream>
#include <format>
#include <ranges>
#include <execution>

struct HyperIO {
    HyperIO() { 
        std::ios::sync_with_stdio(false); 
        std::cin.tie(nullptr); 
        std::cout.setf(std::ios::unitbuf); 
    }
    
    template<typename T>
    T scan() { T x; std::cin >> x; return x; }
    
    template<std::ranges::input_range R>
    void read_range(R&& r) {
        std::for_each(std::execution::par_unseq, 
            std::ranges::begin(r), std::ranges::end(r), 
            [](auto& elem) { std::cin >> elem; });
    }
    
    template<typename... Args>
    void fmt_print(std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::vformat(fmt.get(), std::make_format_args(args...));
    }
};