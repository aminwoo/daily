#include <bits/stdc++.h>
using namespace std;

struct SegTree {
  int size;
  vector<long long> tree;

  SegTree(const vector<long long>& a) : size(1) {
    while (size < static_cast<int>(a.size())) size *= 2;
    tree.assign(2 * size, LLONG_MAX);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) tree[size + i] = a[i];
    for (int i = size - 1; i > 0; --i) {
      tree[i] = min(tree[2 * i], tree[2 * i + 1]);
    }
  }

  void set(int i, long long v) {
    i += size;
    tree[i] = v;
    while (i > 1) {
      tree[i / 2] = min(tree[i], tree[i ^ 1]);
      i /= 2;
    }
  }

  long long query(int l, int r) const {
    long long answer = LLONG_MAX;
    for (l += size, r += size; l < r; l /= 2, r /= 2) {
      if (l & 1) {
        answer = min(answer, tree.at(l));
        ++l;
      }
      if (r & 1) {
        --r;
        answer = min(answer, tree.at(r));
      }
    }
    return answer;
  }
};
