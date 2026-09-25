#include "harness.h"
#include SOLUTION

using Edges = std::vector<std::tuple<int, int, long long>>;

// brute force: max flow == min cut; enumerate all S-sets containing s and not t
static long long slow_min_cut(int n, const Edges& e, int s, int t) {
  long long best = LLONG_MAX;
  for (int mask = 0; mask < (1 << n); ++mask) {
    if (!(mask >> s & 1) || (mask >> t & 1)) continue;
    long long cut = 0;
    for (auto [u, v, c] : e)
      if ((mask >> u & 1) && !(mask >> v & 1)) cut += c;
    best = std::min(best, cut);
  }
  return best;
}

TEST(basic) {
  MaxFlow f(6);
  f.add_edge(0, 1, 16);
  f.add_edge(0, 2, 13);
  f.add_edge(1, 2, 10);
  f.add_edge(2, 1, 4);
  f.add_edge(1, 3, 12);
  f.add_edge(3, 2, 9);
  f.add_edge(2, 4, 14);
  f.add_edge(4, 3, 7);
  f.add_edge(3, 5, 20);
  f.add_edge(4, 5, 4);
  CHECK_EQ(f.max_flow(0, 5), 23LL);  // CLRS example
  MaxFlow g(2);
  CHECK_EQ(g.max_flow(0, 1), 0LL);
  MaxFlow h(2);
  h.add_edge(0, 1, 5);
  h.add_edge(0, 1, 7);
  h.add_edge(1, 0, 100);
  CHECK_EQ(h.max_flow(0, 1), 12LL);
  MaxFlow big(3);
  big.add_edge(0, 1, 1LL << 40);
  big.add_edge(1, 2, 1LL << 40);
  big.add_edge(0, 2, 1LL << 40);
  CHECK_EQ(big.max_flow(0, 2), 1LL << 41);
}

TEST(random_vs_min_cut) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    int n = (int)rnd(2, 9), m = (int)rnd(0, 25);
    Edges e;
    MaxFlow f(n);
    for (int i = 0; i < m; ++i) {
      int u = (int)rnd(0, n - 1), v = (int)rnd(0, n - 1);
      long long c = rnd(0, 10);
      e.push_back({u, v, c});
      f.add_edge(u, v, c);
    }
    int s = (int)rnd(0, n - 1), t = (int)rnd(0, n - 2);
    if (t >= s) ++t;
    CHECK_EQ(f.max_flow(s, t), slow_min_cut(n, e, s, t));
  }
}

TEST(perf_bipartite_matching) {
  using harness::rnd;
  int L = 20000, R = 20000, m = 200000;
  MaxFlow f(L + R + 2);
  int s = L + R, t = L + R + 1;
  for (int i = 0; i < L; ++i) f.add_edge(s, i, 1);
  for (int j = 0; j < R; ++j) f.add_edge(L + j, t, 1);
  for (int i = 0; i < L; ++i)
    f.add_edge(i, L + i, 1);  // a perfect matching exists
  for (int k = 0; k < m; ++k)
    f.add_edge((int)rnd(0, L - 1), L + (int)rnd(0, R - 1), 1);
  CHECK_EQ(f.max_flow(s, t), (long long)L);
}

TEST(perf_layered) {
  using harness::rnd;
  int layers = 50, width = 100, n = layers * width + 2, s = n - 2, t = n - 1;
  MaxFlow f(n);
  for (int i = 0; i < width; ++i) {
    f.add_edge(s, i, rnd(1, 1000));
    f.add_edge((layers - 1) * width + i, t, rnd(1, 1000));
  }
  for (int l = 0; l + 1 < layers; ++l)
    for (int i = 0; i < width; ++i)
      for (int k = 0; k < 8; ++k)
        f.add_edge(l * width + i, (l + 1) * width + (int)rnd(0, width - 1),
                   rnd(1, 300));
  CHECK(f.max_flow(s, t) > 0);
}
