# 25 — Matrix exponentiation

```cpp
using Mat = vector<vector<long long>>;

Mat mat_mul(const Mat& A, const Mat& B, long long mod);   // compatible, non-empty rectangular matrices
Mat mat_pow(Mat A, long long e, long long mod);           // non-empty square matrix; e >= 0

long long fib(long long n, long long mod);                // F(0)=0, F(1)=1, n up to 1e18

// number of walks of exactly k edges from s to t in the graph with 0/1 adjacency matrix adj
long long count_walks(const vector<vector<int>>& adj, int s, int t, long long k, long long mod);
```

For every function, `1 <= mod <= 2e9`; matrix entries are in `[0, mod)`, and
`e = 0` produces the identity (whose diagonal is `1 % mod`). The adjacency
matrix is non-empty and square, with valid `s` and `t`.

Perf: `mat_pow` of a 60x60 matrix to the power 1e18 — that's ~60 multiplications
of 60³, fine if the inner loop is `i-k-j` and skips zero entries.
