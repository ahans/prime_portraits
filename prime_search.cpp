#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <mp++/integer.hpp>

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

namespace py = pybind11;

using int_t = mppp::integer<1>;

std::uniform_real_distribution<float> distribution(0.8f, 1.2f);

template <typename T> T square(T v) { return v*v; }

auto dither(const py::array_t<float>& image, py::array_t<float> centers, std::default_random_engine& generator)
{
    const auto height{image.shape(0)};
    const auto width{image.shape(1)};
    auto i{image.unchecked<3>()};
    auto c{centers.unchecked<2>()};

    const auto find_best_match = [&c] (const auto& pixel) {
        size_t best_idx{0};
        float best_dist = square(c(0, 0) - pixel[0]) + square(c(0, 1) - pixel[1]) + square(c(0, 2) - pixel[2]);
        for (auto i{1}; i < 10; ++i) {
            const float dist = square(c(i, 0) - pixel[0]) + square(c(i, 1) - pixel[1]) + square(c(i, 2) - pixel[2]);
            if (dist < best_dist) {
                best_dist = dist;
                best_idx = i;
            }
        }
        return static_cast<char>(best_idx);
    };

    static std::string out;
    out.resize((width) * (height), '0');
    for (auto y{0}; y < height; ++y) {
        for (auto x{0}; x < width; ++x) {
            const auto rnd0{distribution(generator)};
            const auto rnd1{distribution(generator)};
            const auto rnd2{distribution(generator)};

            const std::array<float, 3> pixel = {rnd0 * i(y, x, 0), rnd1 * i(y, x, 1), rnd2 * i(y, x, 2)};
            const auto idx = find_best_match(pixel);
            out.at(y*(width) + x) = '0' + idx;
        }
    }
    if ((out[width*height-1] - '0') % 2 == 0 ) {
        out[width*height-1] = '1';
    } else if (out[width*height-1] == '5') {
        out[width*height-1] = '7';
    }
    return out;
}

bool is_prime(std::string const& digits)
{
    return int_t{digits}.probab_prime_p(25) > 0;
}

struct PrimePortraitFinder
{
    std::string digits;
    std::atomic_bool stop_requested;
    size_t i;

    void run(const py::array_t<float>& image, const py::array_t<float>& centers)
    {
        find_prime_portrait(image, centers);
    }

    void find_prime_portrait(const py::array_t<float>& image, const py::array_t<float>& centers)
    {
        auto const seed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::default_random_engine generator(seed);

        size_t trials{};

        digits = dither(image, centers, generator);
        while (!is_prime(digits) && !stop_requested) {
            digits = dither(image, centers, generator);
            if (++trials % 100 == 0) {
                std::cout << "thread #" << i << ": " << trials << " trials done" << std::endl;
            }
        }
    }
};

auto find_prime_portrait(size_t num_threads, const py::array_t<float>& image, const py::array_t<float>& centers)
{
    std::vector<PrimePortraitFinder> finders(num_threads);
    std::mutex m;
    std::condition_variable cond;
    std::atomic<size_t> idx;
    std::vector<std::thread> threads;

    idx = num_threads;

    auto task = [&] (size_t i) {
        finders[i].i = i;
        finders[i].run(image, centers);
        idx = i;
        cond.notify_all();
    };
    for (size_t i{0}; i < num_threads; ++i) {
        threads.emplace_back(task, i);
    }

    std::unique_lock<std::mutex> lock{m};
    cond.wait(lock, [&] { return idx < num_threads; });

    std::cout << "thread #" << idx << " finished!" << std::endl;

    for (auto& f : finders) f.stop_requested = true;
    for (auto& t : threads) t.join();

    std::vector<std::string> all_digits;
    for (const auto& f : finders) all_digits.push_back(f.digits);

    return all_digits;
}

PYBIND11_MODULE(prime_search, m)
{
    m.def("find_prime_portrait", &find_prime_portrait);
}
