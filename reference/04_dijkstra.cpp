#include <bits/stdc++.h>
using namespace std;
const long long INF = LLONG_MAX / 4;

vector<long long> dijkstra(int n,
                           const vector<vector<pair<int, long long>>>& adj,
                           int src) {
  vector<long long> distance(n, INF);
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>>
      pq;
  distance[src] = 0;
  pq.push({0, src});
  while (!pq.empty()) {
    auto [current_distance, u] = pq.top();
    pq.pop();
    if (current_distance != distance[u]) continue;
    for (auto [v, w] : adj[u])
      if (current_distance + w < distance[v]) {
        distance[v] = current_distance + w;
        pq.push({distance[v], v});
      }
  }
  return distance;
}
