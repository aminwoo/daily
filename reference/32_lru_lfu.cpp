#include <bits/stdc++.h>
using namespace std;

struct LRUCache {
  int capacity;
  list<pair<int, int>> order;  // front = most recent
  unordered_map<int, list<pair<int, int>>::iterator> position;

  explicit LRUCache(int capacity) : capacity(capacity) {}

  int get(int key) {
    auto it = position.find(key);
    if (it == position.end()) return -1;
    order.splice(order.begin(), order, it->second);
    return it->second->second;
  }

  void put(int key, int value) {
    auto it = position.find(key);
    if (it != position.end()) {
      it->second->second = value;
      order.splice(order.begin(), order, it->second);
      return;
    }
    if ((int)order.size() == capacity) {
      position.erase(order.back().first);
      order.pop_back();
    }
    order.push_front({key, value});
    position[key] = order.begin();
  }
};

struct LFUCache {
  struct Node {
    int key, val, freq;
  };

  int capacity;
  int minimum_frequency = 0;
  unordered_map<int, list<Node>> by_frequency;  // front = most recent
  unordered_map<int, list<Node>::iterator> position;

  explicit LFUCache(int capacity) : capacity(capacity) {}

  void touch(list<Node>::iterator it) {
    int f = it->freq;
    auto& src = by_frequency[f];
    auto& dst = by_frequency[f + 1];
    dst.splice(dst.begin(), src, it);
    it->freq = f + 1;
    if (src.empty()) {
      by_frequency.erase(f);
      if (minimum_frequency == f) minimum_frequency = f + 1;
    }
  }

  int get(int key) {
    auto it = position.find(key);
    if (it == position.end()) return -1;
    touch(it->second);
    return it->second->val;
  }

  void put(int key, int value) {
    auto it = position.find(key);
    if (it != position.end()) {
      it->second->val = value;
      touch(it->second);
      return;
    }
    if ((int)position.size() == capacity) {
      auto& l = by_frequency[minimum_frequency];
      position.erase(l.back().key);
      l.pop_back();
      if (l.empty()) by_frequency.erase(minimum_frequency);
    }
    by_frequency[1].push_front({key, value, 1});
    position[key] = by_frequency[1].begin();
    minimum_frequency = 1;
  }
};
