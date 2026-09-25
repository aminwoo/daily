#include "harness.h"
#include SOLUTION

using Edges = std::vector<std::tuple<int, int, long long>>;

// brute force: Floyd–Warshall; negative cycle reachable from s  <=>  some k
// with d[s][k] < INF and d[k][k] < 0
static bool slow(int n, const Edges& e, int s, std::vector<long long>& out) {
  std::vector<std::vector<long long>> d(n, std::vector<long long>(n, INF));
  for (int i = 0; i < n; ++i) d[i][i] = 0;
  for (auto [u, v, w] : e) d[u][v] = std::min(d[u][v], w);
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (d[i][k] < INF && d[k][j] < INF)
          d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
  for (int k = 0; k < n; ++k)
    if (d[s][k] < INF && d[k][k] < 0) return false;
  out = d[s];
  return true;
}

TEST(basic) {
  std::vector<long long> d;
  CHECK(bellman_ford(4, {{0, 1, 4}, {0, 2, 5}, {2, 1, -3}, {1, 3, 2}}, 0, d));
  CHECK_EQ(d, (std::vector<long long>{0, 2, 5, 4}));
  CHECK(!bellman_ford(3, {{0, 1, 1}, {1, 2, -1}, {2, 1, -1}}, 0, d));
  CHECK(bellman_ford(3, {{1, 2, -1}, {2, 1, -1}}, 0,
                     d));  // negative cycle exists but is unreachable
  CHECK_EQ(d, (std::vector<long long>{0, INF, INF}));
  CHECK(!bellman_ford(1, {{0, 0, -1}}, 0, d));
  CHECK(bellman_ford(1, {{0, 0, 0}}, 0, d));
  CHECK_EQ(d, (std::vector<long long>{0}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  int neg = 0, pos = 0;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(1, 9), m = (int)rnd(0, 16);
    Edges e;
    for (int i = 0; i < m; ++i)
      e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(-6, 12)});
    int s = (int)rnd(0, n - 1);
    std::vector<long long> want, got;
    bool ok = slow(n, e, s, want);
    CHECK_EQ(bellman_ford(n, e, s, got), ok);
    if (ok) {
      CHECK_EQ(got, want);
      ++pos;
    } else
      ++neg;
  }
  CHECK(neg > 20 && pos > 20);  // both cases actually exercised
}

TEST(perf) {
  using harness::rnd;
  int n = 2000, m = 200000;
  Edges e;
  for (int i = 0; i + 1 < n; ++i) e.push_back({i, i + 1, rnd(1, 100)});
  for (int i = 0; i < m; ++i)
    e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1), rnd(1, 1000000)});
  std::vector<long long> d;
  CHECK(bellman_ford(n, e, 0, d));
  CHECK(d[n - 1] > 0 && d[n - 1] < INF);
  e.push_back({n - 1, 0, -1000000000LL});
  CHECK(!bellman_ford(n, e, 0, d));
}
