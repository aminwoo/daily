#include "harness.h"
#include SOLUTION

using Adj = std::vector<std::vector<std::pair<int, long long>>>;

TEST(basic) {
  Adj adj(5);
  auto add = [&](int u, int v, long long w) { adj[u].push_back({v, w}); };
  add(0, 1, 4);
  add(0, 2, 1);
  add(2, 1, 2);
  add(1, 3, 1);
  add(2, 3, 5);
  auto d = dijkstra(5, adj, 0);
  CHECK_EQ(d, (std::vector<long long>{0, 3, 1, 4, INF}));
}

TEST(zero_weight_and_self_loops) {
  Adj adj(3);
  adj[0].push_back({1, 0});
  adj[1].push_back({1, 0});
  adj[1].push_back({2, 0});
  adj[2].push_back({0, 7});
  CHECK_EQ(dijkstra(3, adj, 0), (std::vector<long long>{0, 0, 0}));
  CHECK_EQ(dijkstra(3, adj, 2), (std::vector<long long>{7, 7, 0}));
}

// brute force: Floyd–Warshall
TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int iter = 0; iter < 40; ++iter) {
    int n = (int)rnd(1, 25), m = (int)rnd(0, 80);
    Adj adj(n);
    std::vector<std::vector<long long>> fw(n, std::vector<long long>(n, INF));
    for (int i = 0; i < n; ++i) fw[i][i] = 0;
    for (int e = 0; e < m; ++e) {
      int u = (int)rnd(0, n - 1), v = (int)rnd(0, n - 1);
      long long w = rnd(0, 20);
      adj[u].push_back({v, w});
      fw[u][v] = std::min(fw[u][v], w);
    }
    for (int k = 0; k < n; ++k)
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
          fw[i][j] = std::min(fw[i][j], fw[i][k] + fw[k][j]);
    int src = (int)rnd(0, n - 1);
    CHECK_EQ(dijkstra(n, adj, src), fw[src]);
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 200000, m = 600000;
  Adj adj(n);
  for (int i = 0; i + 1 < n; ++i) adj[i].push_back({i + 1, rnd(1, 1000)});
  for (int e = 0; e < m; ++e)
    adj[rnd(0, n - 1)].push_back({(int)rnd(0, n - 1), rnd(1, 1000000)});
  auto d = dijkstra(n, adj, 0);
  long long mx = 0;
  for (auto x : d) {
    CHECK(x < INF);
    mx = std::max(mx, x);
  }
  CHECK(mx > 0);
}
