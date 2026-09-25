# 17 — Bellman–Ford with negative-cycle detection

```cpp
const long long INF = LLONG_MAX / 4;   // defined in the skeleton

// Directed graph on 0..n-1 (n >= 1), valid edge endpoints and src, w may be negative.
// Fills dist (size n, INF for unreachable) and returns true;
// returns false if a negative cycle is reachable from src (dist is then unspecified).
bool bellman_ford(int n, const vector<tuple<int,int,long long>>& edges, int src, vector<long long>& dist);
```

Every relaxation addition fits in `long long`; when no reachable negative
cycle exists, every finite shortest-path length lies strictly inside
`(-INF, INF)`.

n-1 rounds of relaxation, then one more round: any improvement means a
reachable negative cycle. Skip edges whose tail is still INF. Stop early
if a round changes nothing. Perf: n = 2000, m = 2e5 — early exit matters.
