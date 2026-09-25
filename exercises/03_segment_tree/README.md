# 03 — Segment Tree (point update, range minimum)

```cpp
struct SegTree {
    SegTree(const vector<long long>& a);   // build in O(n)
    void      set(int i, long long v);     // a[i] = v
    long long query(int l, int r);         // min(a[l..r-1]),  0 <= l < r <= n
};
```

Both operations in O(log n). Either the recursive version or the iterative
bottom-up one (size 2n array) is fine — try whichever you know less well.
