#include <bits/stdc++.h>
using namespace std;

struct DSU {
  DSU(int n) {}

  int find(int x) { return x; }

  bool unite(int a, int b) { return false; }

  int size(int x) { return 1; }

  int components() { return 0; }
};
