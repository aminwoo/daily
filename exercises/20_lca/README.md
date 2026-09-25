# 20 — LCA via binary lifting

```cpp
struct LCA {
    // adj is an undirected tree on 0..n-1 (n >= 1)
    LCA(int n, const vector<vector<int>>& adj, int root);
    int depth(int u);              // root has depth 0
    int kth_ancestor(int u, int k);// k >= 0; -1 if k > depth(u)
    int lca(int u, int v);
    int dist(int u, int v);        // number of edges on the path
};
```

`up[j][u]` = 2^j-th ancestor of u (root's ancestor is the root itself, or -1
— pick one and be consistent). Preprocessing O(n log n), queries O(log n).

The perf test uses a path of 3e5 nodes: a recursive DFS is fine (the runner
raises the stack limit) but an iterative one is worth knowing.
