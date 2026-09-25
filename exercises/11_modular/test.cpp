#include "harness.h"
#include SOLUTION

TEST(mod_pow_basic) {
  CHECK_EQ(mod_pow(2, 10, 1000), 24LL);
  CHECK_EQ(mod_pow(0, 0, 7), 1LL);
  CHECK_EQ(mod_pow(5, 0, 1), 0LL);
  CHECK_EQ(mod_pow(3, 1000000000000000000LL, 1000000007), 246336683LL);
  for (int it = 0; it < 500; ++it) {
    long long m = harness::rnd(1, 50), b = harness::rnd(0, m - 1),
              e = harness::rnd(0, 40);
    long long slow = 1 % m;
    for (int i = 0; i < e; ++i) slow = slow * b % m;
    CHECK_EQ(mod_pow(b, e, m), slow);
  }
  long long big = 1999999973;  // near 2e9, prime
  CHECK_EQ(mod_pow(big - 1, 2, big), 1LL);
  CHECK_EQ(mod_pow(big - 2, big - 1, big), 1LL);  // Fermat
}

TEST(ext_gcd_identity) {
  for (int it = 0; it < 2000; ++it) {
    long long a = harness::rnd(0, 1000000000000LL),
              b = harness::rnd(0, 1000000000000LL);
    if (!a && !b) a = 1;
    long long x, y, g = ext_gcd(a, b, x, y);
    CHECK_EQ(g, std::gcd(a, b));
    CHECK_EQ((long long)((__int128)a * x + (__int128)b * y), g);
    CHECK(std::llabs(x) <= std::max(b, 1LL) &&
          std::llabs(y) <= std::max(a, 1LL));  // standard ext-gcd bound
  }
}

TEST(mod_inv_any_modulus) {
  CHECK_EQ(mod_inv(3, 7), 5LL);
  CHECK_EQ(mod_inv(2, 4), -1LL);
  CHECK_EQ(mod_inv(1, 2), 1LL);
  for (int it = 0; it < 3000; ++it) {
    long long m = harness::rnd(2, 1000000), a = harness::rnd(1, m - 1),
              inv = mod_inv(a, m);
    if (std::gcd(a, m) != 1)
      CHECK_EQ(inv, -1LL);
    else {
      CHECK(inv >= 0 && inv < m);
      CHECK_EQ(a * inv % m, 1LL);
    }
  }
  CHECK_EQ(mod_inv(123456789, 1000000007) * 123456789 % 1000000007, 1LL);
}

TEST(binomial_vs_pascal) {
  const long long P = 1000000007;
  int N = 300;
  std::vector<std::vector<long long>> pas(N + 1,
                                          std::vector<long long>(N + 1, 0));
  for (int n = 0; n <= N; ++n) {
    pas[n][0] = 1;
    for (int k = 1; k <= n; ++k)
      pas[n][k] = (pas[n - 1][k - 1] + pas[n - 1][k]) % P;
  }
  Binomial B(N, P);
  for (int n = 0; n <= N; ++n)
    for (int k = 0; k <= n; ++k) CHECK_EQ(B.C(n, k), pas[n][k]);
  CHECK_EQ(B.C(5, 6), 0LL);
  CHECK_EQ(B.C(5, -1), 0LL);
  Binomial small(10, 13);
  CHECK_EQ(small.C(10, 5), 252 % 13);
  Binomial big(1000000, P);
  CHECK_EQ(big.C(1000000, 500000), 996692777LL);
}
