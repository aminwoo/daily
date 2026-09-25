# 23 — Segment tree with lazy propagation (range add, range sum)

```cpp
struct LazySegTree {
    LazySegTree(const vector<long long>& a);    // a is non-empty
    void      add(int l, int r, long long v);   // a[i] += v for 0 <= l <= i < r <= n
    long long sum(int l, int r);                // a[l] + ... + a[r-1], 0 <= l <= r <= n
};
```

Empty ranges are valid no-ops / zero-sum queries. All stored sums and updates
are guaranteed to fit in `long long`.

Each node stores the sum of its range and a pending "add to every element"
value. Push the pending value to the children before descending. Both
operations O(log n). Perf: n = 2e5, 4e5 operations with wide ranges.

(A Fenwick-tree trick can do this too — feel free to try it afterwards.)
