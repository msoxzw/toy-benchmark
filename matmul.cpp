#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <tbb/parallel_for.h>

using namespace std;

int main() {
    constexpr size_t M{1'000}, N{2'000}, P{3'000};
    constexpr size_t S{64};
    constexpr double working_set_GFLO{2.0 * M * N * P / 1e9};

    random_device rd;
    mt19937 gen{rd()};
    normal_distribution dis;

    vector<double> X(M * N), Y(N * P), Z, Z0;

    for (auto &&x:X) x = dis(gen);
    for (auto &&y:Y) y = dis(gen);

    cout << fixed;
    cout.precision(3);

    cout << "Standard version:\n";

    {
        Z0.assign(M * P, 0.0);
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < P; ++j) {
                for (size_t k = 0; k < N; ++k) {
                    Z0[i * P + j] += X[i * N + k] * Y[k * P + j];
                }
            }
        }
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gflops = working_set_GFLO / s.count();
        cout << "Time for naive matrix multiplication: "
             << s.count() << " s / " << gflops << " GFLOPS\n";
    }

    {
        Z.assign(M * P, 0.0);
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < M; ++i) {
            for (size_t k = 0; k < N; ++k) {
                for (size_t j = 0; j < P; ++j) {
                    Z[i * P + j] += X[i * N + k] * Y[k * P + j];
                }
            }
        }
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gflops = working_set_GFLO / s.count();
        cout << "Time for naive with loop interchange: "
             << s.count() << " s / " << gflops << " GFLOPS\n";
        assert(Z == Z0);
    }

    {
        Z.assign(M * P, 0.0);
        auto t0 = chrono::high_resolution_clock::now();
        for (size_t I = 0; I < M; I += S) {
            auto m = min(M, I + S);
            for (size_t K = 0; K < N; K += S) {
                auto n = min(N, K + S);
                for (size_t J = 0; J < P; J += S) {
                    auto p = min(P, J + S);
                    for (auto i = I; i < m; ++i) {
                        for (auto k = K; k < n; ++k) {
                            for (auto j = J; j < p; ++j) {
                                Z[i * P + j] += X[i * N + k] * Y[k * P + j];
                            }
                        }
                    }
                }
            }
        }
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gflops = working_set_GFLO / s.count();
        cout << "Time for tiled matrix multiplication: "
             << s.count() << " s / " << gflops << " GFLOPS\n";
        assert(Z == Z0);
    }


    cout << "Parallel version:\n";

    {
        Z.assign(M * P, 0.0);
        auto t0 = chrono::high_resolution_clock::now();
        tbb::parallel_for(size_t{0}, M, [&, N, P](auto i) {
            for (size_t j = 0; j < P; ++j) {
                for (size_t k = 0; k < N; ++k) {
                    Z[i * P + j] += X[i * N + k] * Y[k * P + j];
                }
            }
        });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gflops = working_set_GFLO / s.count();
        cout << "Time for naive matrix multiplication: "
             << s.count() << " s / " << gflops << " GFLOPS\n";
        assert(Z == Z0);
    }

    {
        Z.assign(M * P, 0.0);
        auto t0 = chrono::high_resolution_clock::now();
        tbb::parallel_for(size_t{0}, M, [&, N, P](auto i) {
            for (size_t k = 0; k < N; ++k) {
                for (size_t j = 0; j < P; ++j) {
                    Z[i * P + j] += X[i * N + k] * Y[k * P + j];
                }
            }
        });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gflops = working_set_GFLO / s.count();
        cout << "Time for naive with loop interchange: "
             << s.count() << " s / " << gflops << " GFLOPS\n";
        assert(Z == Z0);
    }

    {
        Z.assign(M * P, 0.0);
        auto t0 = chrono::high_resolution_clock::now();
        tbb::parallel_for(size_t{0}, M, S, [&, N, P](auto I) {
            auto m = min(M, I + S);
            for (size_t K = 0; K < N; K += S) {
                auto n = min(N, K + S);
                for (size_t J = 0; J < P; J += S) {
                    auto p = min(P, J + S);
                    for (auto i = I; i < m; ++i) {
                        for (auto k = K; k < n; ++k) {
                            for (auto j = J; j < p; ++j) {
                                Z[i * P + j] += X[i * N + k] * Y[k * P + j];
                            }
                        }
                    }
                }
            }
        });
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> s = t1 - t0;
        auto gflops = working_set_GFLO / s.count();
        cout << "Time for tiled matrix multiplication: "
             << s.count() << " s / " << gflops << " GFLOPS\n";
        assert(Z == Z0);
    }
}