# 38 — "Design a data structure" questions

Three classics. Every operation must be O(1) (amortised / expected).

```cpp
// stack that also reports its minimum
struct MinStack {
    void push(int x);
    void pop();          // never called on an empty stack
    int  top();          // only called on a non-empty stack
    int  min();          // only called on a non-empty stack
};

// key -> (value, timestamp) store.  Per key, set() is called with strictly
// increasing timestamps.  get(key, ts) returns the value with the largest
// timestamp <= ts, or "" if there is none.  get() is O(log n) (binary search).
struct TimeMap {
    void   set(const string& key, const string& value, int ts);
    string get(const string& key, int ts);
};

// set with O(1) insert / remove / uniform random element
struct RandomizedSet {
    bool insert(int x);  // false if already present
    bool remove(int x);  // false if absent
    int  get_random();   // uniformly random element; never called when empty
};
```

`MinStack`: a second stack of running minima (or store `(value, min_so_far)`
pairs). `RandomizedSet`: vector + `unordered_map<value, index>`; remove by
swapping the victim with the last element. Use any RNG you like for
`get_random` — the test only checks that every element shows up over many
draws.
