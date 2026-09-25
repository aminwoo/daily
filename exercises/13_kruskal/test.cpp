#include "harness.h"
#include SOLUTION

using Edges = std::vector<std::tuple<int, int, long long>>;

// brute force: try every subset of n-1 edges, keep the cheapest one that
// connects everything
static long long slow_mst(int n, const Edges& e) {
  int m = e.size();
  long long best = -1;
  for (int mask = 0; mask < (1 << m); ++mask) {
    if (__builtin_popcount(mask) != n - 1) continue;
    std::vector<int> lab(n);
    std::iota(lab.begin(), lab.end(), 0);
    long long w = 0;
    for (int i = 0; i < m; ++i)
      if (mask >> i & 1) {
        auto [u, v, c] = e[i];
        w += c;
        int a = lab[u], b = lab[v];
        for (auto& l : lab)
          if (l == a) l = b;
      }
    if (std::count(lab.begin(), lab.end(), lab[0]) != n) continue;
    if (best < 0 || w < best) best = w;
  }
  return best;
}

TEST(basic) {
  CHECK_EQ(kruskal(1, {}), 0LL);
  CHECK_EQ(kruskal(2, {}), -1LL);
  CHECK_EQ(kruskal(2, {{0, 0, 5}}), -1LL);
  CHECK_EQ(kruskal(4, {{0, 1, 1}, {1, 2, 2}, {2, 3, 3}, {0, 3, 10}, {0, 2, 2}}),
           6LL);
  CHECK_EQ(kruskal(3, {{0, 1, 7}, {0, 1, 3}, {1, 2, 4}, {2, 2, 1}}), 7LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 150; ++it) {
    int n = (int)rnd(1, 6), m = (int)rnd(0, 11);
    Edges e;
    for (int i = 0; i < m; ++i)
      e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(0, 20)});
    CHECK_EQ(kruskal(n, e), slow_mst(n, e));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 200000, m = 600000;
  Edges e;
  e.reserve(m + n);
  for (int i = 1; i < n; ++i)
    e.push_back({i, (int)rnd(0, i - 1), 1000000 + rnd(0, 1000)});
  for (int i = 0; i < m; ++i)
    e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(1, 2000000)});
  long long w = kruskal(n, e);
  CHECK(w > 0 && w < (long long)(n - 1) * 1001000);
}
