# 26 — Merge sort & inversion counting

```cpp
// merge two ascending vectors into one ascending vector, O(|a| + |b|), stable (a's elements first on ties)
vector<int> merge_sorted(const vector<int>& a, const vector<int>& b);

// number of pairs i < j with a[i] > a[j]  (write your own merge sort; no std::sort / Fenwick)
long long count_inversions(vector<int> a);
```

The classic top-down merge sort with a single scratch buffer allocated once
(allocating a new vector on every level is a common slowdown). Perf: n = 2e6.
