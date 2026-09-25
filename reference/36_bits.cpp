#include <bits/stdc++.h>
using namespace std;

int single_number(const vector<int>& a) {
  int result = 0;
  for (int value : a) result ^= value;
  return result;
}

int single_number_thrice(const vector<int>& a) {
  unsigned ones = 0, twos = 0;
  for (int value : a) {
    const unsigned bits = static_cast<unsigned>(value);
    ones = (ones ^ bits) & ~twos;
    twos = (twos ^ bits) & ~ones;
  }
  return (int)ones;
}

pair<int, int> two_singles(const vector<int>& a) {
  unsigned combined = 0;
  for (int value : a) combined ^= static_cast<unsigned>(value);
  const unsigned distinguishing_bit = combined & -combined;
  int first = 0;
  int second = 0;
  for (int value : a) {
    if (static_cast<unsigned>(value) & distinguishing_bit)
      first ^= value;
    else
      second ^= value;
  }
  if (first > second) swap(first, second);
  return {first, second};
}

vector<int> count_bits(int n) {
  vector<int> result(n + 1);
  for (int i = 1; i <= n; ++i) result[i] = result[i >> 1] + (i & 1);
  return result;
}

uint32_t reverse_bits(uint32_t x) {
  x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
  x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
  x = ((x >> 4) & 0x0F0F0F0Fu) | ((x & 0x0F0F0F0Fu) << 4);
  x = ((x >> 8) & 0x00FF00FFu) | ((x & 0x00FF00FFu) << 8);
  return (x >> 16) | (x << 16);
}

int max_xor_pair(const vector<int>& a) {
  // binary trie in flat arrays
  vector<array<int, 2>> t(1, {0, 0});
  t.reserve(a.size() * 31);
  for (int v : a) {
    int cur = 0;
    for (int b = 29; b >= 0; --b) {
      int bit = v >> b & 1;
      if (!t[cur][bit]) {
        t[cur][bit] = t.size();
        t.push_back({0, 0});
      }
      cur = t[cur][bit];
    }
  }
  int best = 0;
  for (int v : a) {
    int cur = 0, x = 0;
    for (int b = 29; b >= 0; --b) {
      int bit = v >> b & 1;
      if (t[cur][bit ^ 1]) {
        x |= 1 << b;
        cur = t[cur][bit ^ 1];
      } else
        cur = t[cur][bit];
    }
    best = max(best, x);
  }
  return best;
}
