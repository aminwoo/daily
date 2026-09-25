#include <bits/stdc++.h>
using namespace std;

long long mod_pow(long long base, long long exponent, long long modulus) {
  long long result = 1 % modulus;
  base %= modulus;
  while (exponent > 0) {
    if (exponent & 1) result = result * base % modulus;
    base = base * base % modulus;
    exponent >>= 1;
  }
  return result;
}

long long ext_gcd(long long a, long long b, long long& x, long long& y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  long long next_x;
  long long next_y;
  const long long gcd = ext_gcd(b, a % b, next_x, next_y);
  x = next_y;
  y = next_x - (a / b) * next_y;
  return gcd;
}

long long mod_inv(long long a, long long m) {
  long long x;
  long long y;
  const long long gcd = ext_gcd(a, m, x, y);
  if (gcd != 1) return -1;
  return ((x % m) + m) % m;
}

struct Binomial {
  long long modulus;
  vector<long long> factorial;
  vector<long long> inverse_factorial;

  Binomial(int n, long long p)
      : modulus(p), factorial(n + 1), inverse_factorial(n + 1) {
    factorial[0] = 1;
    for (int i = 1; i <= n; ++i) {
      factorial[i] = factorial[i - 1] * i % modulus;
    }
    inverse_factorial[n] = mod_pow(factorial[n], modulus - 2, modulus);
    for (int i = n; i > 0; --i) {
      inverse_factorial[i - 1] = inverse_factorial[i] * i % modulus;
    }
  }

  long long C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return factorial[n] * inverse_factorial[k] % modulus *
           inverse_factorial[n - k] % modulus;
  }
};
