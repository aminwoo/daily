# 15 — Monotonic stack & monotonic deque

```cpp
// max of every window of k consecutive elements (1 <= k <= n); result has n-k+1 entries
vector<int> sliding_window_max(const vector<int>& a, int k);

// ps[i] = largest j < i with a[j] < a[i], or -1
vector<int> previous_smaller(const vector<int>& a);

// largest rectangle area under a histogram with bar heights h (h[i] >= 0, width 1 each)
long long largest_rectangle(const vector<int>& h);
```

All three in O(n): a deque of candidate indices for the window, a stack of
indices for the other two. For `largest_rectangle`, a sentinel of height 0
appended to the end saves a second pass.
