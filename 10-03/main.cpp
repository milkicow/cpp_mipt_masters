#include <boost/multi_array.hpp>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

constexpr int N = 10;

using Grid = boost::multi_array<int, 2>;
using Index = Grid::index;

Grid make_grid() { return Grid(boost::extents[N][N]); }

int count_neighbors(const Grid& g, int i, int j) {
    int alive = 0;

    for (int di = -1; di <= 1; ++di)
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;

            int ni = (i + di + N) % N;
            int nj = (j + dj + N) % N;

            alive += g[ni][nj];
        }

    return alive;
}

Grid step(const Grid& current) {
    Grid next = make_grid();

    for (Index i = 0; i < N; ++i)
        for (Index j = 0; j < N; ++j) {
            const int neighbors = count_neighbors(current, i, j);

            if (current[i][j] == 1) {
                next[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                next[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }

    return next;
}

void print(const Grid& g, int generation) {
    std::cout << "Generation " << generation << ":\n";
    std::cout << '+' << std::string(N * 2, '-') << "+\n";

    for (Index i = 0; i < N; ++i) {
        std::cout << '|';

        for (Index j = 0; j < N; ++j) std::cout << (g[i][j] ? " #" : " .");

        std::cout << " |\n";
    }

    std::cout << '+' << std::string(N * 2, '-') << "+\n";
}

int main() {
    Grid grid = make_grid();

    //  Glider
    //  . . # . . .
    //  . . . # . .
    //  . # # # . .
    grid[1][2] = 1;
    grid[2][3] = 1;
    grid[3][1] = 1;
    grid[3][2] = 1;
    grid[3][3] = 1;

    constexpr int generations = 20;

    for (int gen = 0; gen <= generations; ++gen) {
        print(grid, gen);

        if (gen < generations) grid = step(grid);
    }

    return 0;
}
