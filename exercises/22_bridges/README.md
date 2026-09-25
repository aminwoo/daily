# 22 — Bridges (Tarjan low-link)

```cpp
// Undirected multigraph on 0..n-1 (n >= 0); endpoints are valid and
// edges[i] = (u, v).  Parallel edges and self loops allowed.
// Return the indices of all bridge edges, ascending.
vector<int> find_bridges(int n, const vector<pair<int,int>>& edges);
```

DFS with `tin`/`low`; edge (p -> v) is a bridge iff `low[v] > tin[p]`.
Skip the edge you came *in on* by **edge index**, not by parent vertex —
otherwise parallel edges are misclassified (the tests check this).
Perf: n = 3e5, m = 5e5.
