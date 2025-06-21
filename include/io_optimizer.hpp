#pragma once
#include <iostream>
#include <format>
#include <ranges>
#include <vector>
#include <thread>
#include <future>
#include <sstream>
#include <iterator>

struct HyperIO {
    HyperIO() { 
        std::ios::sync_with_stdio(false); 
        std::cin.tie(nullptr); 
        std::cout.setf(std::ios::unitbuf); 
    }
    
    template<typename T>
    T scan() { 
        T x; 
        std::cin >> x; 
        return x; 
    }
    
    template<typename T>
    void read_range(std::vector<T>& container, size_t n) {
        container.resize(n);
        
        std::string raw_input;
        raw_input.reserve(n * 15);
        std::copy_n(std::istreambuf_iterator<char>(std::cin),
                   n * 15, 
                   std::back_inserter(raw_input));
        
        const size_t thread_count = std::thread::hardware_concurrency();
        const size_t block_size = (n + thread_count - 1) / thread_count;
        
        std::vector<std::future<void>> tasks;
        for (size_t i = 0; i < thread_count; ++i) {
            tasks.emplace_back(std::async(std::launch::async, 
                [&, start = i * block_size] {
                    const size_t end = std::min(start + block_size, n);
                    std::string_view block(raw_input.data() + start * 15, 
                                          (end - start) * 15);
                    
                    std::stringstream ss;
                    for (size_t j = start; j < end; ++j) {
                        ss.str(block.substr((j - start) * 15, 15));
                        ss >> container[j];
                        ss.clear();
                    }
                }
            ));
        }
        
        for (auto& task : tasks) task.wait();
    }
    
    template<typename... Args>
    void fmt_print(std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::vformat(fmt.get(), 
                   std::make_format_args(args...));
    }
};