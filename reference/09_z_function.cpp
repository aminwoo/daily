#include <bits/stdc++.h>
using namespace std;

namespace {
template <typename Sequence>
vector<int> compute_z_values(const Sequence& s) {
  const int n = static_cast<int>(s.size());
  vector<int> z(n, 0);
  if (n) z[0] = n;
  int left = 0;
  int right = 0;
  for (int i = 1; i < n; ++i) {
    if (i < right) z[i] = min(right - i, z[i - left]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
    if (i + z[i] > right) {
      left = i;
      right = i + z[i];
    }
  }
  return z;
}
}  // namespace

vector<int> z_function(const string& s) { return compute_z_values(s); }

long long count_occurrences(const string& text, const string& pat) {
  vector<int> s;
  s.reserve(pat.size() + 1 + text.size());
  for (unsigned char c : pat) s.push_back(c);
  s.push_back(256);  // outside the range of every possible byte
  for (unsigned char c : text) s.push_back(c);

  const vector<int> z = compute_z_values(s);
  long long count = 0;
  const int pattern_length = static_cast<int>(pat.size());
  for (int i = pattern_length + 1; i < (int)s.size(); ++i) {
    if (z[i] >= pattern_length) ++count;
  }
  return count;
}
