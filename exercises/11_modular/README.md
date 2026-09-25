# 11 — Modular arithmetic toolkit

```cpp
// b^e mod m.   0 <= b < m,  e >= 0,  1 <= m <= 2e9 (so products fit in long long)
long long mod_pow(long long b, long long e, long long m);

// extended Euclid: returns g = gcd(a, b) and sets x, y with a*x + b*y = g.
// 0 <= a,b <= 1e12, not both 0 (coefficients and intermediates fit in long long).
long long ext_gcd(long long a, long long b, long long& x, long long& y);

// inverse of a modulo m in [0, m), or -1 if gcd(a, m) != 1.   1 <= a < m.  (use ext_gcd)
long long mod_inv(long long a, long long m);

// binomial coefficients modulo a prime p > n, via precomputed factorials and inverse factorials.
struct Binomial {
    Binomial(int n, long long p);      // precompute for 0..n
    long long C(int n, int k);         // 0 if k < 0 or k > n
};
```

Do `mod_pow` iteratively (square-and-multiply). `mod_inv` for prime modulus
could use Fermat, but here it must work for any coprime modulus, so use
`ext_gcd` and normalise the result into `[0, m)`.
