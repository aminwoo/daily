#include "harness.h"
#include SOLUTION

TEST(merge_basic) {
  CHECK_EQ(merge_sorted({1, 4, 9}, {2, 3, 10, 11}),
           (std::vector<int>{1, 2, 3, 4, 9, 10, 11}));
  CHECK_EQ(merge_sorted({}, {5}), (std::vector<int>{5}));
  CHECK_EQ(merge_sorted({}, {}), (std::vector<int>{}));
  CHECK_EQ(merge_sorted({1, 1}, {1}), (std::vector<int>{1, 1, 1}));
}

TEST(inversions_basic) {
  CHECK_EQ(count_inversions({}), 0LL);
  CHECK_EQ(count_inversions({1}), 0LL);
  CHECK_EQ(count_inversions({1, 2, 3}), 0LL);
  CHECK_EQ(count_inversions({3, 2, 1}), 3LL);
  CHECK_EQ(count_inversions({2, 4, 1, 3, 5}), 3LL);
  CHECK_EQ(count_inversions({5, 5, 5}), 0LL);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    int n = (int)rnd(0, 40);
    std::vector<int> a(n);
    for (auto& x : a) x = (int)rnd(-20, 20);
    long long inv = 0;
    for (int i = 0; i < n; ++i)
      for (int j = i + 1; j < n; ++j)
        if (a[i] > a[j]) ++inv;
    CHECK_EQ(count_inversions(a), inv);
    int m = (int)rnd(0, 20);
    std::vector<int> b(m);
    for (auto& x : b) x = (int)rnd(-20, 20);
    std::vector<int> sa = a, sb = b, want;
    std::sort(sa.begin(), sa.end());
    std::sort(sb.begin(), sb.end());
    std::merge(sa.begin(), sa.end(), sb.begin(), sb.end(),
               std::back_inserter(want));
    CHECK_EQ(merge_sorted(sa, sb), want);
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 2000000;
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) a[i] = n - i;
  CHECK_EQ(count_inversions(a), (long long)n * (n - 1) / 2);
  for (auto& x : a) x = (int)rnd(0, 1000000000);
  long long inv = count_inversions(a);
  CHECK(inv > (long long)n * (n - 1) / 5 &&
        inv < (long long)n * (n - 1) / 3);  // ~n²/4 expected
}
