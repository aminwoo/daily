#include <bits/stdc++.h>
using namespace std;

struct RollingHash {
  static constexpr unsigned long long MOD = (1ULL << 61) - 1;

  static unsigned long long mulmod(unsigned long long a, unsigned long long b) {
    __uint128_t c = (__uint128_t)a * b;
    unsigned long long r =
        (unsigned long long)(c >> 61) + (unsigned long long)(c & MOD);
    return r >= MOD ? r - MOD : r;
  }

  static unsigned long long base() {
    static unsigned long long b =
        mt19937_64(random_device{}())() % (MOD - 1000) + 500;
    return b;
  }

  vector<unsigned long long> prefix_hash, powers;

  explicit RollingHash(const string& s)
      : prefix_hash(s.size() + 1, 0), powers(s.size() + 1, 1) {
    for (size_t i = 0; i < s.size(); ++i) {
      prefix_hash[i + 1] =
          (mulmod(prefix_hash[i], base()) + (unsigned char)s[i]) % MOD;
      powers[i + 1] = mulmod(powers[i], base());
    }
  }

  unsigned long long get(int l, int r) const {
    unsigned long long hash =
        prefix_hash[r] + MOD - mulmod(prefix_hash[l], powers[r - l]);
    return hash >= MOD ? hash - MOD : hash;
  }
};
