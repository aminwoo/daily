#include "harness.h"
#include SOLUTION

using Adj = std::vector<std::vector<int>>;

static Adj random_tree(int n, std::vector<int>& par, int root) {
  // random parent array, then relabel so `root` is the root: simplest is to
  // root at 0 and map
  Adj adj(n);
  par.assign(n, -1);
  for (int i = 1; i < n; ++i) {
    int p = (int)harness::rnd(0, i - 1);
    adj[i].push_back(p);
    adj[p].push_back(i);
  }
  // reroot at `root` by walking BFS from root
  std::vector<int> order;
  par.assign(n, -1);
  std::vector<char> seen(n, 0);
  seen[root] = 1;
  order.push_back(root);
  for (size_t h = 0; h < order.size(); ++h)
    for (int v : adj[order[h]])
      if (!seen[v]) {
        seen[v] = 1;
        par[v] = order[h];
        order.push_back(v);
      }
  return adj;
}

static int slow_depth(const std::vector<int>& par, int u) {
  int d = 0;
  while (par[u] != -1) {
    u = par[u];
    ++d;
  }
  return d;
}

static int slow_lca(const std::vector<int>& par, int u, int v) {
  std::set<int> anc;
  for (int x = u; x != -1; x = par[x]) anc.insert(x);
  for (int x = v;; x = par[x])
    if (anc.count(x)) return x;
}

TEST(basic) {
  //          0
  //       ╭──┼──╮
  //       1  2  3
  //      ╭┴╮    ╰╮
  //      4 5     6
  //              ╰╮
  //               7
  Adj adj(8);
  auto e = [&](int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
  };
  e(0, 1);
  e(0, 2);
  e(0, 3);
  e(1, 4);
  e(1, 5);
  e(3, 6);
  e(6, 7);
  LCA t(8, adj, 0);
  CHECK_EQ(t.depth(0), 0);
  CHECK_EQ(t.depth(7), 3);
  CHECK_EQ(t.depth(4), 2);
  CHECK_EQ(t.lca(4, 5), 1);
  CHECK_EQ(t.lca(4, 7), 0);
  CHECK_EQ(t.lca(7, 3), 3);
  CHECK_EQ(t.lca(2, 2), 2);
  CHECK_EQ(t.dist(4, 7), 5);
  CHECK_EQ(t.dist(6, 7), 1);
  CHECK_EQ(t.dist(5, 5), 0);
  CHECK_EQ(t.kth_ancestor(7, 0), 7);
  CHECK_EQ(t.kth_ancestor(7, 2), 3);
  CHECK_EQ(t.kth_ancestor(7, 3), 0);
  CHECK_EQ(t.kth_ancestor(7, 4), -1);
  CHECK_EQ(t.kth_ancestor(0, 1), -1);
  LCA single(1, Adj(1), 0);
  CHECK_EQ(single.lca(0, 0), 0);
  CHECK_EQ(single.depth(0), 0);
  CHECK_EQ(single.kth_ancestor(0, 5), -1);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 60; ++it) {
    int n = (int)rnd(1, 50), root = (int)rnd(0, n - 1);
    std::vector<int> par;
    Adj adj = random_tree(n, par, root);
    LCA t(n, adj, root);
    for (int q = 0; q < 200; ++q) {
      int u = (int)rnd(0, n - 1), v = (int)rnd(0, n - 1);
      CHECK_EQ(t.depth(u), slow_depth(par, u));
      int l = slow_lca(par, u, v);
      CHECK_EQ(t.lca(u, v), l);
      CHECK_EQ(t.dist(u, v), slow_depth(par, u) + slow_depth(par, v) -
                                 2 * slow_depth(par, l));
      int k = (int)rnd(0, n + 1);
      int x = u;
      for (int i = 0; i < k && x != -1; ++i) x = par[x];
      CHECK_EQ(t.kth_ancestor(u, k), x);
    }
  }
}

TEST(perf_path) {
  using harness::rnd;
  int n = 300000;
  Adj adj(n);
  for (int i = 0; i + 1 < n; ++i) {
    adj[i].push_back(i + 1);
    adj[i + 1].push_back(i);
  }
  LCA t(n, adj, 0);
  long long chk = 0;
  for (int q = 0; q < 300000; ++q) {
    int u = (int)rnd(0, n - 1), v = (int)rnd(0, n - 1);
    chk += t.lca(u, v);
    CHECK_EQ(t.dist(u, v), std::abs(u - v));
  }
  CHECK(chk > 0);
  CHECK_EQ(t.kth_ancestor(n - 1, n - 1), 0);
}
