#include "harness.h"
#include SOLUTION

using Edges = std::vector<std::pair<int, int>>;

static int components(int n, const Edges& e, int skip) {
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), 0);
  auto find = [&](int x) {
    while (p[x] != x) x = p[x] = p[p[x]];
    return x;
  };
  int c = n;
  for (int i = 0; i < (int)e.size(); ++i) {
    if (i == skip) continue;
    int a = find(e[i].first), b = find(e[i].second);
    if (a != b) {
      p[a] = b;
      --c;
    }
  }
  return c;
}

static std::vector<int> slow(int n, const Edges& e) {
  std::vector<int> r;
  int base = components(n, e, -1);
  for (int i = 0; i < (int)e.size(); ++i)
    if (components(n, e, i) > base) r.push_back(i);
  return r;
}

TEST(basic) {
  //  0-1-2 triangle, 2-3 bridge, 3-4 and 3-4 parallel (not bridges), 4-5
  //  bridge, 5-5 loop
  Edges e = {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {3, 4}, {4, 5}, {5, 5}};
  CHECK_EQ(find_bridges(6, e), (std::vector<int>{3, 6}));
  CHECK_EQ(find_bridges(1, {}), (std::vector<int>{}));
  CHECK_EQ(find_bridges(2, {{0, 1}}), (std::vector<int>{0}));
  CHECK_EQ(find_bridges(2, {{0, 1}, {1, 0}}), (std::vector<int>{}));
  CHECK_EQ(find_bridges(4, {{0, 1}, {2, 3}}),
           (std::vector<int>{0, 1}));  // disconnected
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(1, 12), m = (int)rnd(0, 16);
    Edges e;
    for (int i = 0; i < m; ++i)
      e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1)});
    CHECK_EQ(find_bridges(n, e), slow(n, e));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 300000;
  Edges e;
  for (int i = 0; i + 1 < n; ++i) e.push_back({i, i + 1});  // path: all bridges
  auto b = find_bridges(n, e);
  CHECK_EQ(b.size(), (size_t)n - 1);
  for (int i = 0; i < 200000; ++i)
    e.push_back({(int)rnd(0, n - 1), (int)rnd(0, n - 1)});
  b = find_bridges(n, e);
  CHECK(b.size() < (size_t)n - 1);
  CHECK(std::is_sorted(b.begin(), b.end()));
}
