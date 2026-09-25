#include "harness.h"
#include SOLUTION

using Adj = std::vector<std::vector<int>>;

static std::vector<std::vector<char>> reach(int n, const Adj& adj) {
  std::vector<std::vector<char>> r(n, std::vector<char>(n, 0));
  for (int s = 0; s < n; ++s) {
    std::vector<int> st = {s};
    r[s][s] = 1;
    while (!st.empty()) {
      int u = st.back();
      st.pop_back();
      for (int v : adj[u])
        if (!r[s][v]) {
          r[s][v] = 1;
          st.push_back(v);
        }
    }
  }
  return r;
}

static void check_scc(int n, const Adj& adj, const std::vector<int>& comp) {
  REQUIRE_EQ(comp.size(), (size_t)n);
  auto r = reach(n, adj);
  int k = 0;
  for (int i = 0; i < n; ++i) {
    REQUIRE(comp[i] >= 0 && comp[i] < n);
    k = std::max(k, comp[i] + 1);
  }
  std::vector<int> seen(k, 0);
  for (int c : comp) seen[c] = 1;
  CHECK(std::count(seen.begin(), seen.end(), 0) ==
        0);  // ids are contiguous 0..k-1
  for (int u = 0; u < n; ++u)
    for (int v = 0; v < n; ++v)
      CHECK_EQ(comp[u] == comp[v], (bool)(r[u][v] && r[v][u]));
  for (int u = 0; u < n; ++u)
    for (int v : adj[u]) CHECK(comp[u] <= comp[v]);
}

TEST(basic) {
  Adj adj(8);
  auto e = [&](int a, int b) { adj[a].push_back(b); };
  e(0, 1);
  e(1, 2);
  e(2, 0);
  e(2, 3);
  e(3, 4);
  e(4, 5);
  e(5, 3);
  e(6, 5);
  e(6, 7);
  e(7, 6);
  auto c = scc(8, adj);
  check_scc(8, adj, c);
  CHECK_EQ(*std::max_element(c.begin(), c.end()), 2);
  CHECK_EQ(c[3], 2);  // the sink component must get the last id
  check_scc(1, Adj(1), scc(1, Adj(1)));
  Adj two(2);
  check_scc(2, two, scc(2, two));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 150; ++it) {
    int n = (int)rnd(1, 30), m = (int)rnd(0, 60);
    Adj adj(n);
    for (int i = 0; i < m; ++i)
      adj[rnd(0, n - 1)].push_back((int)rnd(0, n - 1));
    check_scc(n, adj, scc(n, adj));
  }
}

TEST(perf_deep_chain_and_random) {
  using harness::rnd;
  int n = 300000;
  Adj adj(n);
  for (int i = 0; i + 1 < n; ++i) adj[i].push_back(i + 1);  // one long path
  for (int i = 0; i < 300000; ++i)
    adj[rnd(0, n - 1)].push_back((int)rnd(0, n - 1));
  auto c = scc(n, adj);
  REQUIRE_EQ(c.size(), (size_t)n);
  for (int u = 0; u < n; ++u)
    for (int v : adj[u]) CHECK(c[u] <= c[v]);
  // a full cycle: everything is one component
  Adj cyc(n);
  for (int i = 0; i < n; ++i) cyc[i].push_back((i + 1) % n);
  auto c2 = scc(n, cyc);
  CHECK(std::all_of(c2.begin(), c2.end(), [](int x) { return x == 0; }));
}
