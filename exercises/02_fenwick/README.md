# 02 — Fenwick Tree (Binary Indexed Tree)

Point update, prefix sum, in O(log n) each. Indices are 0-based on the outside;
use whatever you like internally.

```cpp
struct Fenwick {
    Fenwick(int n);                 // n elements, all zero
    void      add(int i, long long v);   // a[i] += v            (0 <= i < n)
    long long prefix(int i);             // a[0] + ... + a[i-1]  (0 <= i <= n)
    long long range(int l, int r);       // a[l] + ... + a[r-1]  (0 <= l <= r <= n)
};
```

Bonus (not tested): implement `int lower_bound(long long s)` — the smallest
`i` with `prefix(i+1) >= s` — in O(log n) by descending powers of two.
