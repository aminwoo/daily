# 18 — Longest increasing subsequence in O(n log n)

```cpp
// length of the longest STRICTLY increasing subsequence
int lis_length(const vector<int>& a);

// one longest strictly increasing subsequence (any valid one); empty for empty input
vector<int> lis_sequence(const vector<int>& a);
```

Maintain `tails[k]` = smallest possible last element of an increasing
subsequence of length k+1, updated with a binary search. For the
reconstruction, store for each element the index of its predecessor.
Perf: n = 1e6 (an O(n²) DP won't make it).
