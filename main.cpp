//
// Created by utusikov on 04.12.22.
//

#include <functional>

#include <iostream>
#include <chrono>
#include <parlay/sequence.h>
#include <parlay/random.h>
#include <numeric>
#include <parlay/internal/get_time.h>
#include <parlay/internal/sequence_ops.h>
#include <cxxabi.h>
#include "qsort_par.h"
#include "qsort_seq_std.h"
#include "qsort_seq_my.h"
#include "utils.h"

template<typename Impl>
double benchmark(size_t n, size_t tries, bool need_check_order = false) {
    std::cout << "Benchmark: " << utils::demangle<Impl>() << std::endl;

    const auto dump_seconds = [] (const std::string& prefix, double time) {
        std::cout << prefix << ": " << time << "s" << std::endl;
    };

    static parlay::random_generator gen(std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution dis(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    using Cmp = std::less<>;

    std::vector<double> times;
    times.reserve(tries);

    parlay::sequence<int> result;

    parlay::internal::timer t;
    for (int i = 0; i < tries; i++) {
        auto data = parlay::internal::tabulate(n, [&](auto i) {
            auto r = gen[i];
            return dis(r);
        });
        t.start();
        result = Impl::sort(data, Cmp{});
        times.emplace_back(t.stop());
        dump_seconds("Time of try " + std::to_string(i), times.back());
        if (need_check_order) {
            assert(data.size() == result.size());
            std::sort(data.begin(), data.end(), Cmp{});
            for (size_t j = 0; j < data.size(); ++j) {
                if (data[j] != result[j]) {
                    std::cout << "Wrong pos: " << j << ". " << data[j] << " != " << result[j] << std::endl;
                    assert(false && "Bad order!!!");
                }
            }
        }
    }
    double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / static_cast<double>(times.size());
    dump_seconds("Avg exec time", avg_time);
    return avg_time;
}

int main() {
    const auto n = static_cast<size_t>(1e8);
    const size_t tries = 5;

    double avg_par = benchmark<qsort_par>(n, tries);
    double avg_seq = benchmark<qsort_seq_std>(n, tries);
    std::cout << "Speed up with std: " << avg_seq / avg_par << std::endl;

    double avg_my = benchmark<qsort_seq_my>(n, tries);
    std::cout << "Speed up with my: " << avg_my / avg_par << std::endl;
}
