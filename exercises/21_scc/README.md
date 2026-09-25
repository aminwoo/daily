# 21 — Strongly connected components (Tarjan or Kosaraju)

```cpp
// Directed graph on 0..n-1 (n >= 0), with adj.size() == n and valid neighbors.
// Returns comp[v] = id of v's SCC, where the ids
// 0..k-1 are numbered in topological order of the condensation:
// for every edge u -> v,  comp[u] <= comp[v].
vector<int> scc(int n, const vector<vector<int>>& adj);
```

Tarjan produces components in *reverse* topological order, so number them
`k-1, k-2, ...` as you pop them. Kosaraju's second pass produces them in
topological order directly. Perf: n = 3e5 with a long chain (deep recursion
is OK — the runner raises the stack limit).
