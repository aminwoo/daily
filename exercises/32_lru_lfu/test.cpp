#include "harness.h"
#include SOLUTION

// O(capacity) per op models of both policies
struct SlowLRU {
  int cap;
  std::vector<std::pair<int, int>> v;  // back = most recent

  int get(int k) {
    for (size_t i = 0; i < v.size(); ++i)
      if (v[i].first == k) {
        auto e = v[i];
        v.erase(v.begin() + i);
        v.push_back(e);
        return e.second;
      }
    return -1;
  }

  void put(int k, int val) {
    if (get(k) != -1) {
      v.back().second = val;
      return;
    }
    if ((int)v.size() == cap) v.erase(v.begin());
    v.push_back({k, val});
  }
};

struct SlowLFU {
  struct E {
    int key, val, freq;
    long long last;
  };

  int cap;
  long long clock = 0;
  std::vector<E> v;

  int get(int k) {
    for (auto& e : v)
      if (e.key == k) {
        e.freq++;
        e.last = ++clock;
        return e.val;
      }
    return -1;
  }

  void put(int k, int val) {
    for (auto& e : v)
      if (e.key == k) {
        e.val = val;
        e.freq++;
        e.last = ++clock;
        return;
      }
    if ((int)v.size() == cap) {
      size_t worst = 0;
      for (size_t i = 1; i < v.size(); ++i)
        if (v[i].freq < v[worst].freq ||
            (v[i].freq == v[worst].freq && v[i].last < v[worst].last))
          worst = i;
      v.erase(v.begin() + worst);
    }
    v.push_back({k, val, 1, ++clock});
  }
};

TEST(basic_lru) {
  LRUCache c(2);
  c.put(1, 1);
  c.put(2, 2);
  CHECK_EQ(c.get(1), 1);
  c.put(3, 3);  // evicts 2
  CHECK_EQ(c.get(2), -1);
  c.put(4, 4);  // evicts 1
  CHECK_EQ(c.get(1), -1);
  CHECK_EQ(c.get(3), 3);
  CHECK_EQ(c.get(4), 4);
  c.put(4, 40);
  CHECK_EQ(c.get(4), 40);
  LRUCache one(1);
  one.put(5, 5);
  one.put(6, 6);
  CHECK_EQ(one.get(5), -1);
  CHECK_EQ(one.get(6), 6);
}

TEST(basic_lfu) {
  LFUCache c(2);
  c.put(1, 1);
  c.put(2, 2);
  CHECK_EQ(c.get(1), 1);
  c.put(3, 3);  // evicts 2 (freq 1 < freq 2)
  CHECK_EQ(c.get(2), -1);
  CHECK_EQ(c.get(3), 3);
  c.put(4, 4);  // 1 and 3 both freq 2; 1 is older -> evict 1
  CHECK_EQ(c.get(1), -1);
  CHECK_EQ(c.get(3), 3);
  CHECK_EQ(c.get(4), 4);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    int cap = (int)rnd(1, 5), keys = (int)rnd(1, 8);
    LRUCache lru(cap);
    SlowLRU slru{cap, {}};
    LFUCache lfu(cap);
    SlowLFU slfu{cap, 0, {}};
    for (int op = 0; op < 200; ++op) {
      int k = (int)rnd(0, keys - 1);
      if (rnd(0, 1)) {
        CHECK_EQ(lru.get(k), slru.get(k));
        CHECK_EQ(lfu.get(k), slfu.get(k));
      } else {
        int v = (int)rnd(0, 100);
        lru.put(k, v);
        slru.put(k, v);
        lfu.put(k, v);
        slfu.put(k, v);
      }
    }
  }
}

TEST(perf) {
  using harness::rnd;
  int cap = 200000, n = 2000000;
  LRUCache lru(cap);
  LFUCache lfu(cap);
  long long s = 0;
  for (int i = 0; i < n; ++i) {
    int k = (int)rnd(0, 400000);
    if (i & 1) {
      s += lru.get(k);
      s += lfu.get(k);
    } else {
      lru.put(k, i);
      lfu.put(k, i);
    }
  }
  CHECK(s != 0);
  // sequential scan: every put evicts, every get misses
  LRUCache scan(1000);
  for (int i = 0; i < n; ++i) scan.put(i, i);
  CHECK_EQ(scan.get(0), -1);
  CHECK_EQ(scan.get(n - 1), n - 1);
}
