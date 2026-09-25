#include <bits/stdc++.h>
using namespace std;

long long kruskal(int n, vector<tuple<int, int, long long>> edges) {
  sort(edges.begin(), edges.end(),
       [](auto& a, auto& b) { return get<2>(a) < get<2>(b); });
  vector<int> parent(n);
  vector<int> component_size(n, 1);
  iota(parent.begin(), parent.end(), 0);
  auto find = [&](int x) {
    while (parent[x] != x) {
      parent[x] = parent[parent[x]];
      x = parent[x];
    }
    return x;
  };
  long long total = 0;
  int edges_used = 0;
  for (auto [u, v, w] : edges) {
    int root_u = find(u);
    int root_v = find(v);
    if (root_u == root_v) continue;
    if (component_size[root_u] < component_size[root_v]) swap(root_u, root_v);
    parent[root_v] = root_u;
    component_size[root_u] += component_size[root_v];
    total += w;
    ++edges_used;
  }
  return edges_used == n - 1 ? total : -1;
}
