#include <bits/stdc++.h>
using namespace std;

struct KthLargest {
  int k;
  priority_queue<int, vector<int>, greater<int>> largest;

  KthLargest(int k, const vector<int>& init) : k(k) {
    for (int x : init) add(x);
  }

  int add(int x) {
    largest.push(x);
    if ((int)largest.size() > k) largest.pop();
    return largest.top();
  }
};

struct MedianFinder {
  priority_queue<int> lower_half;
  priority_queue<int, vector<int>, greater<int>> upper_half;

  void add(int x) {
    if (lower_half.empty() || x <= lower_half.top())
      lower_half.push(x);
    else
      upper_half.push(x);
    if (lower_half.size() > upper_half.size() + 1) {
      upper_half.push(lower_half.top());
      lower_half.pop();
    }
    if (upper_half.size() > lower_half.size()) {
      lower_half.push(upper_half.top());
      upper_half.pop();
    }
  }

  double median() {
    if (lower_half.size() > upper_half.size()) return lower_half.top();
    return (lower_half.top() + static_cast<double>(upper_half.top())) / 2;
  }
};

vector<int> merge_k_sorted(const vector<vector<int>>& lists) {
  using T = tuple<int, int, int>;  // value, list, index
  priority_queue<T, vector<T>, greater<T>> candidates;
  size_t total = 0;
  for (int i = 0; i < (int)lists.size(); ++i) {
    total += lists[i].size();
    if (!lists[i].empty()) candidates.push({lists[i][0], i, 0});
  }
  vector<int> out;
  out.reserve(total);
  while (!candidates.empty()) {
    auto [value, list_index, element_index] = candidates.top();
    candidates.pop();
    out.push_back(value);
    if (element_index + 1 < (int)lists[list_index].size()) {
      candidates.push({lists[list_index][element_index + 1], list_index,
                       element_index + 1});
    }
  }
  return out;
}

vector<int> top_k_frequent(const vector<int>& a, int k) {
  unordered_map<int, int> cnt;
  for (int x : a) cnt[x]++;
  // min-heap on (freq, -value): the worst element is the smallest freq, and
  // among equal freqs the largest value
  using T = pair<int, long long>;
  priority_queue<T, vector<T>, greater<T>> pq;
  for (auto [v, c] : cnt) {
    pq.push({c, -(long long)v});
    if ((int)pq.size() > k) pq.pop();
  }
  vector<int> out(k);
  for (int i = k - 1; i >= 0; --i) {
    out[i] = (int)-pq.top().second;
    pq.pop();
  }
  return out;
}
