#include "harness.h"
#include SOLUTION

static bool is_prime_slow(long long x) {
  if (x < 2) return false;
  for (long long d = 2; d * d <= x; ++d)
    if (x % d == 0) return false;
  return true;
}

TEST(basic) {
  CHECK_EQ(primes_up_to(0), (std::vector<int>{}));
  CHECK_EQ(primes_up_to(1), (std::vector<int>{}));
  CHECK_EQ(primes_up_to(2), (std::vector<int>{2}));
  CHECK_EQ(primes_up_to(30),
           (std::vector<int>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}));
  CHECK_EQ(spf_up_to(12),
           (std::vector<int>{0, 0, 2, 3, 2, 5, 2, 7, 2, 3, 2, 11, 2}));
  CHECK_EQ(spf_up_to(0), (std::vector<int>{0}));
  CHECK_EQ(spf_up_to(1), (std::vector<int>{0, 0}));
  auto spf = spf_up_to(1000);
  CHECK_EQ(factorize(1, spf), (std::vector<std::pair<int, int>>{}));
  CHECK_EQ(factorize(360, spf),
           (std::vector<std::pair<int, int>>{{2, 3}, {3, 2}, {5, 1}}));
  CHECK_EQ(factorize(997, spf), (std::vector<std::pair<int, int>>{{997, 1}}));
}

TEST(vs_trial_division) {
  int N = 3000;
  std::vector<int> want;
  for (int i = 0; i <= N; ++i)
    if (is_prime_slow(i)) want.push_back(i);
  CHECK_EQ(primes_up_to(N), want);
  auto spf = spf_up_to(N);
  REQUIRE_EQ(spf.size(), (size_t)N + 1);
  for (int i = 2; i <= N; ++i) {
    int s = 2;
    while (i % s) ++s;
    CHECK_EQ(spf[i], s);
    auto f = factorize(i, spf);
    long long prod = 1;
    for (size_t k = 0; k < f.size(); ++k) {
      CHECK(is_prime_slow(f[k].first));
      CHECK(k == 0 || f[k - 1].first < f[k].first);
      for (int e = 0; e < f[k].second; ++e) prod *= f[k].first;
    }
    CHECK_EQ(prod, (long long)i);
  }
}

TEST(perf) {
  int N = 20000000;
  auto p = primes_up_to(N);
  CHECK_EQ(p.size(), (size_t)1270607);
  CHECK_EQ(p.back(), 19999999);
  auto spf = spf_up_to(N);
  CHECK_EQ(spf[N], 2);
  CHECK_EQ(spf[19999999], 19999999);
  CHECK_EQ(spf[19999997], 59);
  long long chk = 0;
  for (int i = 0; i < 200000; ++i)
    for (auto [pr, e] : factorize((int)harness::rnd(1, N), spf)) chk += pr * e;
  CHECK(chk > 0);
}
