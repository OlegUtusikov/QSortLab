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
#include "qsort_seq_my.h"

struct qsort_par final {
    qsort_par() = delete;
    ~qsort_par() = delete;

    template<typename Range, typename Cmp = std::less<>>
    static auto sort(Range &input, Cmp cmp = Cmp()) {
        size_t input_size = input.size();
        parlay::sequence<typename Range::value_type> out(input_size);
        qsort_impl(input.cut(0, input_size), out.cut(0, input_size), cmp);
        return out;
    }
private:
    static constexpr size_t BLOCK = 16384ULL;

    enum GROUP : uint8_t {
         LE = 0U,
         G = 1U,
         SIZE
    };

    template<typename RangeIn, typename RangeOut, typename Cmp>
    static void qsort_impl(RangeIn &&in_slice, RangeOut &&out_slice, const Cmp &cmp) {
        using value_type = typename RangeIn::value_type;
        size_t size = in_slice.size();
        if (size > BLOCK)  {
            auto separator_pair = utils::median<5, value_type, Cmp>({
                    std::make_pair(in_slice[0], 0),
                    std::make_pair(in_slice[size / 4], size / 4),
                    std::make_pair(in_slice[size / 2], size / 2),
                    std::make_pair(in_slice[3 * (size / 4)], 3 * (size / 4)),
                    std::make_pair(in_slice[size - 1], size - 1)
                }, cmp);

            const auto map_val_to_group = [&cmp, separator = separator_pair.first] (value_type val) -> uint8_t { return !cmp(separator, val) ? LE : G; };
            auto groups_offsets = parlay::counting_sort_inplace(in_slice, SIZE, map_val_to_group);
            const size_t g_offset = groups_offsets[G];
            parlay::par_do(
                    [&] { qsort_impl(in_slice.cut(0, g_offset), out_slice.cut(0, g_offset), cmp); },
                    [&] { qsort_impl(in_slice.cut(g_offset, size), out_slice.cut(g_offset, size), cmp); }
            );
        } else {
            parlay::copy(in_slice, out_slice);
            qsort_seq_my::sort(out_slice, cmp);
        }
    }
};