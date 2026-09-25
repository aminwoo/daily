#include <bits/stdc++.h>
using namespace std;

int longest_unique_substring(const string& s) {
  int last[256];
  fill(begin(last), end(last), -1);
  int best = 0;
  int left = 0;
  for (int right = 0; right < (int)s.size(); ++right) {
    unsigned char c = s[right];
    if (last[c] >= left) left = last[c] + 1;
    last[c] = right;
    best = max(best, right - left + 1);
  }
  return best;
}

int min_window_len(const string& s, const string& t) {
  if (t.empty() || t.size() > s.size()) return 0;
  int need[256] = {0};
  for (unsigned char c : t) need[c]++;
  int missing = (int)t.size();
  int best = 0;
  int left = 0;
  for (int right = 0; right < (int)s.size(); ++right) {
    if (need[(unsigned char)s[right]]-- > 0) --missing;
    while (missing == 0) {
      if (best == 0 || right - left + 1 < best) best = right - left + 1;
      if (++need[(unsigned char)s[left]] > 0) ++missing;
      ++left;
    }
  }
  return best;
}

long long max_subarray_sum(const vector<int>& a) {
  long long best = a[0];
  long long current = 0;
  for (int x : a) {
    current = max<long long>(current + x, x);
    best = max(best, current);
  }
  return best;
}

long long count_subarrays_with_sum(const vector<int>& a, long long k) {
  unordered_map<long long, long long> seen;
  seen.reserve(a.size() * 2);
  seen[0] = 1;
  long long prefix_sum = 0;
  long long count = 0;
  for (int x : a) {
    prefix_sum += x;
    auto it = seen.find(prefix_sum - k);
    if (it != seen.end()) count += it->second;
    ++seen[prefix_sum];
  }
  return count;
}
