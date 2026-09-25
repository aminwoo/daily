#include <bits/stdc++.h>
using namespace std;

int lis_length(const vector<int>& a) {
  vector<int> tails;
  for (int x : a) {
    auto it = lower_bound(tails.begin(), tails.end(), x);
    if (it == tails.end())
      tails.push_back(x);
    else
      *it = x;
  }
  return tails.size();
}

vector<int> lis_sequence(const vector<int>& a) {
  int n = a.size();
  vector<int> tails, idx, prev(n, -1);  // idx[k] = index in a of tails[k]
  for (int i = 0; i < n; ++i) {
    int k = lower_bound(tails.begin(), tails.end(), a[i]) - tails.begin();
    if (k == (int)tails.size()) {
      tails.push_back(a[i]);
      idx.push_back(i);
    } else {
      tails[k] = a[i];
      idx[k] = i;
    }
    if (k > 0) prev[i] = idx[k - 1];
  }
  vector<int> res;
  for (int i = idx.empty() ? -1 : idx.back(); i != -1; i = prev[i])
    res.push_back(a[i]);
  reverse(res.begin(), res.end());
  return res;
}
