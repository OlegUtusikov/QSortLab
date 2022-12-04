//
// Created by utusikov on 04.12.22.
//

#pragma once

#include <array>
#include <algorithm>
#include <string>
#include <cxxabi.h>

namespace utils {
    template<size_t SZ, typename T, typename Cmp>
    std::pair<T, size_t> median(std::array<std::pair<T, size_t>, SZ> a, Cmp cmp = Cmp()) {
        static_assert(SZ >= 3, "Bad count iof elements in median!!!");
        std::nth_element(a.begin(), a.begin() + SZ / 2U, a.end(), std::move(cmp));
        return a[SZ / 2U];
    }

    inline std::string demangle(const char *raw_name) {
        int status = 0;
        char *real_name = abi::__cxa_demangle(raw_name, nullptr, nullptr, &status);
        std::string res_name = (status == 0) ? real_name : raw_name;
        std::free(real_name);
        return res_name;
    }
    template<typename T>
    std::string demangle() {
        return demangle(typeid(T).name());
    }

}   // namespace utils
