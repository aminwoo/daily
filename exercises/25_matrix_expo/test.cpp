#include "harness.h"
#include SOLUTION

static Mat slow_mul(const Mat& A, const Mat& B, long long mod) {
  Mat C(A.size(), std::vector<long long>(B[0].size(), 0));
  for (size_t i = 0; i < A.size(); ++i)
    for (size_t j = 0; j < B[0].size(); ++j)
      for (size_t k = 0; k < B.size(); ++k)
        C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
  return C;
}

TEST(mul_and_pow_basic) {
  Mat A = {{1, 1}, {1, 0}};
  CHECK_EQ(mat_mul(A, A, 1000), (Mat{{2, 1}, {1, 1}}));
  CHECK_EQ(mat_mul({{1, 2, 3}}, {{1}, {2}, {3}}, 100), (Mat{{14}}));
  CHECK_EQ(mat_pow(A, 0, 1000), (Mat{{1, 0}, {0, 1}}));
  CHECK_EQ(mat_pow(A, 1, 1000), A);
  CHECK_EQ(mat_pow(A, 10, 1000), (Mat{{89, 55}, {55, 34}}));
  CHECK_EQ(mat_pow({{2}}, 62, 1000000007), (Mat{{(1LL << 62) % 1000000007}}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    long long mod = rnd(1, 1000);
    int n = (int)rnd(1, 5), m = (int)rnd(1, 5), p = (int)rnd(1, 5);
    Mat A(n, std::vector<long long>(m)), B(m, std::vector<long long>(p));
    for (auto& r : A)
      for (auto& x : r) x = rnd(0, mod - 1);
    for (auto& r : B)
      for (auto& x : r) x = rnd(0, mod - 1);
    CHECK_EQ(mat_mul(A, B, mod), slow_mul(A, B, mod));
    Mat S(n, std::vector<long long>(n));
    for (auto& r : S)
      for (auto& x : r) x = rnd(0, mod - 1);
    int e = (int)rnd(0, 12);
    Mat want(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) want[i][i] = 1 % mod;
    for (int i = 0; i < e; ++i) want = slow_mul(want, S, mod);
    CHECK_EQ(mat_pow(S, e, mod), want);
  }
}

TEST(fib_and_walks) {
  using harness::rnd;
  const long long M = 1000000007;
  long long a = 0, b = 1;
  for (int n = 0; n <= 300; ++n) {
    CHECK_EQ(fib(n, M), a);
    long long c = (a + b) % M;
    a = b;
    b = c;
  }
  CHECK_EQ(fib(1000000000000000000LL, M), 209783453LL);
  CHECK_EQ(fib(10, 7), 55 % 7);
  for (int it = 0; it < 100; ++it) {
    int n = (int)rnd(1, 6);
    long long k = rnd(0, 8), mod = rnd(2, 1000);
    std::vector<std::vector<int>> adj(n, std::vector<int>(n));
    for (auto& r : adj)
      for (auto& x : r) x = (int)rnd(0, 1);
    int s = (int)rnd(0, n - 1), t = (int)rnd(0, n - 1);
    std::vector<long long> cur(n, 0);
    cur[s] = 1;
    for (int step = 0; step < k; ++step) {
      std::vector<long long> nx(n, 0);
      for (int u = 0; u < n; ++u)
        for (int v = 0; v < n; ++v)
          if (adj[u][v]) nx[v] = (nx[v] + cur[u]) % mod;
      cur = nx;
    }
    CHECK_EQ(count_walks(adj, s, t, k, mod), cur[t]);
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 60;
  Mat A(n, std::vector<long long>(n));
  for (auto& r : A)
    for (auto& x : r) x = rnd(0, 1000000006);
  auto P = mat_pow(A, 1000000000000000000LL, 1000000007);
  long long s = 0;
  for (auto& r : P)
    for (auto x : r) s += x;
  CHECK(s > 0);
}
