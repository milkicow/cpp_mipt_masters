/////////////////////////////////////////////////////////////////////////////////

// chapter : Number Processing

/////////////////////////////////////////////////////////////////////////////////

// section : Chrono Management

/////////////////////////////////////////////////////////////////////////////////

// content : Timing
//
// content : Timer with start, stop, and average
//
// content : std::vector of std::chrono::duration < double >

/////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

template <typename D = std::chrono::duration<double> >
class Timer {
   public:
    Timer(std::string const& scope) : m_scope(scope), m_begin(clock_t::now()) {}
    ~Timer() { std::print("{} : {:.6f}\n", m_scope, elapsed().count()); }

    auto elapsed() const { return std::chrono::duration_cast<D>(clock_t::now() - m_begin); }

    auto start() -> void {
        if (m_running) {
            throw std::logic_error("timer is already running");
        }

        m_running = true;

        m_begin = clock_t::now();
    }

    auto stop() -> void {
        if (!m_running) {
            throw std::logic_error("timer is not running");
        }

        m_running = false;

        m_samples.push_back(std::chrono::duration_cast<D>(clock_t::now() - m_begin));
    }

    auto average() const -> double {
        if (m_samples.empty()) {
            return 0.0;
        }

        auto sum = std::accumulate(m_samples.begin(), m_samples.end(), D{});

        return (sum / static_cast<double>(m_samples.size())).count();
    }

   private:
    using clock_t = std::chrono::steady_clock;

    std::string m_scope;
    clock_t::time_point m_begin;
    bool m_running = false;
    std::vector<D> m_samples;
};

auto calculate(std::size_t size) {
    auto x = 0.0;

    for (auto i = 0uz; i < size; ++i) {
        x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
    }

    return x;
}

auto equal(double x, double y, double epsilon = 1e-6) { return std::abs(x - y) < epsilon; }

int main() {
    Timer timer("main : timer");
    assert(equal(calculate(1'000'000), 1'000'000));

    Timer<> series("series : timer");

    for (auto i = 0; i < 5; ++i) {
        series.start();
        assert(equal(calculate(1'000'000), 1'000'000));
        series.stop();
    }

    std::print("series average : {:.6f}\n", series.average());
}
