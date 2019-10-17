#include <cassert>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    constexpr size_t N{100'000'000};
    constexpr double working_set_GB{3.0 * N * sizeof(double) / exp2(30)};

    random_device rd;
    mt19937 gen{rd()};
    normal_distribution dis;

    vector<double> X(N), Y(N), Z(N), Z0(N), Z1(N);

    for (auto &&x:X) x = dis(gen);
    for (auto &&y:Y) y = dis(gen);

    cout << fixed;
    cout.precision(3);

    cout << "for loop version: \n";

    {
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            Z0[i] = sqrt(X[i] * X[i] + Y[i] * Y[i]);
        }
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gbs = working_set_GB / s.count();
        cout << "Time for an algebraic expression:     "
             << s.count() << " s / " << gbs << " GB/s\n";
    }

    {
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; ++i) {
            Z1[i] = atan2(Y[i], X[i]);
        }
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gbs = working_set_GB / s.count();
        cout << "Time for a transcendental expression: "
             << s.count() << " s / " << gbs << " GB/s\n";
    }

    cout << "STL algorithms version: \n";

    {
        auto t0 = chrono::high_resolution_clock::now();
        transform(begin(X), end(X), begin(Y), begin(Z),
                  [](auto x, auto y) { return sqrt(x * x + y * y); });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gbs = working_set_GB / s.count();
        cout << "Time for an algebraic expression:     "
             << s.count() << " s / " << gbs << " GB/s\n";
        assert(Z == Z0);
    }

    {
        auto t0 = chrono::high_resolution_clock::now();
        transform(begin(X), end(X), begin(Y), begin(Z),
                  [](auto x, auto y) { return atan2(y, x); });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gbs = working_set_GB / s.count();
        cout << "Time for a transcendental expression: "
             << s.count() << " s / " << gbs << " GB/s\n";
        assert(Z == Z1);
    }

    cout << "Parallel STL algorithms version:\n";

    {
        auto t0 = chrono::high_resolution_clock::now();
        transform(execution::par, begin(X), end(X), begin(Y), begin(Z),
                  [](auto x, auto y) { return sqrt(x * x + y * y); });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gbs = working_set_GB / s.count();
        cout << "Time for an algebraic expression:     "
             << s.count() << " s / " << gbs << " GB/s\n";
        assert(Z == Z0);
    }

    {
        auto t0 = chrono::high_resolution_clock::now();
        transform(execution::par, begin(X), end(X), begin(Y), begin(Z),
                  [](auto x, auto y) { return atan2(y, x); });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gbs = working_set_GB / s.count();
        cout << "Time for a transcendental expression: "
             << s.count() << " s / " << gbs << " GB/s\n";
        assert(Z == Z1);
    }
}
