#include "harness.h"
#include SOLUTION

static long long slow_knap(const std::vector<int>& w,
                           const std::vector<long long>& v, int W) {
  int n = w.size();
  long long best = 0;
  for (int m = 0; m < (1 << n); ++m) {
    long long tw = 0, tv = 0;
    for (int i = 0; i < n; ++i)
      if (m >> i & 1) {
        tw += w[i];
        tv += v[i];
      }
    if (tw <= W) best = std::max(best, tv);
  }
  return best;
}

static long long slow_ways(const std::vector<int>& c, int t, size_t i = 0) {
  if (t == 0) return 1;
  if (i == c.size() || t < 0) return 0;
  return slow_ways(c, t - c[i], i) + slow_ways(c, t, i + 1);
}

static int slow_min(const std::vector<int>& c, int t) {  // BFS over amounts
  std::vector<int> d(t + 1, -1);
  d[0] = 0;
  std::queue<int> q;
  q.push(0);
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    for (int ci : c)
      if (x + ci <= t && d[x + ci] == -1) {
        d[x + ci] = d[x] + 1;
        q.push(x + ci);
      }
  }
  return d[t];
}

static int slow_lcs(const std::string& a,
                    const std::string& b) {  // enumerate subsequences of a
  int n = a.size(), best = 0;
  for (int m = 0; m < (1 << n); ++m) {
    std::string s;
    for (int i = 0; i < n; ++i)
      if (m >> i & 1) s += a[i];
    size_t k = 0;
    for (char ch : b)
      if (k < s.size() && ch == s[k]) ++k;
    if (k == s.size()) best = std::max(best, (int)s.size());
  }
  return best;
}

TEST(basic) {
  CHECK_EQ(knapsack01({1, 3, 4, 5}, {1, 4, 5, 7}, 7), 9LL);
  CHECK_EQ(knapsack01({}, {}, 10), 0LL);
  CHECK_EQ(knapsack01({5}, {10}, 4), 0LL);
  CHECK_EQ(count_ways({1, 2, 5}, 5), 4LL);
  CHECK_EQ(count_ways({2}, 3), 0LL);
  CHECK_EQ(count_ways({2}, 0), 1LL);
  CHECK_EQ(min_coins({1, 2, 5}, 11), 3);
  CHECK_EQ(min_coins({2}, 3), -1);
  CHECK_EQ(min_coins({3, 7}, 0), 0);
  CHECK_EQ(lcs("abcde", "ace"), 3);
  CHECK_EQ(lcs("abc", "def"), 0);
  CHECK_EQ(lcs("", "abc"), 0);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    int n = (int)rnd(0, 10), W = (int)rnd(0, 30);
    std::vector<int> w(n);
    std::vector<long long> v(n);
    for (int i = 0; i < n; ++i) {
      w[i] = (int)rnd(1, 12);
      v[i] = rnd(0, 100);
    }
    CHECK_EQ(knapsack01(w, v, W), slow_knap(w, v, W));

    int k = (int)rnd(1, 4), t = (int)rnd(0, 25);
    std::vector<int> c(k);
    for (auto& x : c) x = (int)rnd(1, 9);
    std::sort(c.begin(), c.end());
    c.erase(std::unique(c.begin(), c.end()), c.end());
    CHECK_EQ(count_ways(c, t), slow_ways(c, t));
    CHECK_EQ(min_coins(c, t), slow_min(c, t));

    std::string a((int)rnd(0, 10), 'a'), b((int)rnd(0, 12), 'a');
    for (auto& ch : a) ch = 'a' + rnd(0, 2);
    for (auto& ch : b) ch = 'a' + rnd(0, 2);
    CHECK_EQ(lcs(a, b), slow_lcs(a, b));
  }
}

TEST(perf) {
  using harness::rnd;
  int n = 2000, W = 100000;
  std::vector<int> w(n);
  std::vector<long long> v(n);
  for (int i = 0; i < n; ++i) {
    w[i] = (int)rnd(1, 1000);
    v[i] = rnd(1, 1000000);
  }
  CHECK(knapsack01(w, v, W) > 0);
  CHECK_EQ(count_ways({1, 5, 10, 25, 50}, 100000), 66793412685001LL);
  CHECK_EQ(min_coins({7, 11}, 7000033), 636367);
  std::string a(5000, 'a'), b(5000, 'a');
  for (auto& ch : a) ch = 'a' + rnd(0, 3);
  for (auto& ch : b) ch = 'a' + rnd(0, 3);
  int L = lcs(a, b);
  CHECK(L > 3000 && L < 4000);
}
