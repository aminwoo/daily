#include <bits/stdc++.h>
using namespace std;

vector<int> find_bridges(int n, const vector<pair<int, int>>& edges) {
  vector<vector<pair<int, int>>> adj(n);
  for (int i = 0; i < (int)edges.size(); ++i) {
    auto [u, v] = edges[i];
    adj[u].push_back({v, i});
    adj[v].push_back({u, i});
  }
  vector<int> discovery_time(n, -1), low_link(n), bridges;
  int timer = 0;
  function<void(int, int)> dfs = [&](int u, int parent_edge) {
    discovery_time[u] = low_link[u] = timer++;
    for (auto [v, id] : adj[u]) {
      if (id == parent_edge) continue;
      if (discovery_time[v] == -1) {
        dfs(v, id);
        low_link[u] = min(low_link[u], low_link[v]);
        if (low_link[v] > discovery_time[u]) bridges.push_back(id);
      } else {
        low_link[u] = min(low_link[u], discovery_time[v]);
      }
    }
  };
  for (int i = 0; i < n; ++i)
    if (discovery_time[i] == -1) dfs(i, -1);
  sort(bridges.begin(), bridges.end());
  return bridges;
}
