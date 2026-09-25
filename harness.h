// harness.h — tiny test framework shared by every exercise's test.cpp.
// test.cpp does:   #include "harness.h"   then   #include SOLUTION
// (SOLUTION is passed by ./daily as -DSOLUTION="path/to/your.cpp")
#pragma once
#include <bits/stdc++.h>
#include <unistd.h>

#include <csignal>

namespace harness {

struct Test {
  const char* name;
  void (*fn)();
};

inline std::vector<Test>& tests() {
  static std::vector<Test> t;
  return t;
}

struct Reg {
  Reg(const char* n, void (*f)()) { tests().push_back({n, f}); }
};

inline int checks = 0, fails = 0, fails_this = 0;
inline const int MAX_REPORT = 5;
inline unsigned timeout_s = 10;  // per-test wall clock limit
inline const char* current = "";
inline std::vector<std::string>
    pending;  // failure messages of the running test

inline std::mt19937_64 rng(20240911);

inline long long rnd(long long lo, long long hi) {
  return std::uniform_int_distribution<long long>(lo, hi)(rng);
}

template <class T>
concept Streamable = requires(std::ostream& o, T v) { o << v; };
template <class T>
std::string show(const T& v);
template <class A, class B>
std::string show(const std::pair<A, B>& p);
template <class T>
std::string show(const std::vector<T>& v);

template <class T>
std::string show(const T& v) {
  if constexpr (Streamable<T>) {
    std::ostringstream o;
    o << v;
    return o.str();
  } else
    return "<?>";
}

template <class A, class B>
std::string show(const std::pair<A, B>& p) {
  return "(" + show(p.first) + ", " + show(p.second) + ")";
}

template <class T>
std::string show(const std::vector<T>& v) {
  std::string s = "[";
  for (size_t i = 0; i < v.size() && i < 20; ++i)
    s += (i ? ", " : "") + show(v[i]);
  if (v.size() > 20) s += ", ... (" + std::to_string(v.size()) + " items)";
  return s + "]";
}

inline void fail(const char* file, int line, const std::string& msg) {
  ++fails;
  ++fails_this;
  if (fails_this <= MAX_REPORT)
    pending.push_back(std::string("      ") + file + ":" +
                      std::to_string(line) + "  " + msg);
  else if (fails_this == MAX_REPORT + 1)
    pending.push_back("      ... more failures in this test suppressed");
}

inline void on_alarm(int) {
  std::printf("TIMEOUT (> %us)\n", timeout_s);
  for (auto& m : pending) std::printf("%s\n", m.c_str());
  std::printf(
      "\n  aborted: '%s' is too slow (or stuck) — check your complexity\n",
      current);
  std::fflush(stdout);
  _exit(2);
}

inline int run_all() {
  std::signal(SIGALRM, on_alarm);
  int failed_tests = 0;
  for (auto& t : tests()) {
    current = t.name;
    fails_this = 0;
    pending.clear();
    std::printf("  %-42s", t.name);
    std::fflush(stdout);
    auto t0 = std::chrono::steady_clock::now();
    alarm(timeout_s);
    t.fn();
    alarm(0);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - t0)
                  .count();
    if (fails_this) {
      ++failed_tests;
      std::printf("FAIL  (%lld ms)\n", (long long)ms);
      for (auto& m : pending) std::printf("%s\n", m.c_str());
    } else {
      std::printf("ok    (%lld ms)\n", (long long)ms);
    }
    std::fflush(stdout);
  }
  std::printf("\n  %zu tests, %d checks, %d failing test%s\n", tests().size(),
              checks, failed_tests, failed_tests == 1 ? "" : "s");
  return failed_tests ? 1 : 0;
}

}  // namespace harness

#define TEST(name)                              \
  static void name();                           \
  static harness::Reg name##_reg(#name, &name); \
  static void name()

#define CHECK(c)                                     \
  do {                                               \
    ++harness::checks;                               \
    if (!(c)) harness::fail(__FILE__, __LINE__, #c); \
  } while (0)
#define CHECK_EQ(a, b)                                                         \
  do {                                                                         \
    ++harness::checks;                                                         \
    auto&& _a = (a);                                                           \
    auto&& _b = (b);                                                           \
    if (!(_a == _b))                                                           \
      harness::fail(__FILE__, __LINE__,                                        \
                    std::string(#a " == " #b "   got: ") + harness::show(_a) + \
                        "   expected: " + harness::show(_b));                  \
  } while (0)
// REQUIRE* abort the current test on failure (use when later checks would be
// meaningless / crash)
#define REQUIRE(c)                           \
  do {                                       \
    ++harness::checks;                       \
    if (!(c)) {                              \
      harness::fail(__FILE__, __LINE__, #c); \
      return;                                \
    }                                        \
  } while (0)
#define REQUIRE_EQ(a, b)                                                       \
  do {                                                                         \
    ++harness::checks;                                                         \
    auto&& _a = (a);                                                           \
    auto&& _b = (b);                                                           \
    if (!(_a == _b)) {                                                         \
      harness::fail(__FILE__, __LINE__,                                        \
                    std::string(#a " == " #b "   got: ") + harness::show(_a) + \
                        "   expected: " + harness::show(_b));                  \
      return;                                                                  \
    }                                                                          \
  } while (0)

int main() { return harness::run_all(); }
