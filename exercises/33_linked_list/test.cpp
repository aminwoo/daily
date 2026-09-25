#include "harness.h"
#include SOLUTION

static ListNode* build(const std::vector<int>& v) {
  ListNode dummy(0);
  ListNode* t = &dummy;
  for (int x : v) t = t->next = new ListNode(x);
  return dummy.next;
}

static std::vector<int> to_vec(ListNode* h, size_t limit = 5000000) {
  std::vector<int> v;
  for (; h && v.size() < limit; h = h->next) v.push_back(h->val);
  return v;
}

static std::set<ListNode*> nodes(ListNode* h) {
  std::set<ListNode*> s;
  for (; h; h = h->next) s.insert(h);
  return s;
}

static std::vector<int> rand_vec(int n, int lo, int hi) {
  std::vector<int> v(n);
  for (auto& x : v) x = (int)harness::rnd(lo, hi);
  return v;
}

TEST(basic) {
  CHECK_EQ(to_vec(reverse_list(build({1, 2, 3}))), (std::vector<int>{3, 2, 1}));
  CHECK_EQ(to_vec(reverse_list(nullptr)), (std::vector<int>{}));
  CHECK_EQ(to_vec(merge_sorted(build({1, 3, 5}), build({2, 4}))),
           (std::vector<int>{1, 2, 3, 4, 5}));
  CHECK_EQ(to_vec(merge_sorted(nullptr, build({2}))), (std::vector<int>{2}));
  CHECK_EQ(middle(build({1, 2, 3, 4}))->val, 3);
  CHECK_EQ(middle(build({1, 2, 3}))->val, 2);
  CHECK_EQ(middle(build({7}))->val, 7);
  CHECK_EQ(to_vec(remove_nth_from_end(build({1, 2, 3, 4, 5}), 2)),
           (std::vector<int>{1, 2, 3, 5}));
  CHECK_EQ(to_vec(remove_nth_from_end(build({1}), 1)), (std::vector<int>{}));
  CHECK_EQ(to_vec(remove_nth_from_end(build({1, 2}), 2)),
           (std::vector<int>{2}));
  CHECK(cycle_start(build({1, 2, 3})) == nullptr);
  CHECK(cycle_start(nullptr) == nullptr);
  CHECK(is_palindrome(build({1, 2, 2, 1})));
  CHECK(is_palindrome(build({1, 2, 1})));
  CHECK(is_palindrome(build({1})));
  CHECK(!is_palindrome(build({1, 2})));
  CHECK(is_palindrome(nullptr));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 300; ++it) {
    auto v = rand_vec((int)rnd(0, 12), 0, 3);
    ListNode* h = build(v);
    auto before = nodes(h);
    ListNode* r = reverse_list(h);
    auto rv = v;
    std::reverse(rv.begin(), rv.end());
    CHECK_EQ(to_vec(r), rv);
    CHECK(nodes(r) == before);

    auto a = rand_vec((int)rnd(0, 8), 0, 5), b = rand_vec((int)rnd(0, 8), 0, 5);
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    ListNode *la = build(a), *lb = build(b);
    auto all = nodes(la);
    all.merge(nodes(lb));
    // stability: tag by source, expect a's node before b's on ties
    std::vector<std::pair<ListNode*, int>> exp;
    for (ListNode* p = la; p; p = p->next) exp.push_back({p, 0});
    for (ListNode* p = lb; p; p = p->next) exp.push_back({p, 1});
    std::stable_sort(exp.begin(), exp.end(), [](auto& x, auto& y) {
      return x.first->val < y.first->val;
    });
    ListNode* m = merge_sorted(la, lb);
    size_t i = 0;
    for (ListNode* p = m; p; p = p->next, ++i)
      CHECK(i < exp.size() && p == exp[i].first);
    CHECK_EQ(i, exp.size());

    if (!v.empty()) {
      h = build(v);
      CHECK_EQ(middle(h)->val, v[v.size() / 2]);
      int n = (int)rnd(1, v.size());
      before = nodes(h);
      ListNode* victim = h;
      for (int k = 0; k < (int)v.size() - n; ++k) victim = victim->next;
      before.erase(victim);
      ListNode* rem = remove_nth_from_end(h, n);
      auto ev = v;
      ev.erase(ev.begin() + (v.size() - n));
      CHECK_EQ(to_vec(rem), ev);
      CHECK(nodes(rem) == before);

      // cycle: tail -> node k
      h = build(v);
      std::vector<ListNode*> ptr;
      for (ListNode* p = h; p; p = p->next) ptr.push_back(p);
      int k = (int)rnd(0, v.size() - 1);
      ptr.back()->next = ptr[k];
      CHECK(cycle_start(h) == ptr[k]);
      for (size_t j = 0; j < ptr.size(); ++j)  // untouched
        CHECK(ptr[j]->next == (j + 1 < ptr.size() ? ptr[j + 1] : ptr[k]));
      ptr.back()->next = nullptr;
      CHECK(cycle_start(h) == nullptr);
    }

    auto pv = rand_vec((int)rnd(0, 10), 0, 1);
    if (rnd(0, 1)) {
      auto q = pv;
      std::reverse(q.begin(), q.end());
      if (rnd(0, 1) && !pv.empty()) q.erase(q.begin());
      pv.insert(pv.end(), q.begin(), q.end());
    }
    bool pal = std::equal(pv.begin(), pv.begin() + pv.size() / 2, pv.rbegin());
    CHECK_EQ(is_palindrome(build(pv)), pal);
  }
}

TEST(perf) {
  int n = 3000000;
  std::vector<int> v(n);
  for (int i = 0; i < n; ++i) v[i] = std::min(i, n - 1 - i);
  ListNode* h = build(v);
  CHECK(is_palindrome(h));
  h = build(v);
  CHECK_EQ(reverse_list(h)->val, 0);
  h = build(v);
  CHECK_EQ(middle(h)->val, n / 2 - 1);
  ListNode* tail = h;
  while (tail->next) tail = tail->next;
  tail->next = h->next->next;  // cycle back to node 2
  CHECK(cycle_start(h) == h->next->next);
  tail->next = nullptr;
  ListNode* r = remove_nth_from_end(h, n);
  CHECK_EQ(r->val, 1);
  std::vector<int> a(n / 2), b(n / 2);
  for (int i = 0; i < n / 2; ++i) a[i] = 2 * i, b[i] = 2 * i + 1;
  auto m = to_vec(merge_sorted(build(a), build(b)));
  CHECK(std::is_sorted(m.begin(), m.end()));
  CHECK_EQ(m.size(), (size_t)n);
}
