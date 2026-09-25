#include <bits/stdc++.h>
using namespace std;
using P = pair<long long, long long>;

static long long cross_product(P origin, P a, P b) {
  return (a.first - origin.first) * (b.second - origin.second) -
         (a.second - origin.second) * (b.first - origin.first);
}

vector<P> convex_hull(vector<P> pts) {
  sort(pts.begin(), pts.end());
  pts.erase(unique(pts.begin(), pts.end()), pts.end());
  int n = pts.size();
  if (n <= 1) return pts;
  vector<P> hull(2 * n);
  int hull_size = 0;
  for (int i = 0; i < n; ++i) {
    while (hull_size >= 2 && cross_product(hull[hull_size - 2],
                                           hull[hull_size - 1], pts[i]) <= 0) {
      --hull_size;
    }
    hull[hull_size++] = pts[i];
  }
  const int upper_start = hull_size + 1;
  for (int i = n - 2; i >= 0; --i) {
    while (hull_size >= upper_start &&
           cross_product(hull[hull_size - 2], hull[hull_size - 1], pts[i]) <=
               0) {
      --hull_size;
    }
    hull[hull_size++] = pts[i];
  }
  hull.resize(hull_size - 1);
  // rotate so that the lowest (then leftmost) point is first
  int best = 0;
  for (int i = 1; i < (int)hull.size(); ++i)
    if (make_pair(hull[i].second, hull[i].first) <
        make_pair(hull[best].second, hull[best].first))
      best = i;
  rotate(hull.begin(), hull.begin() + best, hull.end());
  return hull;
}
