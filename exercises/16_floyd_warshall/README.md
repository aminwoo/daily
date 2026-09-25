# 16 — Floyd–Warshall

```cpp
const long long INF = LLONG_MAX / 4;   // defined in the skeleton

// Directed graph on 0..n-1 (n >= 0), edges (u, v, w) with valid endpoints,
// possibly negative w, but no negative cycles.
// Parallel edges: keep the cheapest.  Return dist[i][j] (INF if unreachable, dist[i][i] = 0).
vector<vector<long long>> floyd_warshall(int n, const vector<tuple<int,int,long long>>& edges);
```

O(n³), the loop order is k-i-j. Don't relax through an INF (`INF + negative`
silently becomes a "real" distance). Perf: n = 400.
All finite path lengths and relaxation additions fit strictly inside
`(-INF, INF)`.
