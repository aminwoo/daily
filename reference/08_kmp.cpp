#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_function(const string& s) {
  int n = s.size();
  vector<int> pi(n, 0);
  for (int i = 1; i < n; ++i) {
    int matched = pi[i - 1];
    while (matched > 0 && s[i] != s[matched]) matched = pi[matched - 1];
    if (s[i] == s[matched]) ++matched;
    pi[i] = matched;
  }
  return pi;
}

vector<int> find_occurrences(const string& text, const string& pat) {
  const vector<int> pi = prefix_function(pat);
  vector<int> occurrences;
  const int pattern_length = static_cast<int>(pat.size());
  int matched = 0;
  for (int i = 0; i < (int)text.size(); ++i) {
    while (matched > 0 &&
           (matched == pattern_length || text[i] != pat[matched])) {
      matched = pi[matched - 1];
    }
    if (text[i] == pat[matched]) ++matched;
    if (matched == pattern_length) {
      occurrences.push_back(i - pattern_length + 1);
    }
  }
  return occurrences;
}
