#include <bits/stdc++.h>
using namespace std;

struct LazySegTree {
  int size;
  vector<long long> tree;
  vector<long long> lazy;

  LazySegTree(const vector<long long>& a)
      : size(a.size()), tree(4 * a.size()), lazy(4 * a.size(), 0) {
    build(1, 0, size, a);
  }

  void build(int node, int left, int right, const vector<long long>& a) {
    if (right - left == 1) {
      tree[node] = a[left];
      return;
    }
    int middle = left + (right - left) / 2;
    build(2 * node, left, middle, a);
    build(2 * node + 1, middle, right, a);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
  }

  void apply(int node, int left, int right, long long value) {
    tree[node] += value * (right - left);
    lazy[node] += value;
  }

  void push(int node, int left, int right) {
    if (lazy[node] == 0) return;
    int middle = left + (right - left) / 2;
    apply(2 * node, left, middle, lazy[node]);
    apply(2 * node + 1, middle, right, lazy[node]);
    lazy[node] = 0;
  }

  void add(int node, int left, int right, int query_left, int query_right,
           long long value) {
    if (query_right <= left || right <= query_left) return;
    if (query_left <= left && right <= query_right) {
      apply(node, left, right, value);
      return;
    }
    push(node, left, right);
    int middle = left + (right - left) / 2;
    add(2 * node, left, middle, query_left, query_right, value);
    add(2 * node + 1, middle, right, query_left, query_right, value);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
  }

  long long sum(int node, int left, int right, int query_left,
                int query_right) {
    if (query_right <= left || right <= query_left) return 0;
    if (query_left <= left && right <= query_right) return tree[node];
    push(node, left, right);
    int middle = left + (right - left) / 2;
    return sum(2 * node, left, middle, query_left, query_right) +
           sum(2 * node + 1, middle, right, query_left, query_right);
  }

  void add(int l, int r, long long v) {
    if (l < r) add(1, 0, size, l, r, v);
  }

  long long sum(int l, int r) { return l < r ? sum(1, 0, size, l, r) : 0; }
};
