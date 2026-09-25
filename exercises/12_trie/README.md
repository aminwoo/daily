# 12 — Trie

Lowercase `a-z` words. Words can be inserted more than once (multiset semantics).

```cpp
struct Trie {
    void insert(const string& s);        // add one copy of s ("" is a valid word)
    bool contains(const string& s);      // at least one copy of s is present
    int  count_prefix(const string& p);  // number of stored words (with multiplicity) starting with p
    bool erase(const string& s);         // remove one copy; false if s is not present
};
```

Store nodes in a `vector<array<int,26>>` (or a `vector<Node>`) rather than
raw `new` — it is faster and there is nothing to free. Keep a per-node count
of words passing through it so that `count_prefix` is O(|p|).

Gotcha: if you take a reference/pointer to a node and then `push_back` a new
node, the vector may reallocate and your reference dangles. The tests run
under AddressSanitizer, so this shows up as `heap-use-after-free`.
