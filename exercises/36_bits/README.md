# 36 — Bit manipulation

```cpp
int       single_number(const vector<int>& a);      // every value appears twice except one
int       single_number_thrice(const vector<int>& a);  // every value appears 3 times except one (which appears once).  O(1) extra, no hash map.
pair<int,int> two_singles(const vector<int>& a);   // exactly two values appear once, the rest twice; return (smaller, larger).  O(1) extra.
vector<int> count_bits(int n);                     // n >= 0; popcount of 0..n, O(n) from earlier entries
uint32_t  reverse_bits(uint32_t x);                // bit 0 <-> bit 31, etc.
int       max_xor_pair(const vector<int>& a);      // max a[i] ^ a[j] over i < j.  n >= 2, 0 <= a[i] < 2^30.  O(n log C)
```

Values may be negative for the first three — XOR on `int` is fine, but
`two_singles` needs the lowest set bit of the combined XOR, so compute it as
`unsigned` (`x & -x` on a negative `int` is well-defined but read it back
carefully). `max_xor_pair` is either a binary trie over the 30 bits, or the
greedy "build the answer bit by bit with a hash set of prefixes" trick.
