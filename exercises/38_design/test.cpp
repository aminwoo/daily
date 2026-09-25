#include "harness.h"
#include SOLUTION

TEST(basic) {
  MinStack s;
  s.push(-2);
  s.push(0);
  s.push(-3);
  CHECK_EQ(s.min(), -3);
  s.pop();
  CHECK_EQ(s.top(), 0);
  CHECK_EQ(s.min(), -2);
  s.push(-2);
  s.push(5);
  s.pop();
  s.pop();
  CHECK_EQ(s.min(), -2);  // duplicate min popped once, still -2

  TimeMap tm;
  tm.set("foo", "bar", 1);
  CHECK_EQ(tm.get("foo", 1), "bar");
  CHECK_EQ(tm.get("foo", 3), "bar");
  CHECK_EQ(tm.get("foo", 0), "");
  tm.set("foo", "bar2", 4);
  CHECK_EQ(tm.get("foo", 4), "bar2");
  CHECK_EQ(tm.get("foo", 5), "bar2");
  CHECK_EQ(tm.get("foo", 3), "bar");
  CHECK_EQ(tm.get("nope", 3), "");

  RandomizedSet r;
  CHECK(r.insert(1));
  CHECK(!r.remove(2));
  CHECK(r.insert(2));
  int g = r.get_random();
  CHECK(g == 1 || g == 2);
  CHECK(r.remove(1));
  CHECK(!r.insert(2));
  CHECK_EQ(r.get_random(), 2);
  CHECK(r.remove(2));
  CHECK(!r.remove(2));
  CHECK(r.insert(2));
  CHECK_EQ(r.get_random(), 2);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    MinStack s;
    std::vector<int> ref;
    for (int op = 0; op < 100; ++op) {
      if (ref.empty() || rnd(0, 2)) {
        int x = (int)rnd(-10, 10);
        s.push(x);
        ref.push_back(x);
      } else {
        s.pop();
        ref.pop_back();
      }
      if (!ref.empty()) {
        CHECK_EQ(s.top(), ref.back());
        CHECK_EQ(s.min(), *std::min_element(ref.begin(), ref.end()));
      }
    }

    TimeMap tm;
    std::map<std::string, std::vector<std::pair<int, std::string>>> ref_tm;
    for (int op = 0; op < 100; ++op) {
      std::string key = std::string(1, (char)('a' + rnd(0, 2)));
      if (rnd(0, 1)) {
        auto& v = ref_tm[key];
        int ts = (v.empty() ? 0 : v.back().first) + (int)rnd(1, 5);
        std::string val = "v" + std::to_string(op);
        tm.set(key, val, ts);
        v.push_back({ts, val});
      } else {
        int ts = (int)rnd(0, 60);
        std::string exp;
        for (auto& [t, val] : ref_tm[key])
          if (t <= ts) exp = val;
        CHECK_EQ(tm.get(key, ts), exp);
      }
    }

    RandomizedSet rs;
    std::set<int> ref_set;
    for (int op = 0; op < 100; ++op) {
      int x = (int)rnd(0, 6);
      if (rnd(0, 1)) {
        CHECK_EQ(rs.insert(x), ref_set.insert(x).second);
      } else {
        CHECK_EQ(rs.remove(x), ref_set.erase(x) == 1);
      }
      if (!ref_set.empty()) CHECK(ref_set.count(rs.get_random()));
    }
  }
  // uniformity, loosely: with 3 elements and 3000 draws each shows >= 700
  RandomizedSet rs;
  for (int x : {10, 20, 30}) rs.insert(x);
  std::map<int, int> hist;
  for (int i = 0; i < 3000; ++i) hist[rs.get_random()]++;
  CHECK_EQ(hist.size(), (size_t)3);
  for (auto [x, c] : hist) CHECK(c >= 700);
  // after swap-remove, the survivors are still all reachable
  rs.remove(10);
  hist.clear();
  for (int i = 0; i < 1000; ++i) hist[rs.get_random()]++;
  CHECK_EQ(hist.size(), (size_t)2);
}

TEST(perf) {
  using harness::rnd;
  int n = 2000000;
  MinStack s;
  long long acc = 0;
  for (int i = 0; i < n; ++i) {
    s.push((int)rnd(-1000000, 1000000));
    acc += s.min();
    if (i % 3 == 2) s.pop();
  }
  CHECK(acc < 0);
  TimeMap tm;
  for (int i = 0; i < 300000; ++i)
    tm.set("k" + std::to_string(i % 100), "v", i);
  for (int i = 0; i < 300000; ++i)
    acc += tm.get("k" + std::to_string(i % 100), i).size();
  CHECK(acc != 0);
  RandomizedSet rs;
  rs.insert(-1);  // never removed, so get_random() is always legal
  for (int i = 0; i < n; ++i) {
    int x = (int)rnd(0, 500000);
    if (rnd(0, 1))
      rs.insert(x);
    else
      rs.remove(x);
    if (i % 2 == 1) acc += rs.get_random();
  }
  CHECK(acc != 0);
}
