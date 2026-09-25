#include <bits/stdc++.h>
using namespace std;

struct Trie {
  vector<array<int, 26>> children;
  vector<int> prefix_count;
  vector<int> word_count;

  Trie() {
    children.push_back({});
    children[0].fill(-1);
    prefix_count.push_back(0);
    word_count.push_back(0);
  }

  int walk(const string& s) {
    int node = 0;
    for (char c : s) {
      node = children[node][c - 'a'];
      if (node < 0) return -1;
    }
    return node;
  }

  void insert(const string& s) {
    int node = 0;
    ++prefix_count[0];
    for (char c : s) {
      int next = children[node][c - 'a'];
      if (next < 0) {
        next = children.size();
        children[node][c - 'a'] = next;
        children.push_back({});
        children.back().fill(-1);
        prefix_count.push_back(0);
        word_count.push_back(0);
      }
      node = next;
      ++prefix_count[node];
    }
    ++word_count[node];
  }

  bool contains(const string& s) {
    int node = walk(s);
    return node >= 0 && word_count[node] > 0;
  }

  int count_prefix(const string& p) {
    int node = walk(p);
    return node < 0 ? 0 : prefix_count[node];
  }

  bool erase(const string& s) {
    if (!contains(s)) return false;
    int node = 0;
    --prefix_count[0];
    for (char c : s) {
      node = children[node][c - 'a'];
      --prefix_count[node];
    }
    --word_count[node];
    return true;
  }
};
