#include <bits/stdc++.h>
using namespace std;

long long first_true(long long lo, long long hi,
                     const function<bool(long long)>& pred) {
  while (lo < hi) {
    // (ull)hi - (ull)lo is the exact difference even when hi - lo would
    // overflow long long
    long long mid =
        lo + (long long)(((unsigned long long)hi - (unsigned long long)lo) / 2);
    if (pred(mid))
      hi = mid;
    else
      lo = mid + 1;
  }
  return lo;
}

int lower_bound_index(const vector<int>& a, int x) {
  int lo = 0, hi = a.size();
  while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (a[mid] >= x)
      hi = mid;
    else
      lo = mid + 1;
  }
  return lo;
}

long long isqrt(long long n) {
  long long lo = 0, hi = 3037000500LL;  // hi*hi overflows? 3037000500^2 > 2^63
                                        // so treat hi as exclusive
  while (lo < hi) {
    long long mid = lo + (hi - lo) / 2;
    if (mid <= 3037000499LL && mid * mid <= n)
      lo = mid + 1;
    else
      hi = mid;
  }
  return lo - 1;
}
