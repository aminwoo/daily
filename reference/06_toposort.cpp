#include <bits/stdc++.h>
using namespace std;

vector<int> topo_sort(int n, const vector<pair<int, int>>& edges) {
  vector<vector<int>> adjacency(n);
  vector<int> indegree(n, 0);
  for (auto [u, v] : edges) {
    adjacency[u].push_back(v);
    ++indegree[v];
  }
  vector<int> order;
  order.reserve(n);
  for (int i = 0; i < n; ++i)
    if (indegree[i] == 0) order.push_back(i);
  for (size_t head = 0; head < order.size(); ++head) {
    for (int v : adjacency[order[head]]) {
      if (--indegree[v] == 0) order.push_back(v);
    }
  }
  return static_cast<int>(order.size()) == n ? order : vector<int>{};
}
