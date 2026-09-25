# 09 — Z-function

```cpp
// z[i] = length of the longest common prefix of s and s.substr(i).  By convention z[0] = n.
vector<int> z_function(const string& s);

// number of (possibly overlapping) occurrences of pat in text, computed with a Z-function.
// text and pat may contain arbitrary bytes; pat is non-empty.
long long count_occurrences(const string& text, const string& pat);
```

Target: O(n) — maintain the [l, r) box of the rightmost Z-match found so far.
For occurrence counting, map bytes to integers and place a value outside
`0..255` between the pattern and text; no `char` is a universally safe
separator for arbitrary byte strings.
