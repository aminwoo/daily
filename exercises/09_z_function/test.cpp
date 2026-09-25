#include "harness.h"
#include SOLUTION

static std::vector<int> slow_z(const std::string& s) {
  int n = s.size();
  std::vector<int> z(n, 0);
  for (int i = 0; i < n; ++i)
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
  return z;
}

TEST(basic) {
  CHECK_EQ(z_function("aaaaa"), (std::vector<int>{5, 4, 3, 2, 1}));
  CHECK_EQ(z_function("aaabaab"), (std::vector<int>{7, 2, 1, 0, 2, 1, 0}));
  CHECK_EQ(z_function("abacaba"), (std::vector<int>{7, 0, 1, 0, 3, 0, 1}));
  CHECK_EQ(z_function("a"), (std::vector<int>{1}));
  CHECK_EQ(z_function(""), (std::vector<int>{}));
  CHECK_EQ(count_occurrences("abababa", "aba"), 3LL);
  CHECK_EQ(count_occurrences("aaaa", "a"), 4LL);
  CHECK_EQ(count_occurrences("abc", "abcd"), 0LL);
  CHECK_EQ(count_occurrences("", "x"), 0LL);
  CHECK_EQ(count_occurrences("##", "#"), 2LL);
  const std::string byte_pat{"\x01\0", 2};
  const std::string byte_text{"\x01\0\x01\0\xff", 5};
  CHECK_EQ(count_occurrences(byte_text, byte_pat), 2LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(0, 50), alpha = (int)rnd(1, 3);
    std::string s(n, 'a');
    for (auto& c : s) c = 'a' + rnd(0, alpha - 1);
    CHECK_EQ(z_function(s), slow_z(s));
    int m = (int)rnd(1, 4);
    std::string p(m, 'a');
    for (auto& c : p) c = 'a' + rnd(0, alpha - 1);
    long long cnt = 0;
    for (int i = 0; i + m <= n; ++i)
      if (s.compare(i, m, p) == 0) ++cnt;
    CHECK_EQ(count_occurrences(s, p), cnt);
  }
}

TEST(perf_worst_case) {
  std::string s(3000000, 'a');
  auto z = z_function(s);
  CHECK_EQ(z[1], 2999999);
  CHECK_EQ(count_occurrences(s, std::string(1000000, 'a')), 2000001LL);
}
