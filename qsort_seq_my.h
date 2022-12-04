//
// Created by utusikov on 04.12.22.
//

#pragma once

#include <algorithm>
#include <chrono>

#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/random.h>
#include "utils.h"

struct qsort_seq_my final {
    qsort_seq_my() = delete;
    ~qsort_seq_my() = delete;

    template<typename Range, typename Cmp = std::less<>>
    static auto sort(Range &input, Cmp cmp = Cmp()) {
        long long size = input.size();
        qsort_impl(input.data(), size, cmp);
        parlay::sequence<typename Range::value_type> out(size);
        parlay::copy(input, out);
        return out;
    }

private:
    static constexpr size_t BLOCK = 7ULL;
    template<typename T, typename Cmp>
    static inline std::pair<long long, long long> partition(T *data, long long size, Cmp cmp) {
        auto sep_pair = utils::median<5, T, Cmp>({std::make_pair(data[0], 0),
                                                  std::make_pair(data[size / 4], size / 4),
                                                  std::make_pair(data[size / 2], size / 2),
                                                  std::make_pair(data[3 * (size / 4)], 3 * (size / 4)),
                                                  std::make_pair(data[size - 1], size - 1)}, cmp);
        std::swap(data[size / 2], data[sep_pair.second]);
        long long i = 0;
        long long j = size - 1;
        do {
            while (cmp(data[i], sep_pair.first)) {
                i++;
            }
            while (cmp(sep_pair.first, data[j])) {
                j--;
            }

            if (i <= j) {
                std::swap(data[i++], data[j--]);
            }
        } while (i <= j);
        return std::make_pair(i, j);
    }

    template<typename T, typename Cmp>
    static inline void slow_sort_impl(T* data, long long size, Cmp cmp) {
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (cmp(data[j], data[i])) {
                    std::swap(data[i], data[j]);
                }
            }
        }
    }

    template<typename T, typename Cmp>
    static inline void qsort_impl(T* data, long long size, const Cmp &cmp) {
        if (size <= BLOCK) {
            slow_sort_impl(data, size, cmp);
            return;
        }
        auto borders = partition(data, size, cmp);
        if (borders.second > 0) {
            qsort_impl(data, borders.second + 1, cmp);
        }
        if (borders.first < size) {
            qsort_impl(&data[borders.first], size - borders.first, cmp);
        }
    }
};