#include "harness.h"
#include SOLUTION

using VV = std::vector<std::vector<int>>;

static VV sorted(VV v) {
  std::sort(v.begin(), v.end());
  return v;
}

static VV slow_subsets(const std::vector<int>& a) {
  VV out;
  for (int m = 0; m < (1 << a.size()); ++m) {
    std::vector<int> s;
    for (size_t i = 0; i < a.size(); ++i)
      if (m >> i & 1) s.push_back(a[i]);
    out.push_back(s);
  }
  return sorted(out);
}

static VV slow_perms(std::vector<int> a) {
  std::sort(a.begin(), a.end());
  VV out;
  do out.push_back(a);
  while (std::next_permutation(a.begin(), a.end()));
  return out;  // already sorted & unique
}

// enumerate multiplicity vectors
static VV slow_combos(const std::vector<int>& cand, int target) {
  VV out;
  std::vector<int> cnt(cand.size());
  auto rec = [&](auto& self, size_t i, int rem) -> void {
    if (i == cand.size()) {
      if (rem == 0) {
        std::vector<int> v;
        for (size_t j = 0; j < cand.size(); ++j)
          for (int k = 0; k < cnt[j]; ++k) v.push_back(cand[j]);
        std::sort(v.begin(), v.end());
        out.push_back(v);
      }
      return;
    }
    for (int k = 0; k * cand[i] <= rem; ++k) {
      cnt[i] = k;
      self(self, i + 1, rem - k * cand[i]);
    }
    cnt[i] = 0;
  };
  rec(rec, 0, target);
  return sorted(out);
}

static bool balanced(const std::string& s) {
  int d = 0;
  for (char c : s) {
    d += c == '(' ? 1 : -1;
    if (d < 0) return false;
  }
  return d == 0;
}

TEST(basic) {
  CHECK_EQ(sorted(subsets({1, 2, 3})),
           (VV{{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}}));
  CHECK_EQ(sorted(subsets({})), (VV{{}}));
  CHECK_EQ(sorted(permutations_unique({1, 1, 2})),
           (VV{{1, 1, 2}, {1, 2, 1}, {2, 1, 1}}));
  CHECK_EQ(sorted(permutations_unique({3, 3, 3})), (VV{{3, 3, 3}}));
  CHECK_EQ(sorted(permutations_unique({})), (VV{{}}));
  CHECK_EQ(sorted(combination_sum({2, 3, 6, 7}, 7)), (VV{{2, 2, 3}, {7}}));
  CHECK_EQ(sorted(combination_sum({2, 3, 5}, 8)),
           (VV{{2, 2, 2, 2}, {2, 3, 3}, {3, 5}}));
  CHECK_EQ(sorted(combination_sum({2}, 1)), (VV{}));
  CHECK_EQ(sorted(combination_sum({2}, 0)), (VV{{}}));
  int q[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724};
  CHECK_EQ(n_queens(0), 1);
  for (int n = 1; n <= 10; ++n) CHECK_EQ(n_queens(n), q[n]);
  auto p3 = generate_parens(3);
  std::sort(p3.begin(), p3.end());
  CHECK_EQ(p3, (std::vector<std::string>{"((()))", "(()())", "(())()", "()(())",
                                         "()()()"}));
  CHECK_EQ(generate_parens(0), (std::vector<std::string>{""}));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    std::vector<int> a((size_t)rnd(0, 7));
    std::set<int> seen;
    for (auto& x : a) {
      do x = (int)rnd(-10, 10);
      while (!seen.insert(x).second);
    }
    CHECK_EQ(sorted(subsets(a)), slow_subsets(a));
    std::vector<int> p((size_t)rnd(0, 6));
    for (auto& x : p) x = (int)rnd(0, 2);
    CHECK_EQ(sorted(permutations_unique(p)), slow_perms(p));
    std::vector<int> c((size_t)rnd(1, 4));
    std::set<int> cs;
    for (auto& x : c) {
      do x = (int)rnd(1, 9);
      while (!cs.insert(x).second);
    }
    int t = (int)rnd(0, 20);
    CHECK_EQ(sorted(combination_sum(c, t)), slow_combos(c, t));
  }
  for (int n = 0; n <= 7; ++n) {
    auto g = generate_parens(n);
    std::set<std::string> u(g.begin(), g.end());
    CHECK_EQ(u.size(), g.size());
    for (auto& s : g) CHECK(s.size() == (size_t)2 * n && balanced(s));
    long long cat[] = {1, 1, 2, 5, 14, 42, 132, 429};
    CHECK_EQ((long long)g.size(), cat[n]);
  }
}

TEST(perf) {
  CHECK_EQ(n_queens(12), 14200);
  CHECK_EQ(n_queens(13), 73712);
  std::vector<int> a(18);
  std::iota(a.begin(), a.end(), 0);
  CHECK_EQ(subsets(a).size(), (size_t)1 << 18);
  std::vector<int> p(9);
  std::iota(p.begin(), p.end(), 0);
  CHECK_EQ(permutations_unique(p).size(), (size_t)362880);
  std::vector<int> d(
      12, 1);  // all equal: exactly one permutation, must not blow up
  CHECK_EQ(permutations_unique(d).size(), (size_t)1);
  CHECK_EQ(generate_parens(12).size(), (size_t)208012);
  CHECK_EQ(combination_sum({1, 2, 3, 4, 5}, 40).size(), (size_t)1747);
}
