#include "harness.h"
#include SOLUTION

using Edges = std::vector<std::tuple<int, int, long long>>;

// brute force: Bellman-Ford from every source
static std::vector<std::vector<long long>> slow(int n, const Edges& e) {
  std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
  for (int s = 0; s < n; ++s) {
    d[s][s] = 0;
    for (int it = 0; it < n; ++it)
      for (auto [u, v, w] : e)
        if (d[s][u] < INF && d[s][u] + w < d[s][v]) d[s][v] = d[s][u] + w;
  }
  return d;
}

TEST(basic) {
  auto d = floyd_warshall(
      4, {{0, 1, 5}, {1, 2, -2}, {0, 2, 4}, {2, 3, 1}, {3, 1, 10}});
  CHECK_EQ(d[0], (std::vector<long long>{0, 5, 3, 4}));
  CHECK_EQ(d[1], (std::vector<long long>{INF, 0, -2, -1}));
  CHECK_EQ(d[3], (std::vector<long long>{INF, 10, 8, 0}));
  auto e = floyd_warshall(2, {{0, 1, 3}, {0, 1, 1}, {1, 1, 4}});
  CHECK_EQ(e[0], (std::vector<long long>{0, 1}));
  CHECK_EQ(e[1], (std::vector<long long>{INF, 0}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 100; ++it) {
    int n = (int)rnd(1, 12), m = (int)rnd(0, 30);
    Edges e;
    // edges forward in a hidden order may be negative; backward edges are so
    // heavy that no cycle (which must use one) can be negative
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), harness::rng);
    for (int i = 0; i < m; ++i) {
      int a = (int)rnd(0, n - 1), b = (int)rnd(0, n - 1);
      long long w =
          a < b ? rnd(-15, 15) : (a == b ? rnd(0, 5) : rnd(1000, 1100));
      e.push_back({perm[a], perm[b], w});
    }
    CHECK_EQ(floyd_warshall(n, e), slow(n, e));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 400;
  Edges e;
  for (int i = 0; i < 4000; ++i)
    e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(1, 1000)});
  auto d = floyd_warshall(n, e);
  long long s = 0;
  for (auto& row : d)
    for (auto x : row)
      if (x < INF) s += x;
  CHECK(s > 0);
}
