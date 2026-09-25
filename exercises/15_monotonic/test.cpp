#include "harness.h"
#include SOLUTION

static std::vector<int> slow_win(const std::vector<int>& a, int k) {
  std::vector<int> r;
  for (size_t i = 0; i + k <= a.size(); ++i)
    r.push_back(*std::max_element(a.begin() + i, a.begin() + i + k));
  return r;
}

static std::vector<int> slow_ps(const std::vector<int>& a) {
  std::vector<int> r(a.size(), -1);
  for (size_t i = 0; i < a.size(); ++i)
    for (int j = i - 1; j >= 0; --j)
      if (a[j] < a[i]) {
        r[i] = j;
        break;
      }
  return r;
}

static long long slow_rect(const std::vector<int>& h) {
  long long best = 0;
  for (size_t i = 0; i < h.size(); ++i) {
    int mn = h[i];
    for (size_t j = i; j < h.size(); ++j) {
      mn = std::min(mn, h[j]);
      best = std::max(best, (long long)mn * (long long)(j - i + 1));
    }
  }
  return best;
}

TEST(basic) {
  CHECK_EQ(sliding_window_max({1, 3, -1, -3, 5, 3, 6, 7}, 3),
           (std::vector<int>{3, 3, 5, 5, 6, 7}));
  CHECK_EQ(sliding_window_max({4, 2}, 1), (std::vector<int>{4, 2}));
  CHECK_EQ(sliding_window_max({4, 2, 9}, 3), (std::vector<int>{9}));
  CHECK_EQ(previous_smaller({3, 1, 4, 1, 5, 9, 2, 6}),
           (std::vector<int>{-1, -1, 1, -1, 3, 4, 3, 6}));
  CHECK_EQ(previous_smaller({2, 2, 2}), (std::vector<int>{-1, -1, -1}));
  CHECK_EQ(largest_rectangle({2, 1, 5, 6, 2, 3}), 10LL);
  CHECK_EQ(largest_rectangle({}), 0LL);
  CHECK_EQ(largest_rectangle({0, 0}), 0LL);
  CHECK_EQ(largest_rectangle({5}), 5LL);
  CHECK_EQ(largest_rectangle({3, 3, 3}), 9LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(1, 30);
    std::vector<int> a(n);
    for (auto& x : a) x = (int)rnd(0, 8);
    int k = (int)rnd(1, n);
    CHECK_EQ(sliding_window_max(a, k), slow_win(a, k));
    CHECK_EQ(previous_smaller(a), slow_ps(a));
    CHECK_EQ(largest_rectangle(a), slow_rect(a));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 2000000;
  std::vector<int> a(n);
  for (auto& x : a) x = (int)rnd(0, 1000000000);
  auto w = sliding_window_max(a, 100000);
  CHECK_EQ(w.size(), (size_t)n - 100000 + 1);
  std::vector<int> desc(n);
  for (int i = 0; i < n; ++i) desc[i] = n - i;  // worst case for a naive stack
  auto ps = previous_smaller(desc);
  CHECK_EQ(ps[n - 1], -1);
  CHECK(largest_rectangle(desc) > 0);
  std::vector<int> asc(n);
  for (int i = 0; i < n; ++i) asc[i] = i + 1;
  CHECK_EQ(largest_rectangle(asc), 1000001000000LL);
}
