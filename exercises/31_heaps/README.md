# 31 — Heaps

```cpp
// k-th largest element of a stream.  add() returns the k-th largest of
// everything added so far (the tests only call it once >= k elements exist).
struct KthLargest {
    KthLargest(int k, const vector<int>& init);  // k >= 1
    int add(int x);
};

// running median.  median() = middle element, or mean of the two middles.
struct MedianFinder {
    void   add(int x);
    double median();     // only called when non-empty
};

// merge k sorted vectors into one sorted vector.  O(N log k), N = total size.
vector<int> merge_k_sorted(const vector<vector<int>>& lists);

// the k most frequent values, ordered by frequency desc, ties by value asc.
// k <= number of distinct values.  O(n log k) (or O(n) with bucket sort).
vector<int> top_k_frequent(const vector<int>& a, int k); // 1 <= k <= distinct values
```

`KthLargest` is a min-heap capped at k. `MedianFinder` is two heaps (max-heap
of the lower half, min-heap of the upper half) kept balanced within one
element. Know `priority_queue<T, vector<T>, greater<T>>` cold.
