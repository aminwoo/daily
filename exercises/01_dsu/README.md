# 01 — Disjoint Set Union (Union–Find)

Implement a DSU over elements `0..n-1` with **path compression** and
**union by size** (or rank).

```cpp
struct DSU {
    DSU(int n);               // every element in its own set
    int  find(int x);         // representative of x's set
    bool unite(int a, int b); // merge the sets; false if already in the same set
    int  size(int x);         // number of elements in x's set
    int  components();        // number of disjoint sets
};
```

Target: (amortised) near-constant time per operation. The perf test does
~1e6 operations on a chain, which will time out without both optimisations.
