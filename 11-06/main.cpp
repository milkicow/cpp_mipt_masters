#include <algorithm>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_traits.hpp>
#include <format>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

constexpr int N = 10;

using Graph = boost::adjacency_matrix<boost::undirectedS, boost::no_property,
                                      boost::property<boost::edge_weight_t, int>>;

int main() {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    Graph g(N);

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            boost::add_edge(i, j, dist(gen), g);
        }
    }

    auto weight = boost::get(boost::edge_weight, g);

    std::cout << "Adjacency matrix:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) {
                std::cout << std::format("{:>4}", 0);
            } else {
                auto [e, ok] = boost::edge(i, j, g);
                std::cout << std::format("{:>4}", boost::get(weight, e));
            }
        }
        std::cout << '\n';
    }

    std::vector<int> perm(N - 1);
    std::iota(perm.begin(), perm.end(), 1);

    int best_cost = std::numeric_limits<int>::max();
    std::vector<int> best_perm;

    do {
        int cost = 0;
        int prev = 0;
        for (int v : perm) {
            auto [e, ok] = boost::edge(prev, v, g);
            cost += boost::get(weight, e);
            prev = v;
        }
        auto [e, ok] = boost::edge(prev, 0, g);
        cost += boost::get(weight, e);

        if (cost < best_cost) {
            best_cost = cost;
            best_perm = perm;
        }
    } while (std::next_permutation(perm.begin(), perm.end()));

    std::cout << "\nOptimal tour: 0";
    for (int v : best_perm) std::cout << " -> " << v;
    std::cout << " -> 0\n";
    std::cout << "Total cost: " << best_cost << '\n';

    return 0;
}
