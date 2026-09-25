# 04 — Dijkstra

Single-source shortest paths with non-negative edge weights, using a binary
heap (`std::priority_queue`) with lazy deletion.

```cpp
const long long INF = LLONG_MAX / 4;   // already defined in the skeleton

// n >= 1, adj.size() == n, and 0 <= src, v < n.
// adj[u] = list of (v, w) for directed edges u -> v with weight w >= 0.
// returns dist[v] for every v; INF if v is unreachable
vector<long long> dijkstra(int n, const vector<vector<pair<int,long long>>>& adj, int src);
```

All finite path lengths are guaranteed to be less than `INF`, and additions
of a reachable distance and an edge weight fit in `long long`.

Target: O((n + m) log n). The perf test uses n = 2e5, m = 6e5, so an O(n²)
"pick the min by scanning" version will time out.
