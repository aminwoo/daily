#include <bits/stdc++.h>
using namespace std;

int grid_bfs(const vector<string>& grid, int sr, int sc, int tr, int tc) {
  const int rows = static_cast<int>(grid.size());
  const int columns = static_cast<int>(grid[0].size());
  vector<int> distance(rows * columns, -1);
  vector<int> queue(rows * columns);
  int head = 0;
  int tail = 0;
  distance[sr * columns + sc] = 0;
  queue[tail++] = sr * columns + sc;
  constexpr int row_delta[] = {1, -1, 0, 0};
  constexpr int column_delta[] = {0, 0, 1, -1};
  while (head < tail) {
    const int current = queue[head++];
    const int row = current / columns;
    const int column = current % columns;
    if (row == tr && column == tc) return distance[current];
    for (int direction = 0; direction < 4; ++direction) {
      const int next_row = row + row_delta[direction];
      const int next_column = column + column_delta[direction];
      if (next_row < 0 || next_column < 0 || next_row >= rows ||
          next_column >= columns || grid[next_row][next_column] == '#') {
        continue;
      }
      const int next = next_row * columns + next_column;
      if (distance[next] != -1) continue;
      distance[next] = distance[current] + 1;
      queue[tail++] = next;
    }
  }
  return -1;
}
