#include <bits/stdc++.h>
using namespace std;

vector<int> sliding_window_max(const vector<int>& a, int k) {
  vector<int> result;
  deque<int> candidates;
  for (int i = 0; i < (int)a.size(); ++i) {
    while (!candidates.empty() && a[candidates.back()] <= a[i]) {
      candidates.pop_back();
    }
    candidates.push_back(i);
    if (candidates.front() <= i - k) candidates.pop_front();
    if (i >= k - 1) result.push_back(a[candidates.front()]);
  }
  return result;
}

vector<int> previous_smaller(const vector<int>& a) {
  vector<int> result(a.size(), -1);
  vector<int> stack;
  for (int i = 0; i < (int)a.size(); ++i) {
    while (!stack.empty() && a[stack.back()] >= a[i]) stack.pop_back();
    if (!stack.empty()) result[i] = stack.back();
    stack.push_back(i);
  }
  return result;
}

long long largest_rectangle(const vector<int>& h) {
  long long best = 0;
  vector<int> stack;
  for (int i = 0; i <= (int)h.size(); ++i) {
    const int current_height = i == (int)h.size() ? 0 : h[i];
    while (!stack.empty() && h[stack.back()] >= current_height) {
      const int height = h[stack.back()];
      stack.pop_back();
      const int left = stack.empty() ? -1 : stack.back();
      best = max(best, (long long)height * (i - left - 1));
    }
    if (i < (int)h.size()) stack.push_back(i);
  }
  return best;
}
