#include "harness.h"
#include SOLUTION

static long long cross(P o, P a, P b) {
  return (a.first - o.first) * (b.second - o.second) -
         (a.second - o.second) * (b.first - o.first);
}

static bool on_segment(P a, P b, P p) {
  return cross(a, b, p) == 0 && std::min(a.first, b.first) <= p.first &&
         p.first <= std::max(a.first, b.first) &&
         std::min(a.second, b.second) <= p.second &&
         p.second <= std::max(a.second, b.second);
}

static bool in_triangle(P a, P b, P c,
                        P p) {  // closed triangle, any orientation
  long long d1 = cross(a, b, p), d2 = cross(b, c, p), d3 = cross(c, a, p);
  bool neg = d1 < 0 || d2 < 0 || d3 < 0, pos = d1 > 0 || d2 > 0 || d3 > 0;
  return !(neg && pos);
}

// p is a hull vertex iff it is not inside the convex hull of the OTHER points
// (closed)
static std::set<P> slow_vertices(std::vector<P> pts) {
  std::sort(pts.begin(), pts.end());
  pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
  int n = pts.size();
  std::set<P> res;
  for (int i = 0; i < n; ++i) {
    bool inside = false;
    for (int a = 0; a < n && !inside; ++a) {
      if (a == i) continue;
      for (int b = a + 1; b < n && !inside; ++b) {
        if (b == i) continue;
        if (on_segment(pts[a], pts[b], pts[i])) {
          inside = true;
          break;
        }
        for (int c = b + 1; c < n && !inside; ++c) {
          if (c == i) continue;
          if (cross(pts[a], pts[b], pts[c]) != 0 &&
              in_triangle(pts[a], pts[b], pts[c], pts[i]))
            inside = true;
        }
      }
    }
    if (!inside) res.insert(pts[i]);
  }
  return res;
}

static void check_hull(const std::vector<P>& pts, const std::vector<P>& hull) {
  auto want = slow_vertices(pts);
  std::set<P> got(hull.begin(), hull.end());
  CHECK_EQ(got.size(), hull.size());  // no duplicates
  CHECK(got == want);
  if (hull.size() >= 1)
    CHECK_EQ(hull[0], *std::min_element(pts.begin(), pts.end(), [](P a, P b) {
               return std::make_pair(a.second, a.first) <
                      std::make_pair(b.second, b.first);
             }));
  if (hull.size() >= 3)
    for (size_t i = 0; i < hull.size(); ++i)
      CHECK(cross(hull[i], hull[(i + 1) % hull.size()],
                  hull[(i + 2) % hull.size()]) > 0);  // strictly CCW
}

TEST(basic) {
  CHECK_EQ(convex_hull({}), (std::vector<P>{}));
  CHECK_EQ(convex_hull({{3, 3}}), (std::vector<P>{{3, 3}}));
  CHECK_EQ(convex_hull({{3, 3}, {3, 3}}), (std::vector<P>{{3, 3}}));
  CHECK_EQ(convex_hull({{0, 0}, {5, 5}, {2, 2}, {1, 1}}),
           (std::vector<P>{{0, 0}, {5, 5}}));
  CHECK_EQ(
      convex_hull({{0, 0}, {4, 0}, {4, 4}, {0, 4}, {2, 2}, {2, 0}, {4, 2}}),
      (std::vector<P>{{0, 0}, {4, 0}, {4, 4}, {0, 4}}));
  CHECK_EQ(convex_hull({{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {0, 0}}),
           (std::vector<P>{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 400; ++it) {
    int n = (int)rnd(0, 12),
        range = (int)rnd(
            1, 6);  // small range => lots of collinear/duplicate points
    std::vector<P> pts(n);
    for (auto& p : pts) p = {rnd(-range, range), rnd(-range, range)};
    check_hull(pts, convex_hull(pts));
  }
}

TEST(large_coordinates) {
  std::vector<P> pts = {{-1000000000, -1000000000},
                        {1000000000, -1000000000},
                        {1000000000, 1000000000},
                        {-1000000000, 1000000000},
                        {0, 0},
                        {999999999, 999999998}};
  CHECK_EQ(convex_hull(pts), (std::vector<P>{{-1000000000, -1000000000},
                                             {1000000000, -1000000000},
                                             {1000000000, 1000000000},
                                             {-1000000000, 1000000000}}));
}

TEST(perf) {
  using harness::rnd;
  int n = 500000;
  std::vector<P> pts(n);
  for (auto& p : pts)
    p = {rnd(-1000000000, 1000000000), rnd(-1000000000, 1000000000)};
  auto h = convex_hull(pts);
  CHECK(h.size() > 10 && h.size() < 200);
  for (size_t i = 0; i < h.size(); ++i)
    CHECK(cross(h[i], h[(i + 1) % h.size()], h[(i + 2) % h.size()]) > 0);
}
