#include "harness.h"
#include SOLUTION

TEST(equal_substrings_equal_hashes) {
  using harness::rnd;
  for (int it = 0; it < 30; ++it) {
    int n = (int)rnd(1, 40);
    std::string s(n, 'a');
    for (auto& c : s) c = 'a' + rnd(0, 1);
    RollingHash h(s);
    for (int l1 = 0; l1 < n; ++l1)
      for (int r1 = l1; r1 <= n; ++r1)
        for (int l2 = 0; l2 < n; ++l2) {
          int r2 = l2 + (r1 - l1);
          if (r2 > n) continue;
          CHECK_EQ(h.get(l1, r1) == h.get(l2, r2),
                   s.compare(l1, r1 - l1, s, l2, r2 - l2) == 0);
        }
  }
}

TEST(two_objects_agree) {
  std::string a = "hello world, this is a test",
              b = "another string with a test inside";
  RollingHash ha(a), hb(b);
  CHECK_EQ(ha.get(23, 27), hb.get(22, 26));  // "test"
  CHECK_EQ(ha.get(0, 0), hb.get(5, 5));      // empty
  CHECK(ha.get(0, 5) != hb.get(0, 5));
}

TEST(birthday_no_collisions) {
  using harness::rnd;
  int n = 200000, L = 12;
  std::string s(n + L, 'a');
  for (auto& c : s) c = 'a' + rnd(0, 25);
  RollingHash h(s);
  std::unordered_map<unsigned long long, std::string> seen;
  seen.reserve(2 * n);
  int collisions = 0;
  for (int i = 0; i < n; ++i) {
    auto v = h.get(i, i + L);
    std::string sub = s.substr(i, L);
    auto it = seen.find(v);
    if (it == seen.end())
      seen.emplace(v, sub);
    else if (it->second != sub)
      ++collisions;
  }
  CHECK_EQ(collisions, 0);
}

TEST(thue_morse) {
  // t[i] = parity of popcount(i).  t and its complement collide under mod 2^64
  // for length >= 2048.
  int k = 13, n = 1 << k;
  std::string s;
  for (int i = 0; i < n; ++i) s += (__builtin_popcount(i) & 1) ? 'b' : 'a';
  for (int i = 0; i < n; ++i) s += (__builtin_popcount(i) & 1) ? 'a' : 'b';
  RollingHash h(s);
  CHECK(h.get(0, n) != h.get(n, 2 * n));
  for (int len = 1024; len <= n; len *= 2)
    CHECK(h.get(0, len) != h.get(n, n + len));
}

TEST(perf) {
  using harness::rnd;
  int n = 2000000;
  std::string s(n, 'a');
  for (auto& c : s) c = 'a' + rnd(0, 25);
  RollingHash h(s);
  unsigned long long x = 0;
  for (int i = 0; i < 2000000; ++i) {
    int l = (int)rnd(0, n - 1), r = (int)rnd(l, n);
    x ^= h.get(l, r);
  }
  CHECK(x != 0);
}
