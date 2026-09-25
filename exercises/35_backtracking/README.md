# 35 — Backtracking

```cpp
// all 2^n subsets of a (elements distinct).  Any order; the tests sort.
vector<vector<int>> subsets(const vector<int>& a);

// all DISTINCT permutations of a, which may contain duplicates.  Any order.
vector<vector<int>> permutations_unique(vector<int> a);

// all multisets of candidates (distinct, >= 1, unlimited reuse) summing to
// target, each written in non-decreasing order.  Any order of multisets.
vector<vector<int>> combination_sum(const vector<int>& cand, int target);

// number of ways to place n queens on an n x n board.  n up to 13 in the tests.
int n_queens(int n);  // 0 <= n <= 13; n == 0 has one empty placement

// all well-formed strings of n '(' and n ')'.  Any order.
vector<string> generate_parens(int n);  // n >= 0; n == 0 returns {""}
```

`permutations_unique`: sort, then at each depth skip `a[i]` if it equals
`a[i-1]` and `a[i-1]` is still unused. `combination_sum`: pass a start
index so each multiset is generated exactly once. `n_queens`: three
bool/bit arrays (column, diag `r+c`, anti-diag `r-c+n`) — a `vector<bool>`
board scan is too slow for n = 13.
