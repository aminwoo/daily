#include <bits/stdc++.h>
using namespace std;

vector<int> primes_up_to(int n) {
  vector<int> primes;
  if (n < 2) return primes;
  vector<char> is_composite(n + 1, false);
  for (long long i = 2; i <= n; ++i) {
    if (is_composite[i]) continue;
    primes.push_back(static_cast<int>(i));
    for (long long multiple = i * i; multiple <= n; multiple += i) {
      is_composite[multiple] = true;
    }
  }
  return primes;
}

vector<int> spf_up_to(int n) {
  vector<int> spf(n + 1, 0);
  for (long long i = 2; i <= n; ++i) {
    if (spf[i] != 0) continue;
    for (long long multiple = i; multiple <= n; multiple += i) {
      if (spf[multiple] == 0) spf[multiple] = static_cast<int>(i);
    }
  }
  return spf;
}

vector<pair<int, int>> factorize(int x, const vector<int>& spf) {
  vector<pair<int, int>> factors;
  while (x > 1) {
    const int prime = spf[x];
    int exponent = 0;
    while (x % prime == 0) {
      x /= prime;
      ++exponent;
    }
    factors.push_back({prime, exponent});
  }
  return factors;
}
