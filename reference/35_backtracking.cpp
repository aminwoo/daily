#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> subsets(const vector<int>& a) {
  vector<vector<int>> out;
  vector<int> cur;
  auto rec = [&](auto& self, size_t i) -> void {
    if (i == a.size()) {
      out.push_back(cur);
      return;
    }
    self(self, i + 1);
    cur.push_back(a[i]);
    self(self, i + 1);
    cur.pop_back();
  };
  rec(rec, 0);
  return out;
}

vector<vector<int>> permutations_unique(vector<int> a) {
  sort(a.begin(), a.end());
  vector<vector<int>> out;
  vector<int> cur;
  vector<bool> used(a.size());
  auto rec = [&](auto& self) -> void {
    if (cur.size() == a.size()) {
      out.push_back(cur);
      return;
    }
    for (size_t i = 0; i < a.size(); ++i) {
      if (used[i] || (i && a[i] == a[i - 1] && !used[i - 1])) continue;
      used[i] = true;
      cur.push_back(a[i]);
      self(self);
      cur.pop_back();
      used[i] = false;
    }
  };
  rec(rec);
  return out;
}

vector<vector<int>> combination_sum(const vector<int>& cand, int target) {
  vector<vector<int>> out;
  vector<int> cur;
  auto rec = [&](auto& self, size_t start, int rem) -> void {
    if (rem == 0) {
      out.push_back(cur);
      return;
    }
    for (size_t i = start; i < cand.size(); ++i) {
      if (cand[i] > rem) continue;
      cur.push_back(cand[i]);
      self(self, i, rem - cand[i]);
      cur.pop_back();
    }
  };
  rec(rec, 0, target);
  for (auto& v : out) sort(v.begin(), v.end());
  return out;
}

int n_queens(int n) {
  int cnt = 0;
  vector<bool> col(n), d1(2 * n), d2(2 * n);
  auto rec = [&](auto& self, int r) -> void {
    if (r == n) {
      ++cnt;
      return;
    }
    for (int c = 0; c < n; ++c) {
      if (col[c] || d1[r + c] || d2[r - c + n]) continue;
      col[c] = d1[r + c] = d2[r - c + n] = true;
      self(self, r + 1);
      col[c] = d1[r + c] = d2[r - c + n] = false;
    }
  };
  rec(rec, 0);
  return cnt;
}

vector<string> generate_parens(int n) {
  vector<string> out;
  string cur;
  auto rec = [&](auto& self, int open, int close) -> void {
    if (open == n && close == n) {
      out.push_back(cur);
      return;
    }
    if (open < n) {
      cur += '(';
      self(self, open + 1, close);
      cur.pop_back();
    }
    if (close < open) {
      cur += ')';
      self(self, open, close + 1);
      cur.pop_back();
    }
  };
  rec(rec, 0, 0);
  return out;
}
