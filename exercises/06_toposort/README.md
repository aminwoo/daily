# 06 — Topological sort (Kahn's algorithm)

```cpp
// Directed graph on 0..n-1 (n >= 1), edges (u, v) meaning u must come before v.
// Return an ordering of all n vertices with every edge pointing forward,
// or an empty vector if the graph contains a cycle.
vector<int> topo_sort(int n, const vector<pair<int,int>>& edges);
```

Use Kahn's algorithm (in-degree counting + queue). If you finish early, also
write the DFS version with colours and confirm it passes the same tests.
Duplicate edges and self loops (a cycle!) may appear.
