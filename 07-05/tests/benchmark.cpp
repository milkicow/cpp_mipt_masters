#include <benchmark/benchmark.h>

#include <algorithm>
#include <numeric>
#include <vector>

#include "quicksort.hpp"

static std::vector<double> make_reverse_sorted(std::size_t n = 10000) {
    std::vector<double> v(n);
    std::iota(v.begin(), v.end(), 0.0);
    std::reverse(v.begin(), v.end());
    return v;
}

static void BM_QuickSort(benchmark::State& state) {
    const std::size_t threshold = static_cast<std::size_t>(state.range(0));

    const auto source = make_reverse_sorted();

    for (auto _ : state) {
        std::vector<double> data = source;

        benchmark::DoNotOptimize(data.data());
        quicksort::quicksort(data, threshold);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(source.size()));
}

BENCHMARK(BM_QuickSort)->RangeMultiplier(2)->Range(1, 512)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
