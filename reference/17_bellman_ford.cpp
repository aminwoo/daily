#include <bits/stdc++.h>
using namespace std;
const long long INF = LLONG_MAX / 4;

bool bellman_ford(int n, const vector<tuple<int, int, long long>>& edges,
                  int src, vector<long long>& dist) {
  dist.assign(n, INF);
  dist[src] = 0;
  for (int round = 0; round < n; ++round) {
    bool changed = false;
    for (auto [u, v, w] : edges)
      if (dist[u] < INF && dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        changed = true;
      }
    if (!changed) return true;
    if (round == n - 1) return false;
  }
  return true;
}
