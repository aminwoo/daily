# 19 — DP classics

```cpp
// 0/1 knapsack: w.size() == v.size(); max value at weight <= W.
// w[i] >= 1, W >= 0; answer and DP additions fit in long long.  O(n*W), 1-D array.
long long knapsack01(const vector<int>& w, const vector<long long>& v, int W);

// number of ways to make target >= 0 from distinct positive coin denominations
// with unlimited copies (order doesn't matter).  The answer fits in long long.
long long count_ways(const vector<int>& coins, int target);

// fewest coins to make target >= 0 from positive denominations (unlimited), or -1
int min_coins(const vector<int>& coins, int target);

// length of the longest common subsequence
int lcs(const string& a, const string& b);
```

The knapsack and `count_ways` loops look almost identical — one iterates
capacity downward, the other upward. Make sure you know why.
