#include "harness.h"
#include SOLUTION

TEST(basic) {
  Fenwick f(5);
  CHECK_EQ(f.prefix(0), 0LL);
  CHECK_EQ(f.prefix(5), 0LL);
  f.add(0, 3);
  f.add(4, 10);
  f.add(2, -1);
  CHECK_EQ(f.prefix(1), 3LL);
  CHECK_EQ(f.prefix(3), 2LL);
  CHECK_EQ(f.prefix(5), 12LL);
  CHECK_EQ(f.range(2, 5), 9LL);
  CHECK_EQ(f.range(3, 3), 0LL);
  f.add(2, 1);
  CHECK_EQ(f.range(0, 5), 13LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int iter = 0; iter < 30; ++iter) {
    int n = (int)rnd(1, 50);
    Fenwick f(n);
    std::vector<long long> a(n, 0);
    for (int op = 0; op < 400; ++op) {
      if (rnd(0, 1)) {
        int i = (int)rnd(0, n - 1);
        long long v = rnd(-1000, 1000);
        f.add(i, v);
        a[i] += v;
      } else {
        int l = (int)rnd(0, n), r = (int)rnd(l, n);
        long long s = 0;
        for (int i = l; i < r; ++i) s += a[i];
        CHECK_EQ(f.range(l, r), s);
        CHECK_EQ(f.prefix(r), std::accumulate(a.begin(), a.begin() + r, 0LL));
      }
    }
  }
}

TEST(perf) {
  int n = 1000000;
  Fenwick f(n);
  long long chk = 0;
  for (int i = 0; i < 1000000; ++i)
    f.add((int)harness::rnd(0, n - 1), harness::rnd(1, 1000));
  for (int i = 0; i < 1000000; ++i) chk += f.prefix((int)harness::rnd(0, n));
  CHECK(chk > 0);
}
