#include "harness.h"
#include SOLUTION

TEST(first_true_basic) {
  CHECK_EQ(first_true(0, 10, [](long long x) { return x >= 7; }), 7LL);
  CHECK_EQ(first_true(0, 10, [](long long x) { return x >= 0; }), 0LL);
  CHECK_EQ(first_true(0, 10, [](long long) { return false; }), 10LL);
  CHECK_EQ(first_true(5, 5, [](long long) { return true; }), 5LL);
  CHECK_EQ(first_true(-100, 100, [](long long x) { return x > -3; }), -2LL);
}

TEST(first_true_extremes_and_call_count) {
  long long lo = 5000000000000000000LL, hi = 9000000000000000000LL,
            target = 7777777777777777777LL;
  int calls = 0;
  auto pred = [&](long long x) {
    ++calls;
    CHECK(x >= lo && x < hi);
    return x >= target;
  };
  CHECK_EQ(first_true(lo, hi, pred), target);
  CHECK(calls <= 70);
  calls = 0;
  CHECK_EQ(first_true(LLONG_MIN, LLONG_MAX,
                      [&](long long x) {
                        ++calls;
                        return x >= -12345;
                      }),
           -12345LL);
  CHECK(calls <= 70);
  CHECK_EQ(first_true(LLONG_MIN, LLONG_MAX, [](long long) { return false; }),
           LLONG_MAX);
}

TEST(first_true_random) {
  using harness::rnd;
  for (int i = 0; i < 2000; ++i) {
    long long lo = rnd(-1000, 1000), hi = rnd(lo, lo + 2000), t = rnd(lo, hi);
    CHECK_EQ(first_true(lo, hi, [t](long long x) { return x >= t; }), t);
  }
}

TEST(lower_bound_index_random) {
  using harness::rnd;
  for (int it = 0; it < 500; ++it) {
    int n = (int)rnd(0, 30);
    std::vector<int> a(n);
    for (auto& v : a) v = (int)rnd(-10, 10);
    std::sort(a.begin(), a.end());
    for (int x = -12; x <= 12; ++x)
      CHECK_EQ(lower_bound_index(a, x),
               (int)(std::lower_bound(a.begin(), a.end(), x) - a.begin()));
  }
}

TEST(isqrt_exact) {
  for (long long n = 0; n < 5000; ++n) {
    long long r = isqrt(n);
    CHECK(r * r <= n && (r + 1) * (r + 1) > n);
  }
  long long big[] = {1000000000000000000LL,
                     999999999999999999LL,
                     999999998000000001LL,
                     999999998000000000LL,
                     1LL << 59,
                     (1LL << 59) - 1};
  for (long long n : big) {
    long long r = isqrt(n);
    CHECK(r >= 0 && r <= 1000000000LL);
    CHECK(r * r <= n);
    CHECK((r + 1) * (r + 1) > n);
  }
  for (int i = 0; i < 20000; ++i) {
    long long n = harness::rnd(0, 1000000000000000000LL), r = isqrt(n);
    CHECK(r >= 0 && r <= 1000000000LL && r * r <= n &&
          (r + 1) * (r + 1) > n);
  }
}
