#include <bits/stdc++.h>
using namespace std;

struct DSU {
  vector<int> parent;
  vector<int> component_size;
  int component_count;

  explicit DSU(int n) : parent(n), component_size(n, 1), component_count(n) {
    iota(parent.begin(), parent.end(), 0);
  }

  int find(int x) {
    while (parent[x] != x) {
      parent[x] = parent[parent[x]];
      x = parent[x];
    }
    return x;
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (component_size[a] < component_size[b]) swap(a, b);
    parent[b] = a;
    component_size[a] += component_size[b];
    --component_count;
    return true;
  }

  int size(int x) { return component_size[find(x)]; }

  int components() const { return component_count; }
};
