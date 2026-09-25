# 29 — Sliding window & prefix-sum hashing

```cpp
// length of the longest substring with all-distinct bytes (0 for "")
int longest_unique_substring(const string& s);

// length of the shortest substring of s containing every char of t (with
// multiplicity); 0 if none exists.  Bytes are arbitrary (use a 256-entry table).
int min_window_len(const string& s, const string& t);

// maximum sum of a non-empty contiguous subarray (Kadane); a is non-empty
long long max_subarray_sum(const vector<int>& a);

// number of contiguous subarrays whose sum is exactly k (prefix sums + hash map;
// values may be negative, so a two-pointer window does NOT work)
long long count_subarrays_with_sum(const vector<int>& a, long long k);
```

Array prefix sums, subarray sums, and the number of matching subarrays fit in
`long long`.

All four in O(n). The two window problems share the shape: advance `r`,
add `s[r]`, then shrink from `l` while the window is invalid / still valid.
Be precise about which of those two you need for each.
