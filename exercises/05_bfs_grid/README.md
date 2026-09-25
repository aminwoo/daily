# 05 — BFS on a grid

Shortest path in a non-empty rectangular grid with 4-directional moves.
`'#'` is a wall, anything else is walkable. Start and target are valid,
walkable cells.

```cpp
// number of steps from (sr,sc) to (tr,tc); -1 if unreachable
int grid_bfs(const vector<string>& grid, int sr, int sc, int tr, int tc);
```

Target: O(rows * cols). The perf tests use a 1500x1500 grid — use a flat
`vector<int>` for distances and a `std::queue` (or a hand-rolled ring buffer),
not `std::set`/`std::map`.
