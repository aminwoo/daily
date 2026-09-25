#include "harness.h"
#include SOLUTION

TEST(basic) {
  Trie t;
  CHECK(!t.contains(""));
  CHECK_EQ(t.count_prefix(""), 0);
  t.insert("apple");
  t.insert("app");
  t.insert("apple");
  t.insert("banana");
  CHECK(t.contains("app"));
  CHECK(t.contains("apple"));
  CHECK(!t.contains("appl"));
  CHECK(!t.contains("ap"));
  CHECK_EQ(t.count_prefix("app"), 3);
  CHECK_EQ(t.count_prefix("appl"), 2);
  CHECK_EQ(t.count_prefix(""), 4);
  CHECK_EQ(t.count_prefix("c"), 0);
  CHECK(t.erase("apple"));
  CHECK(t.contains("apple"));
  CHECK(t.erase("apple"));
  CHECK(!t.contains("apple"));
  CHECK(!t.erase("apple"));
  CHECK_EQ(t.count_prefix("app"), 1);
  t.insert("");
  CHECK(t.contains(""));
  CHECK_EQ(t.count_prefix(""), 3);
}

TEST(random_vs_multiset) {
  using harness::rnd;
  for (int it = 0; it < 20; ++it) {
    Trie t;
    std::multiset<std::string> ms;
    auto word = [&] {
      int n = (int)rnd(0, 5);
      std::string s(n, 'a');
      for (auto& c : s) c = 'a' + rnd(0, 2);
      return s;
    };
    for (int op = 0; op < 500; ++op) {
      std::string w = word();
      int k = (int)rnd(0, 3);
      if (k == 0) {
        t.insert(w);
        ms.insert(w);
      } else if (k == 1) {
        CHECK_EQ(t.contains(w), ms.count(w) > 0);
      } else if (k == 2) {
        int cnt = 0;
        for (auto& s : ms)
          if (s.compare(0, w.size(), w) == 0) ++cnt;
        CHECK_EQ(t.count_prefix(w), cnt);
      } else {
        auto itr = ms.find(w);
        CHECK_EQ(t.erase(w), itr != ms.end());
        if (itr != ms.end()) ms.erase(itr);
      }
    }
  }
}

TEST(perf) {
  using harness::rnd;
  Trie t;
  std::vector<std::string> words;
  for (int i = 0; i < 200000; ++i) {
    std::string s((int)rnd(1, 20), 'a');
    for (auto& c : s) c = 'a' + rnd(0, 25);
    words.push_back(s);
    t.insert(s);
  }
  long long chk = 0;
  for (auto& w : words) {
    CHECK(t.contains(w));
    chk += t.count_prefix(w.substr(0, 2));
  }
  CHECK(chk > 0);
  for (auto& w : words) CHECK(t.erase(w));
  CHECK_EQ(t.count_prefix(""), 0);
}
