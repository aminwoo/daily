#include "harness.h"
#include SOLUTION

static int slow_lis(const std::vector<int>& a) {
  int n = a.size(), best = 0;
  std::vector<int> dp(n, 1);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j)
      if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
    best = std::max(best, dp[i]);
  }
  return best;
}

static bool is_subsequence(const std::vector<int>& sub,
                           const std::vector<int>& a) {
  size_t k = 0;
  for (size_t i = 0; i < a.size() && k < sub.size(); ++i)
    if (a[i] == sub[k]) ++k;
  return k == sub.size();
}

static bool strictly_increasing(const std::vector<int>& s) {
  for (size_t i = 1; i < s.size(); ++i)
    if (s[i - 1] >= s[i]) return false;
  return true;
}

TEST(basic) {
  CHECK_EQ(lis_length({10, 9, 2, 5, 3, 7, 101, 18}), 4);
  CHECK_EQ(lis_length({7, 7, 7}), 1);
  CHECK_EQ(lis_length({}), 0);
  CHECK_EQ(lis_length({1, 2, 3, 4}), 4);
  CHECK_EQ(lis_length({4, 3, 2, 1}), 1);
  CHECK_EQ(lis_sequence({}), (std::vector<int>{}));
  auto s = lis_sequence({10, 9, 2, 5, 3, 7, 101, 18});
  CHECK_EQ(s.size(), (size_t)4);
  CHECK(strictly_increasing(s));
  CHECK(is_subsequence(s, {10, 9, 2, 5, 3, 7, 101, 18}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 400; ++it) {
    int n = (int)rnd(0, 40);
    std::vector<int> a(n);
    for (auto& x : a) x = (int)rnd(-10, 10);
    int want = slow_lis(a);
    CHECK_EQ(lis_length(a), want);
    auto s = lis_sequence(a);
    CHECK_EQ((int)s.size(), want);
    CHECK(strictly_increasing(s));
    CHECK(is_subsequence(s, a));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 1000000;
  std::vector<int> a(n);
  for (auto& x : a) x = (int)rnd(0, 1000000000);
  int L = lis_length(a);
  CHECK(L > 1000 && L < 5000);  // ~2*sqrt(n) for random permutations
  auto s = lis_sequence(a);
  CHECK_EQ((int)s.size(), L);
  CHECK(strictly_increasing(s));
  CHECK(is_subsequence(s, a));
}
