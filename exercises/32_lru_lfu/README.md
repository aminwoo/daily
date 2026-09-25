# 32 — LRU and LFU caches

```cpp
// Least-recently-used cache.  Both operations O(1).
struct LRUCache {
    LRUCache(int capacity);          // capacity >= 1
    int  get(int key);               // value, or -1 if absent.  Counts as a use.
    void put(int key, int value);    // insert or overwrite; evicts the LRU entry when full
};

// Least-frequently-used cache.  Both operations O(1).
// Evict the entry with the smallest use count; among those, the least recently
// used.  get() and put() on an existing key both count as a use; put() of a new
// key starts it at count 1.
struct LFUCache {
    LFUCache(int capacity);          // capacity >= 1
    int  get(int key);
    void put(int key, int value);
};
```

LRU: `unordered_map<int, list<pair<int,int>>::iterator>` + a `list` you
`splice` to the front. LFU: one such list per frequency plus a `min_freq`
counter — when a key is bumped from `f` to `f+1` and `f`'s list becomes empty
and `f == min_freq`, then `min_freq = f+1`. A new insert always resets
`min_freq = 1`.
