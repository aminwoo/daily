# 30 — Intervals

All intervals are half-open `[l, r)` with `l < r`, as `pair<long long, long long>`.

```cpp
using Iv = pair<long long, long long>;

// union of the input as a sorted list of disjoint intervals.
// Touching intervals merge: [1,3) + [3,5) -> [1,5).
vector<Iv> merge_intervals(vector<Iv> v);

// v is sorted and pairwise disjoint; return the same with x merged in.  O(n).
vector<Iv> insert_interval(const vector<Iv>& v, Iv x);

// smallest number of rooms so that no two meetings in the same room overlap
// ([1,3) and [3,5) can share a room).  0 for no meetings.
int min_rooms(const vector<Iv>& v);

// largest subset of pairwise non-overlapping intervals (activity selection)
int max_non_overlapping(vector<Iv> v);
```

Everything is O(n log n) via a sort — but sort by the right key for each
one. `min_rooms` is the classic "sort starts and ends separately" sweep or a
min-heap of end times; `max_non_overlapping` is greedy by earliest end.
