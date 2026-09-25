# 24 — Polynomial rolling hash

```cpp
struct RollingHash {
    RollingHash(const string& s);              // O(n) prefix hashes + powers
    unsigned long long get(int l, int r);      // position-independent hash of s[l..r), 0 <= l <= r <= n
};
```

Requirements the tests enforce:

* a **random base** (seed from `std::random_device` or the clock) and a big
  modulus — `2^61 - 1` with `__int128` multiplication is the standard choice.
  A single ~1e9 modulus will fail the birthday-paradox test (2e5 substrings).
* **no "natural overflow" mod 2^64**: the tests hash Thue–Morse strings, which
  collide for *any* base under mod 2^64.
* hashes of different objects must be comparable: `get(l, r)` must depend only
  on the characters, not on `l` (so two `RollingHash` objects agree).

Equal substrings must have equal hashes. Unequal substrings can collide in
principle; the randomized 61-bit construction makes that probability
negligible rather than claiming an impossible collision-free guarantee.
