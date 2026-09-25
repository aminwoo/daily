#include "harness.h"
#include SOLUTION

static std::vector<int> slow_pi(const std::string& s) {
  int n = s.size();
  std::vector<int> pi(n, 0);
  for (int i = 1; i < n; ++i)
    for (int k = i; k >= 1; --k)
      if (s.compare(0, k, s, i - k + 1, k) == 0) {
        pi[i] = k;
        break;
      }
  return pi;
}

static std::vector<int> slow_find(const std::string& t, const std::string& p) {
  std::vector<int> r;
  for (size_t i = 0; i + p.size() <= t.size(); ++i)
    if (t.compare(i, p.size(), p) == 0) r.push_back(i);
  return r;
}

TEST(prefix_function_basic) {
  CHECK_EQ(prefix_function("abcabcd"), (std::vector<int>{0, 0, 0, 1, 2, 3, 0}));
  CHECK_EQ(prefix_function("aabaaab"), (std::vector<int>{0, 1, 0, 1, 2, 2, 3}));
  CHECK_EQ(prefix_function("a"), (std::vector<int>{0}));
  CHECK_EQ(prefix_function(""), (std::vector<int>{}));
}

TEST(find_basic) {
  CHECK_EQ(find_occurrences("abababa", "aba"), (std::vector<int>{0, 2, 4}));
  CHECK_EQ(find_occurrences("aaaa", "aa"), (std::vector<int>{0, 1, 2}));
  CHECK_EQ(find_occurrences("abc", "abcd"), (std::vector<int>{}));
  CHECK_EQ(find_occurrences("", "a"), (std::vector<int>{}));
  CHECK_EQ(find_occurrences("xyz", "z"), (std::vector<int>{2}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(0, 40), m = (int)rnd(1, 5), alpha = (int)rnd(1, 3);
    std::string t(n, 'a'), p(m, 'a');
    for (auto& c : t) c = 'a' + rnd(0, alpha - 1);
    for (auto& c : p) c = 'a' + rnd(0, alpha - 1);
    CHECK_EQ(prefix_function(t), slow_pi(t));
    CHECK_EQ(find_occurrences(t, p), slow_find(t, p));
  }
}

TEST(perf_worst_case) {
  std::string t(2000000, 'a'), p(1000000, 'a');
  CHECK_EQ(find_occurrences(t, p).size(), (size_t)1000001);
  auto pi = prefix_function(t);
  CHECK_EQ(pi.back(), 1999999);
  t[1500000] = 'b';
  CHECK_EQ(find_occurrences(t, p).size(), (size_t)500001);
}
