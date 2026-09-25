#include <bits/stdc++.h>
using namespace std;

struct MinStack {
  void push(int x) {}

  void pop() {}

  int top() { return 0; }

  int min() { return 0; }
};

struct TimeMap {
  void set(const string& key, const string& value, int ts) {}

  string get(const string& key, int ts) { return ""; }
};

struct RandomizedSet {
  bool insert(int x) { return false; }

  bool remove(int x) { return false; }

  int get_random() { return 0; }
};
