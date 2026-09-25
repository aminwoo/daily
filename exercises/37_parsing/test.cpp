#include "harness.h"
#include SOLUTION

// random expression tree, printed with the minimum parentheses needed
// (plus some random redundant ones), value tracked alongside.
struct Expr {
  char op;  // 0 = literal
  long long val;
  std::unique_ptr<Expr> l, r;
};

static std::unique_ptr<Expr> gen(int depth) {
  using harness::rnd;
  auto e = std::make_unique<Expr>();
  if (depth == 0 || rnd(0, 3) == 0) {
    e->op = 0;
    e->val = rnd(0, 20);
    return e;
  }
  e->op = "+-*/"[rnd(0, 3)];
  e->l = gen(depth - 1);
  if (e->op == '/') {  // nonzero literal divisor keeps it simple & safe
    e->r = std::make_unique<Expr>();
    e->r->op = 0;
    e->r->val = rnd(1, 9);
  } else {
    e->r = gen(depth - 1);
  }
  long long a = e->l->val, b = e->r->val;
  e->val = e->op == '+'   ? a + b
           : e->op == '-' ? a - b
           : e->op == '*' ? a * b
                          : a / b;
  return e;
}

static int prec(char op) {
  return op == '+' || op == '-' ? 1 : op == '*' || op == '/' ? 2 : 3;
}

static std::string sp() { return harness::rnd(0, 2) == 0 ? " " : ""; }

static std::string print(const Expr& e) {
  if (!e.op) return sp() + std::to_string(e.val) + sp();
  std::string l = print(*e.l), r = print(*e.r);
  // left child needs parens if lower precedence
  bool lp = e.l->op && prec(e.l->op) < prec(e.op);
  // right child needs parens if lower or equal precedence (left-assoc; and
  // integer division doesn't associate: 20*(6/4) != 20*6/4)
  bool rp = e.r->op && prec(e.r->op) <= prec(e.op);
  if (lp || harness::rnd(0, 4) == 0) l = "(" + l + ")";
  if (rp || harness::rnd(0, 4) == 0) r = "(" + r + ")";
  return sp() + l + sp() + e.op + sp() + r + sp();
}

// random nested pattern with expected expansion
static std::pair<std::string, std::string> gen_decode(int depth) {
  using harness::rnd;
  std::string enc, dec;
  int parts = (int)rnd(0, 3);
  for (int i = 0; i < parts; ++i) {
    if (depth > 0 && rnd(0, 1)) {
      int k = (int)rnd(1, 3);
      auto [e, d] = gen_decode(depth - 1);
      enc += std::to_string(k) + "[" + e + "]";
      for (int j = 0; j < k; ++j) dec += d;
    } else {
      int n = (int)rnd(1, 3);
      for (int j = 0; j < n; ++j) {
        char c = (char)('a' + rnd(0, 2));
        enc += c;
        dec += c;
      }
    }
  }
  return {enc, dec};
}

TEST(basic) {
  CHECK_EQ(evaluate("1 + 1"), 2LL);
  CHECK_EQ(evaluate(" 2-1 + 2 "), 3LL);
  CHECK_EQ(evaluate("(1+(4+5+2)-3)+(6+8)"), 23LL);
  CHECK_EQ(evaluate("3+2*2"), 7LL);
  CHECK_EQ(evaluate(" 3/2 "), 1LL);
  CHECK_EQ(evaluate(" 3+5 / 2 "), 5LL);
  CHECK_EQ(evaluate("2*(5+5*2)/3+(6/2+8)"), 21LL);
  CHECK_EQ(evaluate("(2+6*3+5-(3*14/7+2)*5)+3"), -12LL);
  CHECK_EQ(evaluate("0"), 0LL);
  CHECK_EQ(evaluate("((((7))))"), 7LL);
  CHECK_EQ(evaluate("100-10-10"), 80LL);
  CHECK_EQ(evaluate("100/10/5"), 2LL);
  CHECK_EQ(evaluate("1-7/2"), -2LL);
  CHECK_EQ(evaluate("(1-7)/2"), -3LL);
  CHECK_EQ(evaluate("123456789*1000"), 123456789000LL);
  CHECK_EQ(decode_string("3[a]2[bc]"), "aaabcbc");
  CHECK_EQ(decode_string("3[a2[c]]"), "accaccacc");
  CHECK_EQ(decode_string("2[abc]3[cd]ef"), "abcabccdcdcdef");
  CHECK_EQ(decode_string("abc"), "abc");
  CHECK_EQ(decode_string(""), "");
  CHECK_EQ(decode_string("10[a]"), "aaaaaaaaaa");
  CHECK_EQ(decode_string("2[2[2[x]]]"), "xxxxxxxx");
}

TEST(random_vs_bruteforce) {
  for (int it = 0; it < 500; ++it) {
    auto e = gen(5);
    CHECK_EQ(evaluate(print(*e)), e->val);
    auto [enc, dec] = gen_decode(4);
    CHECK_EQ(decode_string(enc), dec);
  }
}

TEST(perf) {
  std::string s = "1";
  for (int i = 0; i < 300000; ++i) s += "+1*1";
  CHECK_EQ(evaluate(s), 300001LL);
  std::string deep(100000, '('), close(100000, ')');
  CHECK_EQ(evaluate(deep + "5" + close), 5LL);
  std::string sub = "1000000";
  for (int i = 0; i < 200000; ++i) sub += "-1";
  CHECK_EQ(evaluate(sub), 800000LL);
  std::string enc = "a";
  for (int i = 0; i < 23; ++i) enc = "2[" + enc + "]";  // 2^23 chars
  CHECK_EQ(decode_string(enc).size(), (size_t)1 << 23);
  std::string flat(1000000, 'z');
  CHECK_EQ(decode_string(flat), flat);
  std::string many;  // 1e5 small groups, output 3e5
  for (int i = 0; i < 100000; ++i) many += "3[q]";
  CHECK_EQ(decode_string(many).size(), (size_t)300000);
}
