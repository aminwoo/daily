#include "harness.h"
#include SOLUTION

static int slow_unique(const std::string& s) {
  int best = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    bool seen[256] = {false};
    for (size_t j = i; j < s.size(); ++j) {
      if (seen[(unsigned char)s[j]]) break;
      seen[(unsigned char)s[j]] = true;
      best = std::max(best, (int)(j - i + 1));
    }
  }
  return best;
}

static int slow_window(const std::string& s, const std::string& t) {
  if (t.empty()) return 0;
  int best = 0;
  for (size_t i = 0; i < s.size(); ++i)
    for (size_t j = i; j < s.size(); ++j) {
      int cnt[256] = {0};
      for (size_t k = i; k <= j; ++k) cnt[(unsigned char)s[k]]++;
      for (unsigned char c : t) cnt[c]--;
      bool ok = true;
      for (int c = 0; c < 256; ++c)
        if (cnt[c] < 0) ok = false;
      if (ok && (best == 0 || (int)(j - i + 1) < best)) best = j - i + 1;
    }
  return best;
}

static long long slow_kadane(const std::vector<int>& a) {
  long long best = LLONG_MIN;
  for (size_t i = 0; i < a.size(); ++i) {
    long long s = 0;
    for (size_t j = i; j < a.size(); ++j) best = std::max(best, s += a[j]);
  }
  return best;
}

static long long slow_count(const std::vector<int>& a, long long k) {
  long long c = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    long long s = 0;
    for (size_t j = i; j < a.size(); ++j)
      if ((s += a[j]) == k) ++c;
  }
  return c;
}

TEST(basic) {
  CHECK_EQ(longest_unique_substring("abcabcbb"), 3);
  CHECK_EQ(longest_unique_substring("bbbbb"), 1);
  CHECK_EQ(longest_unique_substring("pwwkew"), 3);
  CHECK_EQ(longest_unique_substring(""), 0);
  CHECK_EQ(min_window_len("ADOBECODEBANC", "ABC"), 4);
  CHECK_EQ(min_window_len("a", "a"), 1);
  CHECK_EQ(min_window_len("a", "aa"), 0);
  CHECK_EQ(min_window_len("abc", ""), 0);
  CHECK_EQ(min_window_len("aa", "aa"), 2);
  CHECK_EQ(max_subarray_sum({-2, 1, -3, 4, -1, 2, 1, -5, 4}), 6LL);
  CHECK_EQ(max_subarray_sum({-3, -1, -2}), -1LL);
  CHECK_EQ(max_subarray_sum({5}), 5LL);
  CHECK_EQ(count_subarrays_with_sum({1, 1, 1}, 2), 2LL);
  CHECK_EQ(count_subarrays_with_sum({1, 2, 3}, 3), 2LL);
  CHECK_EQ(count_subarrays_with_sum({0, 0, 0}, 0), 6LL);
  CHECK_EQ(count_subarrays_with_sum({1, -1, 1, -1}, 0), 4LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(0, 25);
    std::string s, t;
    for (int i = 0; i < n; ++i) s += (char)('a' + rnd(0, 3));
    int m = (int)rnd(0, 5);
    for (int i = 0; i < m; ++i) t += (char)('a' + rnd(0, 3));
    CHECK_EQ(longest_unique_substring(s), slow_unique(s));
    CHECK_EQ(min_window_len(s, t), slow_window(s, t));
    std::vector<int> a(std::max(n, 1));
    for (auto& x : a) x = (int)rnd(-5, 5);
    CHECK_EQ(max_subarray_sum(a), slow_kadane(a));
    long long k = rnd(-6, 6);
    CHECK_EQ(count_subarrays_with_sum(a, k), slow_count(a, k));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 2000000;
  std::string s(n, 'a');
  for (auto& c : s) c = (char)rnd(0, 255);
  CHECK(longest_unique_substring(s) <= 256);
  std::string t(200, 'x');
  for (auto& c : t) c = (char)rnd(0, 255);
  CHECK(min_window_len(s, t) >= 200);
  std::vector<int> a(n);
  for (auto& x : a) x = (int)rnd(-1000, 1000);
  CHECK(max_subarray_sum(a) >= 1000);
  CHECK(count_subarrays_with_sum(a, 0) >= 0);
  std::vector<int> z(n, 0);
  CHECK_EQ(count_subarrays_with_sum(z, 0), (long long)n * (n + 1) / 2);
}
