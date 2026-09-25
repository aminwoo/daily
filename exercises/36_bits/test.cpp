#include "harness.h"
#include SOLUTION

static std::vector<int> shuffled(std::vector<int> v) {
  std::shuffle(v.begin(), v.end(), harness::rng);
  return v;
}

TEST(basic) {
  CHECK_EQ(single_number({2, 2, 1}), 1);
  CHECK_EQ(single_number({4, 1, 2, 1, 2}), 4);
  CHECK_EQ(single_number({-7}), -7);
  CHECK_EQ(single_number_thrice({2, 2, 3, 2}), 3);
  CHECK_EQ(single_number_thrice({0, 1, 0, 1, 0, 1, 99}), 99);
  CHECK_EQ(single_number_thrice({-2, -2, -2, -5}), -5);
  CHECK_EQ(two_singles({1, 2, 1, 3, 2, 5}), (std::pair<int, int>{3, 5}));
  CHECK_EQ(two_singles({-1, 0}), (std::pair<int, int>{-1, 0}));
  CHECK_EQ(two_singles({INT_MIN, 7, 7, INT_MAX}),
           (std::pair<int, int>{INT_MIN, INT_MAX}));
  CHECK_EQ(count_bits(5), (std::vector<int>{0, 1, 1, 2, 1, 2}));
  CHECK_EQ(count_bits(0), (std::vector<int>{0}));
  CHECK_EQ(reverse_bits(0), 0u);
  CHECK_EQ(reverse_bits(1), 0x80000000u);
  CHECK_EQ(reverse_bits(0x80000000u), 1u);
  CHECK_EQ(reverse_bits(0xFFFFFFFFu), 0xFFFFFFFFu);
  CHECK_EQ(reverse_bits(0b00000010100101000001111010011100u),
           0b00111001011110000010100101000000u);
  CHECK_EQ(max_xor_pair({3, 10, 5, 25, 2, 8}), 28);
  CHECK_EQ(max_xor_pair({0, 0}), 0);
  CHECK_EQ(max_xor_pair({1, 2}), 3);
  CHECK_EQ(max_xor_pair({(1 << 30) - 1, 0}), (1 << 30) - 1);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    std::set<int> used;
    auto fresh = [&] {
      int v;
      do v = (int)rnd(INT_MIN, INT_MAX);
      while (!used.insert(v).second);
      return v;
    };
    int k = (int)rnd(0, 10);
    std::vector<int> a, b, c;
    for (int i = 0; i < k; ++i) {
      int v = fresh();
      a.push_back(v), a.push_back(v);
      b.push_back(v), b.push_back(v), b.push_back(v);
      c.push_back(v), c.push_back(v);
    }
    int s = fresh();
    a.push_back(s);
    b.push_back(s);
    CHECK_EQ(single_number(shuffled(a)), s);
    CHECK_EQ(single_number_thrice(shuffled(b)), s);
    int s2 = fresh();
    c.push_back(s), c.push_back(s2);
    CHECK_EQ(two_singles(shuffled(c)),
             (std::pair<int, int>{std::min(s, s2), std::max(s, s2)}));
    uint32_t x = (uint32_t)rnd(0, UINT32_MAX);
    uint32_t r = 0;
    for (int i = 0; i < 32; ++i)
      if (x >> i & 1) r |= 1u << (31 - i);
    CHECK_EQ(reverse_bits(x), r);
    CHECK_EQ(reverse_bits(reverse_bits(x)), x);
    std::vector<int> v((size_t)rnd(2, 12));
    int bits = (int)rnd(1, 30);
    for (auto& y : v) y = (int)rnd(0, (1 << bits) - 1);
    int best = 0;
    for (size_t i = 0; i < v.size(); ++i)
      for (size_t j = i + 1; j < v.size(); ++j)
        best = std::max(best, v[i] ^ v[j]);
    CHECK_EQ(max_xor_pair(v), best);
  }
  auto cb = count_bits(1000);
  for (int i = 0; i <= 1000; ++i) CHECK_EQ(cb[i], __builtin_popcount(i));
}

TEST(perf) {
  using harness::rnd;
  int n = 300000;
  std::vector<int> v(n);
  for (auto& y : v) y = (int)rnd(0, (1 << 30) - 1);
  CHECK(max_xor_pair(v) > (1 << 29));  // O(n^2) is 4.5e10 pairs
  auto cb = count_bits(10000000);
  CHECK_EQ(cb[9999999], __builtin_popcount(9999999));
  std::vector<int> a;
  for (int i = 0; i < 1000000; ++i) a.push_back(i), a.push_back(i);
  a.push_back(-1);
  CHECK_EQ(single_number(a), -1);
  a.push_back(-2);
  CHECK_EQ(two_singles(a), (std::pair<int, int>{-2, -1}));
}
