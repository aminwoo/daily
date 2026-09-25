# 37 — Parsing: expression evaluator & nested decoding

```cpp
// Evaluate an arithmetic expression: non-negative integer literals, + - * /,
// parentheses, arbitrary spaces.  Usual precedence, left-associative.
// Division is integer division truncating toward zero (C++ semantics); the
// tests never divide by zero.  Intermediate values fit in long long.
long long evaluate(const string& s);

// Expand "k[str]" nesting: "3[a2[c]]" -> "accaccacc", "2[ab]x" -> "ababx".
// Letters are a-z, k >= 1, nesting is arbitrary; output fits in memory.
string decode_string(const string& s);
```

Inputs are valid encodings/expressions; an expression is non-empty after
removing spaces. Unary signs are not part of the grammar (negative results
still arise from subtraction).

`evaluate` is the "basic calculator III" family. Either recursive descent
(`expr → term (('+'|'-') term)*`, `term → factor (('*'|'/') factor)*`,
`factor → number | '(' expr ')'`) or the shunting-yard two-stack approach —
you should be able to write one of them from memory in ten minutes.

Both must be linear in input plus output size: the perf test has a 1e6-char
expression, parentheses nested 1e5 deep, and a decode whose output is 1e7
characters. Beware of `s.substr` in a loop.
