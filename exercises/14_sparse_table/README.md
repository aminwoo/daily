# 14 — Sparse table (static RMQ)

```cpp
struct SparseTable {
    SparseTable(const vector<int>& a);   // O(n log n) build
    int query(int l, int r);             // min(a[l..r-1]),  0 <= l < r <= n,  O(1)
};
```

Precompute `lg[i] = floor(log2 i)` (or use `std::bit_width`/`__builtin_clz`)
so the query really is two lookups. Perf: n = 1e6, 3e6 queries.
