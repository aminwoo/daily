# 27 — Maximum flow (Dinic)

```cpp
struct MaxFlow {
    MaxFlow(int n);                                    // n >= 2
    void      add_edge(int u, int v, long long cap);   // valid endpoints; directed, cap >= 0
    long long max_flow(int s, int t);                  // s != t
};
```

The value of the maximum flow and all residual-capacity updates fit in
`long long`.

Store edges in one flat array with the reverse edge at index `e ^ 1`. Dinic:
BFS to build the level graph, then DFS with a per-node "current edge"
pointer to find blocking flows. The tests check against a brute-force min
cut, so you can use any correct algorithm — but Edmonds–Karp will be
noticeably slower on the perf test.
