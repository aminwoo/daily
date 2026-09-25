# 10 — Sieve of Eratosthenes & smallest prime factors

```cpp
// all primes <= n, ascending  (n >= 0)
vector<int> primes_up_to(int n);

// spf[i] = smallest prime factor of i, for 0 <= i <= n.  spf[0] = spf[1] = 0.
vector<int> spf_up_to(int n);

// prime factorisation of x (1 <= x < spf.size()) as ascending (prime, exponent) pairs, using spf.
vector<pair<int,int>> factorize(int x, const vector<int>& spf);
```

Target: O(n log log n) for the sieve; factorisation in O(log x). Use
`vector<char>`/`vector<bool>` and only start crossing off at i*i (as
`long long`!). Perf test: n = 2e7.
