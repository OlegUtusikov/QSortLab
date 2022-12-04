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

struct qsort_seq_std final {
    qsort_seq_std() = delete;
    ~qsort_seq_std() = delete;

    template<typename Range, typename Cmp = std::less<>>
    static auto sort(Range &input, Cmp cmp = Cmp()) {
        std::sort(input.begin(), input.end(), cmp);
        parlay::sequence<typename Range::value_type> out(input.size());
        parlay::copy(input, out);
        return out;
    }
};