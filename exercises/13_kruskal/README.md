# 13 — Kruskal's MST

```cpp
// Undirected weighted graph on 0..n-1 (n >= 1); edges are (u, v, w), may include
// parallel edges and self loops.  Return the total weight of a minimum spanning
// tree, or -1 if the graph is not connected.
long long kruskal(int n, vector<tuple<int,int,long long>> edges);
```

Edge endpoints are valid and the weight of every candidate spanning tree fits
in `long long`.

Sort edges, then sweep with a DSU (write it again from scratch — it's a
one-liner-per-method by now). Target: O(m log m). Perf: n = 2e5, m = 6e5.
