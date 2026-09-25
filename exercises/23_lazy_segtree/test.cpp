#include "harness.h"
#include SOLUTION

TEST(basic) {
  LazySegTree t({1, 2, 3, 4, 5});
  CHECK_EQ(t.sum(0, 5), 15LL);
  t.add(1, 4, 10);
  CHECK_EQ(t.sum(0, 5), 45LL);
  CHECK_EQ(t.sum(1, 2), 12LL);
  CHECK_EQ(t.sum(3, 5), 19LL);
  t.add(0, 5, -1);
  CHECK_EQ(t.sum(0, 1), 0LL);
  CHECK_EQ(t.sum(2, 2), 0LL);
  CHECK_EQ(t.sum(0, 5), 40LL);
  LazySegTree one({7});
  one.add(0, 1, 3);
  CHECK_EQ(one.sum(0, 1), 10LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 40; ++it) {
    int n = (int)rnd(1, 60);
    std::vector<long long> a(n);
    for (auto& x : a) x = rnd(-100, 100);
    LazySegTree t(a);
    for (int op = 0; op < 400; ++op) {
      int l = (int)rnd(0, n), r = (int)rnd(l, n);
      if (rnd(0, 1)) {
        long long v = rnd(-50, 50);
        t.add(l, r, v);
        for (int i = l; i < r; ++i) a[i] += v;
      } else
        CHECK_EQ(t.sum(l, r),
                 std::accumulate(a.begin() + l, a.begin() + r, 0LL));
    }
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 200000;
  std::vector<long long> a(n);
  for (auto& x : a) x = rnd(0, 1000);
  LazySegTree t(a);
  long long chk = 0;
  for (int op = 0; op < 400000; ++op) {
    int l = (int)rnd(0, n / 4), r = (int)rnd(3 * n / 4, n);
    if (op & 1)
      t.add(l, r, rnd(-10, 10));
    else
      chk += t.sum(l, r);
  }
  CHECK(chk != 0);
}
