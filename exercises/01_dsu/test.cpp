#include "harness.h"
#include SOLUTION

TEST(basic) {
  DSU d(5);
  CHECK_EQ(d.components(), 5);
  CHECK(d.unite(0, 1));
  CHECK(!d.unite(1, 0));
  CHECK(d.unite(2, 3));
  CHECK_EQ(d.components(), 3);
  CHECK_EQ(d.find(0), d.find(1));
  CHECK(d.find(0) != d.find(2));
  CHECK_EQ(d.size(0), 2);
  CHECK_EQ(d.size(4), 1);
  CHECK(d.unite(1, 3));
  CHECK_EQ(d.size(2), 4);
  CHECK_EQ(d.components(), 2);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int iter = 0; iter < 30; ++iter) {
    int n = (int)rnd(1, 60);
    DSU d(n);
    std::vector<int> label(n);
    std::iota(label.begin(), label.end(), 0);
    for (int op = 0; op < 300; ++op) {
      int a = (int)rnd(0, n - 1), b = (int)rnd(0, n - 1);
      bool same = label[a] == label[b];
      CHECK_EQ(d.find(a) == d.find(b), same);
      CHECK_EQ(d.unite(a, b), !same);
      if (!same) {
        int la = label[a], lb = label[b];
        for (auto& l : label)
          if (l == la) l = lb;
      }
      std::map<int, int> cnt;
      for (int l : label) cnt[l]++;
      CHECK_EQ(d.size(a), cnt[label[a]]);
      CHECK_EQ(d.components(), (int)cnt.size());
    }
  }
}

TEST(perf_chain) {
  int n = 1000000;
  DSU d(n);
  for (int i = 0; i + 1 < n; ++i) d.unite(i, i + 1);
  long long s = 0;
  for (int i = 0; i < 2000000; ++i) s += d.find((int)harness::rnd(0, n - 1));
  CHECK(s >= 0);
  CHECK_EQ(d.components(), 1);
  CHECK_EQ(d.size(0), n);
}
