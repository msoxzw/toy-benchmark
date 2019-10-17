### Toy Benchmark

Simple toy benchmark for C++ and Python, includes large matrix multiplication, different

algebraic expression and  transcendental expression for large vector.

#### Requirements

##### C++:

C++ compiler supporting C++17

CMake >= 3.8

Intel tbb

##### Python:

Python >= 3.6

numexpr

#### Possible Results

From matmul.cpp:

```
Standard version:

Time for naive matrix multiplication: 26.716 s / 0.449 GFLOPS

Time for naive with loop interchange: 2.359 s / 5.087 GFLOPS

Time for tiled matrix multiplication: 0.974 s / 12.317 GFLOPS

Parallel version:

Time for naive matrix multiplication: 7.769 s / 1.545 GFLOPS

Time for naive with loop interchange: 0.470 s / 25.548 GFLOPS

Time for tiled matrix multiplication: 0.312 s / 38.414 GFLOPS
```

From parallel.cpp

```
for loop version:

Time for an algebraic expression:     0.187 s / 11.924 GB/s

Time for a transcendental expression: 4.671 s / 0.479 GB/s

STL algorithms version:

Time for an algebraic expression:     0.187 s / 11.922 GB/s

Time for a transcendental expression: 4.515 s / 0.495 GB/s

Parallel STL algorithms version:

Time for an algebraic expression:     0.109 s / 20.445 GB/s

Time for a transcendental expression: 0.672 s / 3.328 GB/s
```

From parallel.py

```
Python version:

Time for an algebraic expression:     22.621 s / 0.099 GB/s

Time for a transcendental expression: 24.880 s / 0.090 GB/s

NumPy version:

Time for an algebraic expression:     1.843 s / 1.213 GB/s

Time for a transcendental expression: 3.421 s / 0.653 GB/s

Numexpr version:

Time for an algebraic expression:     0.141 s / 15.898 GB/s

Time for a transcendental expression: 0.675 s / 3.312 GB/s

```
