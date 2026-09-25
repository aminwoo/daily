# daily — C++ algorithm drills

38 self-contained exercises (DSU, Fenwick, segment trees, Dijkstra, KMP, LCA,
SCC, max flow, … plus an interview-prep block: sliding window, intervals,
heaps, LRU/LFU, linked lists, binary trees, backtracking, bits, parsing,
design questions), each with a problem statement, a skeleton to fill in, and
a test suite with brute-force cross-checks and a perf test that times out on
the wrong complexity. Tests run under `-fsanitize=address,undefined`, so
out-of-bounds indexing and signed overflow are caught too.

## Setup

Needs bash, `g++` with C++20 and AddressSanitizer (GCC 10+; set `CXX=clang++`
to use clang), and Neovim for the editor commands.

```sh
git clone https://github.com/aminwoo/daily.git && cd daily
./daily install       # symlinks daily into ~/.local/bin (or ./daily install <dir>)
daily next
```

Your solutions go in `work/` and your progress in `.progress`. Both are
gitignored, so `git pull` never touches them.

## Daily loop

```sh
./daily next          # opens the first exercise you haven't passed, in nvim
#   left pane: work/<ex>.cpp   right pane: the problem statement
#   <leader>t  → save + run the tests in a split
#   <leader>m  → same via :make; compile errors go to the quickfix list (:cn / :cp)
./daily test <ex>     # run the tests from a shell instead
./daily sample <ex>   # run your own hand-written cases from work/<ex>.sample.cpp
#   <leader>r  → same from nvim; the file is created on first use, never records a pass
#   <leader>n / :DailyNext  → switch to the next unpassed exercise without leaving nvim
#   :DailyOpen 7 / :DailyOpen kmp  → jump to a specific exercise
./daily list          # progress
```

After `./daily install`, `daily` works from any directory, and `daily <ex>` is shorthand for `daily open <ex>`: `daily trie`, `daily 12`, `daily kmp`.

`./daily today` picks one exercise by the date (it cycles through all of them, so
it is a spaced-repetition re-run once you've passed everything).
`./daily reset <ex>` restores the skeleton so you can redo it from scratch;
`./daily reset all` does that for every exercise and clears your progress.

`<ex>` can be a number (`3`), a name (`segment_tree`), the full id, or a path to
the work file — so `:!./daily test %` works inside nvim too.

## Layout

```
daily                 the CLI
harness.h             TEST / CHECK / CHECK_EQ / REQUIRE macros, timer, per-test timeout
exercises/NN_name/    README.md · skeleton.cpp · test.cpp
work/NN_name.cpp      your solutions (created from the skeleton on first open)
work/NN_name.sample.cpp  optional scratch tests for debugging (./daily sample <ex>)
reference/NN_name.cpp reference solutions — ./daily ref <ex> runs the tests against one
.progress             which exercises you've passed and when
```

## Rules of the game

* Your file is `#include`d by `test.cpp` — keep the signatures from the README,
  don't define `main`, and `using namespace std;` is fine.
* Each test has a 10 s wall-clock limit (`harness::timeout_s`); a timeout means
  your complexity is wrong, not that the machine is slow.
* The stack limit is raised before running, so recursive DFS on 3e5 nodes is OK.

## Adding an exercise

`./daily new 39_topic` scaffolds `exercises/39_topic/{README.md,skeleton.cpp,test.cpp}`
and `reference/39_topic.cpp`. Write the tests to compare against a brute force
(see any existing `test.cpp`), get them passing with the reference, then drill.

## Exercises

| # | topic | # | topic |
|---|---|---|---|
| 01 | DSU | 15 | monotonic stack / deque |
| 02 | Fenwick tree | 16 | Floyd–Warshall |
| 03 | segment tree (point set, range min) | 17 | Bellman–Ford + negative cycles |
| 04 | Dijkstra | 18 | LIS in O(n log n) |
| 05 | BFS on a grid | 19 | knapsack / coin change / LCS |
| 06 | topological sort | 20 | LCA via binary lifting |
| 07 | binary search (predicate, lower_bound, isqrt) | 21 | strongly connected components |
| 08 | KMP | 22 | bridges |
| 09 | Z-function | 23 | lazy segment tree (range add, range sum) |
| 10 | sieve + smallest prime factor | 24 | rolling hash |
| 11 | modpow / ext_gcd / inverse / binomials | 25 | matrix exponentiation |
| 12 | trie | 26 | merge sort + inversions |
| 13 | Kruskal | 27 | max flow (Dinic) |
| 14 | sparse table | 28 | convex hull |

Interview prep (29–38): the patterns that show up in a 45-minute round.

| # | topic | # | topic |
|---|---|---|---|
| 29 | sliding window, Kadane, prefix-sum hashing | 34 | binary trees (level order, BST check, LCA, max path, diameter, serialize) |
| 30 | intervals (merge, insert, meeting rooms, activity selection) | 35 | backtracking (subsets, permutations, combination sum, N-queens, parens) |
| 31 | heaps (k-th largest, running median, k-way merge, top-k frequent) | 36 | bit tricks (XOR singles, popcount table, reverse bits, max XOR pair) |
| 32 | LRU and LFU caches | 37 | expression evaluator, nested string decoding |
| 33 | linked lists (reverse, merge, cycle, nth-from-end, palindrome) | 38 | MinStack, TimeMap, RandomizedSet |
