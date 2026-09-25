#include <bits/stdc++.h>
using namespace std;

struct LCA {
  int levels;
  vector<vector<int>> ancestor;
  vector<int> depths;

  LCA(int n, const vector<vector<int>>& adj, int root) {
    levels = 1;
    while ((1 << levels) < n) ++levels;
    ancestor.assign(levels, vector<int>(n, root));
    depths.assign(n, 0);
    vector<int> stack = {root};
    vector<char> seen(n, 0);
    seen[root] = 1;
    while (!stack.empty()) {
      int u = stack.back();
      stack.pop_back();
      for (int v : adj[u])
        if (!seen[v]) {
          seen[v] = 1;
          ancestor[0][v] = u;
          depths[v] = depths[u] + 1;
          stack.push_back(v);
        }
    }
    for (int level = 1; level < levels; ++level) {
      for (int v = 0; v < n; ++v) {
        ancestor[level][v] = ancestor[level - 1][ancestor[level - 1][v]];
      }
    }
  }

  int depth(int u) const { return depths[u]; }

  int kth_ancestor(int u, int k) {
    if (k > depths[u]) return -1;
    for (int level = 0; k > 0; ++level, k >>= 1) {
      if (k & 1) u = ancestor[level][u];
    }
    return u;
  }

  int lca(int u, int v) {
    if (depths[u] < depths[v]) swap(u, v);
    u = kth_ancestor(u, depths[u] - depths[v]);
    if (u == v) return u;
    for (int level = levels - 1; level >= 0; --level)
      if (ancestor[level][u] != ancestor[level][v]) {
        u = ancestor[level][u];
        v = ancestor[level][v];
      }
    return ancestor[0][u];
  }

  int dist(int u, int v) {
    return depths[u] + depths[v] - 2 * depths[lca(u, v)];
  }
};
