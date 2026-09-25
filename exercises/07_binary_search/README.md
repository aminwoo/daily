# 07 — Binary search (three flavours)

No `std::lower_bound` / `std::upper_bound` / `sqrt` allowed — that's the point.

```cpp
// pred is monotone on [lo, hi): false...false true...true.
// Return the smallest x in [lo, hi) with pred(x) == true, or hi if there is none.
// lo, hi can be anywhere in the long long range — watch for overflow in (lo + hi) / 2,
// and call pred at most ~64 times.
long long first_true(long long lo, long long hi, const function<bool(long long)>& pred);

// a is sorted ascending. Index of the first element >= x (a.size() if none).
int lower_bound_index(const vector<int>& a, int x);

// largest r >= 0 with r*r <= n,  for 0 <= n <= 1e18  (exact — no floating point)
long long isqrt(long long n);
```
