#include "harness.h"
#include SOLUTION

// union computed on a coordinate line, then read back as intervals
static std::vector<Iv> slow_merge(const std::vector<Iv>& v) {
  std::set<long long> covered;  // unit cells [x, x+1)
  for (auto [l, r] : v)
    for (long long x = l; x < r; ++x) covered.insert(x);
  std::vector<Iv> out;
  for (long long x : covered) {
    if (!out.empty() && out.back().second == x)
      out.back().second = x + 1;
    else
      out.push_back({x, x + 1});
  }
  return out;
}

static int slow_rooms(const std::vector<Iv>& v) {
  int best = 0;
  for (long long x = -1; x <= 30; ++x) {
    int c = 0;
    for (auto [l, r] : v)
      if (l <= x && x < r) ++c;
    best = std::max(best, c);
  }
  return best;
}

static int slow_max_disjoint(const std::vector<Iv>& v) {
  int n = v.size(), best = 0;
  for (int m = 0; m < (1 << n); ++m) {
    bool ok = true;
    for (int i = 0; i < n && ok; ++i)
      for (int j = i + 1; j < n && ok; ++j)
        if ((m >> i & 1) && (m >> j & 1) &&
            std::max(v[i].first, v[j].first) <
                std::min(v[i].second, v[j].second))
          ok = false;
    if (ok) best = std::max(best, __builtin_popcount(m));
  }
  return best;
}

static std::vector<Iv> rand_ivs(int n, int lo, int hi) {
  std::vector<Iv> v;
  for (int i = 0; i < n; ++i) {
    long long l = harness::rnd(lo, hi - 1);
    long long r = harness::rnd(l + 1, hi);
    v.push_back({l, r});
  }
  return v;
}

TEST(basic) {
  CHECK_EQ(merge_intervals({{1, 3}, {2, 6}, {8, 10}, {15, 18}}),
           (std::vector<Iv>{{1, 6}, {8, 10}, {15, 18}}));
  CHECK_EQ(merge_intervals({{1, 4}, {4, 5}}), (std::vector<Iv>{{1, 5}}));
  CHECK_EQ(merge_intervals({}), (std::vector<Iv>{}));
  CHECK_EQ(insert_interval({{1, 3}, {6, 9}}, {2, 5}),
           (std::vector<Iv>{{1, 5}, {6, 9}}));
  CHECK_EQ(insert_interval({{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}}, {4, 8}),
           (std::vector<Iv>{{1, 2}, {3, 10}, {12, 16}}));
  CHECK_EQ(insert_interval({}, {5, 7}), (std::vector<Iv>{{5, 7}}));
  CHECK_EQ(insert_interval({{1, 5}}, {6, 8}),
           (std::vector<Iv>{{1, 5}, {6, 8}}));
  CHECK_EQ(min_rooms({{0, 30}, {5, 10}, {15, 20}}), 2);
  CHECK_EQ(min_rooms({{7, 10}, {2, 4}}), 1);
  CHECK_EQ(min_rooms({{1, 3}, {3, 5}}), 1);
  CHECK_EQ(min_rooms({}), 0);
  CHECK_EQ(max_non_overlapping({{1, 2}, {2, 3}, {3, 4}, {1, 3}}), 3);
  CHECK_EQ(max_non_overlapping({{1, 2}, {1, 2}, {1, 2}}), 1);
  CHECK_EQ(max_non_overlapping({{1, 100}, {2, 3}, {4, 5}}), 2);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    auto v = rand_ivs((int)rnd(0, 12), 0, 30);
    auto m = merge_intervals(v);
    CHECK_EQ(m, slow_merge(v));
    Iv x = {rnd(0, 29), 0};
    x.second = rnd(x.first + 1, 30);
    auto w = m;
    w.push_back(x);
    CHECK_EQ(insert_interval(m, x), slow_merge(w));
    CHECK_EQ(min_rooms(v), slow_rooms(v));
    CHECK_EQ(max_non_overlapping(v), slow_max_disjoint(v));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 1000000;
  auto v = rand_ivs(n, 0, 1000000000);
  auto m = merge_intervals(v);
  CHECK(m.size() >= 1);
  CHECK(min_rooms(v) >= 1);
  CHECK(max_non_overlapping(v) >= 1);
  std::vector<Iv> d;
  for (int i = 0; i < n; ++i) d.push_back({2LL * i, 2LL * i + 1});
  auto r = insert_interval(d, {1, 2LL * n - 3});
  CHECK_EQ(r.size(), (size_t)2);
  CHECK_EQ(min_rooms(d), 1);
  CHECK_EQ(max_non_overlapping(d), n);
}
