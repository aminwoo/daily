#include "harness.h"
#include SOLUTION

TEST(basic) {
  SegTree t({5, 3, 8, 6, 1, 9});
  CHECK_EQ(t.query(0, 6), 1LL);
  CHECK_EQ(t.query(0, 4), 3LL);
  CHECK_EQ(t.query(2, 4), 6LL);
  CHECK_EQ(t.query(5, 6), 9LL);
  t.set(4, 100);
  CHECK_EQ(t.query(0, 6), 3LL);
  t.set(0, -7);
  CHECK_EQ(t.query(0, 1), -7LL);
  CHECK_EQ(t.query(1, 6), 3LL);
}

TEST(single_element) {
  SegTree t({42});
  CHECK_EQ(t.query(0, 1), 42LL);
  t.set(0, -1);
  CHECK_EQ(t.query(0, 1), -1LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int iter = 0; iter < 40; ++iter) {
    int n = (int)rnd(1, 60);
    std::vector<long long> a(n);
    for (auto& x : a) x = rnd(-1000, 1000);
    SegTree t(a);
    for (int op = 0; op < 300; ++op) {
      if (rnd(0, 1)) {
        int i = (int)rnd(0, n - 1);
        long long v = rnd(-1000, 1000);
        t.set(i, v);
        a[i] = v;
      } else {
        int l = (int)rnd(0, n - 1), r = (int)rnd(l + 1, n);
        CHECK_EQ(t.query(l, r),
                 *std::min_element(a.begin() + l, a.begin() + r));
      }
    }
  }
}

TEST(perf) {
  int n = 500000;
  std::vector<long long> a(n);
  for (auto& x : a) x = harness::rnd(0, 1000000000);
  SegTree t(a);
  long long chk = 0;
  for (int i = 0; i < 500000; ++i) {
    int l = (int)harness::rnd(0, n - 1), r = (int)harness::rnd(l + 1, n);
    chk += t.query(l, r);
    t.set((int)harness::rnd(0, n - 1), harness::rnd(0, 1000000000));
  }
  CHECK(chk >= 0);
}
