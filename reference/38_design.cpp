#include <bits/stdc++.h>
using namespace std;

struct MinStack {
  vector<pair<int, int>> values;  // (value, minimum so far)

  void push(int x) {
    values.push_back(
        {x, values.empty() ? x : std::min(x, values.back().second)});
  }

  void pop() { values.pop_back(); }

  int top() const { return values.back().first; }

  int min() const { return values.back().second; }
};

struct TimeMap {
  unordered_map<string, vector<pair<int, string>>> history;

  void set(const string& key, const string& value, int ts) {
    history[key].push_back({ts, value});
  }

  string get(const string& key, int ts) {
    auto it = history.find(key);
    if (it == history.end()) return "";
    auto& v = it->second;
    auto p = upper_bound(
        v.begin(), v.end(), ts,
        [](int t, const pair<int, string>& e) { return t < e.first; });
    if (p == v.begin()) return "";
    return prev(p)->second;
  }
};

struct RandomizedSet {
  vector<int> values;
  unordered_map<int, int> index;
  mt19937 rng{12345};

  bool insert(int x) {
    if (index.count(x)) return false;
    index[x] = values.size();
    values.push_back(x);
    return true;
  }

  bool remove(int x) {
    auto it = index.find(x);
    if (it == index.end()) return false;
    int i = it->second;
    values[i] = values.back();
    index[values[i]] = i;
    values.pop_back();
    index.erase(it);
    return true;
  }

  int get_random() {
    uniform_int_distribution<size_t> pick(0, values.size() - 1);
    return values[pick(rng)];
  }
};
