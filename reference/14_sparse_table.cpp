#include <bits/stdc++.h>
using namespace std;

struct SparseTable {
  vector<vector<int>> table;
  vector<int> floor_log2;

  SparseTable(const vector<int>& a) {
    const int n = static_cast<int>(a.size());
    floor_log2.assign(n + 1, 0);
    for (int i = 2; i <= n; ++i) floor_log2[i] = floor_log2[i / 2] + 1;
    table.assign(floor_log2[n] + 1, vector<int>(n));
    table[0] = a;
    for (int level = 1; level <= floor_log2[n]; ++level) {
      const int half = 1 << (level - 1);
      for (int i = 0; i + 2 * half <= n; ++i) {
        table[level][i] = min(table[level - 1][i], table[level - 1][i + half]);
      }
    }
  }

  int query(int l, int r) const {
    const int level = floor_log2[r - l];
    return min(table[level][l], table[level][r - (1 << level)]);
  }
};
