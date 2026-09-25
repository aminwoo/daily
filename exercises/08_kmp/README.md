# 08 — KMP / prefix function

```cpp
// pi[i] = length of the longest proper prefix of s[0..i] that is also a suffix of it.  pi[0] = 0.
vector<int> prefix_function(const string& s);

// all start indices i (ascending) with text.substr(i, pat.size()) == pat.  pat is non-empty.
// Do NOT build text + '#' + pat if you can avoid it: run the automaton over text directly.
vector<int> find_occurrences(const string& text, const string& pat);
```

Target: O(|text| + |pat|). The perf test is the classic worst case for naive
matching (`aaaa...a` in `aaaa...a`).
