#include "harness.h"
#include SOLUTION

TEST(basic) {
  SparseTable st({4, 2, 7, 1, 9, 3});
  CHECK_EQ(st.query(0, 6), 1);
  CHECK_EQ(st.query(0, 3), 2);
  CHECK_EQ(st.query(2, 3), 7);
  CHECK_EQ(st.query(4, 6), 3);
  CHECK_EQ(st.query(1, 5), 1);
  SparseTable one({-5});
  CHECK_EQ(one.query(0, 1), -5);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 50; ++it) {
    int n = (int)rnd(1, 70);
    std::vector<int> a(n);
    for (auto& x : a) x = (int)rnd(-100, 100);
    SparseTable st(a);
    for (int l = 0; l < n; ++l)
      for (int r = l + 1; r <= n; ++r)
        CHECK_EQ(st.query(l, r),
                 *std::min_element(a.begin() + l, a.begin() + r));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 1000000;
  std::vector<int> a(n);
  for (auto& x : a) x = (int)rnd(0, 1000000000);
  SparseTable st(a);
  long long chk = 0;
  for (int i = 0; i < 3000000; ++i) {
    int l = (int)rnd(0, n - 1), r = (int)rnd(l + 1, n);
    chk += st.query(l, r);
  }
  CHECK(chk > 0);
}
