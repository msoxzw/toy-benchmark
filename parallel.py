import sys
from math import atan2, sqrt
from time import time

import numexpr as ne
import numpy as np

N = 100_000_000

np.random.seed()

X = np.random.randn(N).tolist()
Y = np.random.randn(N).tolist()

working_set_GB = 3 * N * 8 / 2 ** 30

print(f'Python version: {sys.version}')

t0 = time()
Z0 = [sqrt(x * x + y * y) for x, y in zip(X, Y)]
t1 = time()
gbs = working_set_GB / (t1 - t0)
print(f'Time for an algebraic expression:     {t1 - t0:.3f} s / {gbs:.3f} GB/s')

t0 = time()
Z1 = [atan2(y, x) for x, y in zip(X, Y)]
t1 = time()
gbs = working_set_GB / (t1 - t0)
print(f'Time for a transcendental expression: {t1 - t0:.3f} s / {gbs:.3f} GB/s')

X = np.array(X)
Y = np.array(Y)
Z0 = np.array(Z0)
Z1 = np.array(Z1)

print(f'NumPy version: {np.__version__}')

t0 = time()
Z = np.sqrt(X * X + Y * Y)
t1 = time()
gbs = working_set_GB / (t1 - t0)
print(f'Time for an algebraic expression:     {t1 - t0:.3f} s / {gbs:.3f} GB/s')
np.testing.assert_equal(Z, Z0)

t0 = time()
Z = np.arctan2(Y, X)
t1 = time()
gbs = working_set_GB / (t1 - t0)
print(f'Time for a transcendental expression: {t1 - t0:.3f} s / {gbs:.3f} GB/s')
np.testing.assert_equal(Z, Z1)

print(f'Numexpr version: {ne.__version__}')

t0 = time()
ne.evaluate('sqrt(X * X + Y * Y)', out=Z)
t1 = time()
gbs = working_set_GB / (t1 - t0)
print(f'Time for an algebraic expression:     {t1 - t0:.3f} s / {gbs:.3f} GB/s')
np.testing.assert_equal(Z, Z0)

t0 = time()
ne.evaluate('arctan2(Y, X)', out=Z)
t1 = time()
gbs = working_set_GB / (t1 - t0)
print(f'Time for a transcendental expression: {t1 - t0:.3f} s / {gbs:.3f} GB/s')
np.testing.assert_equal(Z, Z1)
