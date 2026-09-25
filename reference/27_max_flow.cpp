#include <bits/stdc++.h>
using namespace std;

struct MaxFlow {
  struct Edge {
    int destination;
    long long capacity;
  };

  vector<Edge> edges;
  vector<vector<int>> adjacency;
  vector<int> level;
  vector<int> next_edge;

  explicit MaxFlow(int n) : adjacency(n), level(n), next_edge(n) {}

  void add_edge(int u, int v, long long cap) {
    adjacency[u].push_back(edges.size());
    edges.push_back({v, cap});
    adjacency[v].push_back(edges.size());
    edges.push_back({u, 0});
  }

  bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int id : adjacency[u])
        if (edges[id].capacity > 0 && level[edges[id].destination] < 0) {
          level[edges[id].destination] = level[u] + 1;
          q.push(edges[id].destination);
        }
    }
    return level[t] >= 0;
  }

  long long dfs(int u, int t, long long available) {
    if (u == t) return available;
    for (int& i = next_edge[u]; i < (int)adjacency[u].size(); ++i) {
      int id = adjacency[u][i];
      Edge& edge = edges[id];
      if (edge.capacity <= 0 || level[edge.destination] != level[u] + 1) {
        continue;
      }
      long long pushed =
          dfs(edge.destination, t, min(available, edge.capacity));
      if (pushed > 0) {
        edge.capacity -= pushed;
        edges[id ^ 1].capacity += pushed;
        return pushed;
      }
    }
    return 0;
  }

  long long max_flow(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
      fill(next_edge.begin(), next_edge.end(), 0);
      while (long long pushed = dfs(s, t, LLONG_MAX)) flow += pushed;
    }
    return flow;
  }
};
