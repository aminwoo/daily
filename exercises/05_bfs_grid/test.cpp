#include "harness.h"
#include SOLUTION

// brute force: relax distances repeatedly until nothing changes (Bellman-Ford
// style)
static int slow(const std::vector<std::string>& g, int sr, int sc, int tr,
                int tc) {
  int R = g.size(), C = g[0].size(), INF = 1e9;
  std::vector<std::vector<int>> d(R, std::vector<int>(C, INF));
  d[sr][sc] = 0;
  bool ch = true;
  while (ch) {
    ch = false;
    for (int r = 0; r < R; ++r)
      for (int c = 0; c < C; ++c) {
        if (g[r][c] == '#' || d[r][c] == INF) continue;
        int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
        for (int k = 0; k < 4; ++k) {
          int nr = r + dr[k], nc = c + dc[k];
          if (nr < 0 || nc < 0 || nr >= R || nc >= C || g[nr][nc] == '#')
            continue;
          if (d[r][c] + 1 < d[nr][nc]) {
            d[nr][nc] = d[r][c] + 1;
            ch = true;
          }
        }
      }
  }
  return d[tr][tc] == INF ? -1 : d[tr][tc];
}

TEST(basic) {
  std::vector<std::string> g = {
      "....#",
      ".##.#",
      "....#",
      "#.#..",
  };
  CHECK_EQ(grid_bfs(g, 0, 0, 0, 0), 0);
  CHECK_EQ(grid_bfs(g, 0, 0, 0, 3), 3);
  CHECK_EQ(grid_bfs(g, 0, 0, 2, 3), 5);
  CHECK_EQ(grid_bfs(g, 0, 0, 3, 4), 7);
  CHECK_EQ(grid_bfs(g, 3, 1, 0, 3), 5);
  std::vector<std::string> g2 = {".#.", "###", ".#."};
  CHECK_EQ(grid_bfs(g2, 0, 0, 2, 2), -1);
  CHECK_EQ(grid_bfs({"."}, 0, 0, 0, 0), 0);
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int iter = 0; iter < 60; ++iter) {
    int R = (int)rnd(1, 12), C = (int)rnd(1, 12);
    std::vector<std::string> g(R, std::string(C, '.'));
    for (auto& row : g)
      for (auto& ch : row)
        if (rnd(0, 99) < 35) ch = '#';
    int sr = (int)rnd(0, R - 1), sc = (int)rnd(0, C - 1),
        tr = (int)rnd(0, R - 1), tc = (int)rnd(0, C - 1);
    g[sr][sc] = '.';
    g[tr][tc] = '.';
    CHECK_EQ(grid_bfs(g, sr, sc, tr, tc), slow(g, sr, sc, tr, tc));
  }
}

TEST(perf_open) {
  int N = 1500;
  std::vector<std::string> g(N, std::string(N, '.'));
  CHECK_EQ(grid_bfs(g, 0, 0, N - 1, N - 1), 2 * (N - 1));
}

// walking a snake: every second row is open, connected alternately at the
// right/left end
static int slow_snake_expected(int N, int last) {
  // from (0,0): traverse each open row fully (N-1 steps), then 2 steps down
  // through the gap
  return (last / 2) * (N - 1) + last;
}

TEST(perf_snake) {
  int N = 1500;
  std::vector<std::string> g(N, std::string(N, '.'));
  for (int r = 1; r < N; r += 2) {
    g[r] = std::string(N, '#');
    if ((r / 2) % 2 == 0)
      g[r][N - 1] = '.';
    else
      g[r][0] = '.';
  }
  int last = N - 1;
  if (last % 2) --last;
  int got = grid_bfs(g, 0, 0, last, ((last / 2) % 2 == 0) ? 0 : N - 1);
  CHECK_EQ(got, slow_snake_expected(N, last));
}
