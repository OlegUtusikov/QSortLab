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
        qsort_impl(input.cut(0, size), cmp);
        parlay::sequence<typename Range::value_type> out(size);
        parlay::copy(input, out);
        return out;
    }

private:
    static constexpr size_t BLOCK = 7ULL;
    template<typename Range, typename Cmp>
    static inline std::pair<long long, long long> partition(Range &in_slice, Cmp cmp) {
        long long size = in_slice.size();
        auto sep_pair = utils::median<5, typename Range::value_type, Cmp>({std::make_pair(in_slice[0], 0),
                                                  std::make_pair(in_slice[size / 4], size / 4),
                                                  std::make_pair(in_slice[size / 2], size / 2),
                                                  std::make_pair(in_slice[3 * (size / 4)], 3 * (size / 4)),
                                                  std::make_pair(in_slice[size - 1], size - 1)}, cmp);
        std::swap(in_slice[size / 2], in_slice[sep_pair.second]);
        long long i = 0;
        long long j = size - 1;
        do {
            while (cmp(in_slice[i], sep_pair.first)) {
                i++;
            }
            while (cmp(sep_pair.first, in_slice[j])) {
                j--;
            }

            if (i <= j) {
                std::swap(in_slice[i++], in_slice[j--]);
            }
        } while (i <= j);
        return std::make_pair(i, j);
    }

    template<typename Range, typename Cmp>
    static inline void slow_sort_impl(Range &&in_slice, Cmp cmp) {
        long long size = in_slice.size();
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (cmp(in_slice[j], in_slice[i])) {
                    std::swap(in_slice[i], in_slice[j]);
                }
            }
        }
    }

    template<typename Range, typename Cmp>
    static inline void qsort_impl(Range &&in_slice, const Cmp &cmp) {
        long long size = in_slice.size();
        if (size <= BLOCK) {
            slow_sort_impl(in_slice, cmp);
            return;
        }
        auto borders = partition(in_slice, cmp);
        if (borders.second > 0) {
            qsort_impl(in_slice.cut(0, borders.second + 1), cmp);
        }
        if (borders.first < size) {
            qsort_impl(in_slice.cut(borders.first, size), cmp);
        }
    }
};