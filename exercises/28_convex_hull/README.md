# 28 — Convex hull (Andrew's monotone chain)

```cpp
using P = pair<long long, long long>;    // (x, y), |coords| <= 1e9

// Vertices of the convex hull in counter-clockwise order, starting from the
// lowest point (smallest y, then smallest x).  Points on hull edges (collinear)
// and interior points are excluded.  Duplicates may appear in the input.
//   0 points -> {},  1 distinct point -> {p},  all collinear -> the two endpoints.
vector<P> convex_hull(vector<P> pts);
```

Sort, build the lower and upper chains with a cross-product test, pop while
the turn is not strictly left. Use `long long` for the cross product
(coordinates up to 1e9 → products up to 4e18, which fits). Perf: 5e5 points.
