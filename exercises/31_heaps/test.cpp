#include "harness.h"
#include SOLUTION

static std::vector<int> slow_topk(const std::vector<int>& a, int k) {
  std::map<int, int> cnt;
  for (int x : a) cnt[x]++;
  std::vector<std::pair<int, int>> v;  // (-freq, value)
  for (auto [x, c] : cnt) v.push_back({-c, x});
  std::sort(v.begin(), v.end());
  std::vector<int> out;
  for (int i = 0; i < k; ++i) out.push_back(v[i].second);
  return out;
}

TEST(basic) {
  KthLargest kl(3, {4, 5, 8, 2});
  CHECK_EQ(kl.add(3), 4);
  CHECK_EQ(kl.add(5), 5);
  CHECK_EQ(kl.add(10), 5);
  CHECK_EQ(kl.add(9), 8);
  CHECK_EQ(kl.add(4), 8);

  MedianFinder mf;
  mf.add(1);
  CHECK_EQ(mf.median(), 1.0);
  mf.add(2);
  CHECK_EQ(mf.median(), 1.5);
  mf.add(3);
  CHECK_EQ(mf.median(), 2.0);
  mf.add(-10);
  CHECK_EQ(mf.median(), 1.5);

  CHECK_EQ(merge_k_sorted({{1, 4, 5}, {1, 3, 4}, {2, 6}}),
           (std::vector<int>{1, 1, 2, 3, 4, 4, 5, 6}));
  CHECK_EQ(merge_k_sorted({}), (std::vector<int>{}));
  CHECK_EQ(merge_k_sorted({{}, {}}), (std::vector<int>{}));
  CHECK_EQ(merge_k_sorted({{}, {7}}), (std::vector<int>{7}));

  CHECK_EQ(top_k_frequent({1, 1, 1, 2, 2, 3}, 2), (std::vector<int>{1, 2}));
  CHECK_EQ(top_k_frequent({5, 4, 4, 5, 3}, 3), (std::vector<int>{4, 5, 3}));
  CHECK_EQ(top_k_frequent({1}, 1), (std::vector<int>{1}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    int k = (int)rnd(1, 6);
    std::vector<int> init((size_t)rnd(0, 8)), all;
    for (auto& x : init) x = (int)rnd(-20, 20);
    KthLargest kl(k, init);
    all = init;
    MedianFinder mf;
    std::vector<int> seen;
    for (int op = 0; op < 40; ++op) {
      int x = (int)rnd(-20, 20);
      all.push_back(x);
      int got = kl.add(x);
      if ((int)all.size() >= k) {
        auto s = all;
        std::sort(s.rbegin(), s.rend());
        CHECK_EQ(got, s[k - 1]);
      }
      seen.push_back(x);
      mf.add(x);
      auto s = seen;
      std::sort(s.begin(), s.end());
      int n = s.size();
      double med = n % 2 ? s[n / 2] : (s[n / 2 - 1] + (double)s[n / 2]) / 2;
      CHECK_EQ(mf.median(), med);
    }
    std::vector<std::vector<int>> lists((size_t)rnd(0, 6));
    std::vector<int> flat;
    for (auto& l : lists) {
      l.resize(rnd(0, 6));
      for (auto& x : l) flat.push_back(x = (int)rnd(-9, 9));
      std::sort(l.begin(), l.end());
    }
    std::sort(flat.begin(), flat.end());
    CHECK_EQ(merge_k_sorted(lists), flat);
    std::vector<int> a((size_t)rnd(1, 30));
    for (auto& x : a) x = (int)rnd(-5, 5);
    int distinct = std::set<int>(a.begin(), a.end()).size();
    int kk = (int)rnd(1, distinct);
    CHECK_EQ(top_k_frequent(a, kk), slow_topk(a, kk));
  }
}

TEST(perf) {
  using harness::rnd;
  KthLargest kl(100000, {});
  long long s = 0;
  for (int i = 0; i < 1000000; ++i) s += kl.add((int)rnd(0, 1000000000));
  CHECK(s > 0);
  MedianFinder mf;
  double m = 0;
  for (int i = 0; i < 1000000; ++i) {
    mf.add((int)rnd(0, 1000000000));
    m = mf.median();
  }
  CHECK(m > 0);
  int k = 2000;
  std::vector<std::vector<int>> lists(k, std::vector<int>(1000));
  for (auto& l : lists) {
    for (auto& x : l) x = (int)rnd(0, 1000000000);
    std::sort(l.begin(), l.end());
  }
  auto merged = merge_k_sorted(lists);  // O(N*k) is 4e9 here
  CHECK_EQ(merged.size(), (size_t)k * 1000);
  CHECK(std::is_sorted(merged.begin(), merged.end()));
  std::vector<int> a(2000000);
  for (auto& x : a) x = (int)rnd(0, 1000000);
  auto top = top_k_frequent(a, 10);
  CHECK_EQ(top.size(), (size_t)10);
}
