#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
  int size;
  vector<long long> tree;

  explicit Fenwick(int n) : size(n), tree(n + 1, 0) {}

  void add(int i, long long v) {
    for (++i; i <= size; i += i & -i) tree[i] += v;
  }

  long long prefix(int i) const {
    long long sum = 0;
    for (; i > 0; i -= i & -i) sum += tree[i];
    return sum;
  }

  long long range(int l, int r) const { return prefix(r) - prefix(l); }
};
