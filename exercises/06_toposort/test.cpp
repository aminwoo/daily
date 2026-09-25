#include "harness.h"
#include SOLUTION

static bool valid_order(int n, const std::vector<std::pair<int, int>>& edges,
                        const std::vector<int>& ord) {
  if ((int)ord.size() != n) return false;
  std::vector<int> pos(n, -1);
  for (int i = 0; i < n; ++i) {
    if (ord[i] < 0 || ord[i] >= n || pos[ord[i]] != -1) return false;
    pos[ord[i]] = i;
  }
  for (auto [u, v] : edges)
    if (pos[u] >= pos[v]) return false;
  return true;
}

TEST(basic) {
  CHECK(valid_order(1, {}, topo_sort(1, {})));
  CHECK(valid_order(4, {{0, 1}, {1, 2}, {0, 3}, {3, 2}},
                    topo_sort(4, {{0, 1}, {1, 2}, {0, 3}, {3, 2}})));
  CHECK_EQ(topo_sort(3, {{0, 1}, {1, 2}, {2, 0}}), std::vector<int>{});
  CHECK_EQ(topo_sort(2, {{1, 1}}), std::vector<int>{});
  CHECK(valid_order(3, {{0, 1}, {0, 1}}, topo_sort(3, {{0, 1}, {0, 1}})));
  CHECK_EQ(topo_sort(6, {{0, 1}, {1, 2}, {2, 3}, {3, 1}, {4, 5}}),
           std::vector<int>{});
}

TEST(random_dags) {
  using harness::rnd;
  for (int iter = 0; iter < 60; ++iter) {
    int n = (int)rnd(1, 40), m = (int)rnd(0, 120);
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), harness::rng);
    std::vector<std::pair<int, int>> edges;
    for (int e = 0; e < m; ++e) {
      int a = (int)rnd(0, n - 1), b = (int)rnd(0, n - 1);
      if (a == b) continue;
      if (a > b) std::swap(a, b);
      edges.push_back(
          {perm[a], perm[b]});  // always forward in the hidden order => DAG
    }
    auto ord = topo_sort(n, edges);
    CHECK(valid_order(n, edges, ord));
    // now inject a back edge along some path -> cycle
    if (m > 0) {
      auto [u, v] = edges[rnd(0, edges.size() - 1)];
      edges.push_back({v, u});
      CHECK_EQ(topo_sort(n, edges).size(), (size_t)0);
    }
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 300000, m = 900000;
  std::vector<std::pair<int, int>> edges;
  edges.reserve(m);
  for (int e = 0; e < m; ++e) {
    int a = (int)rnd(0, n - 2), b = (int)rnd(a + 1, n - 1);
    edges.push_back({a, b});
  }
  CHECK(valid_order(n, edges, topo_sort(n, edges)));
  edges.push_back({n - 1, 0});
  CHECK_EQ(topo_sort(n, edges).size(), (size_t)0);
}
