#include <bits/stdc++.h>
using namespace std;
const long long INF = LLONG_MAX / 4;

vector<vector<long long>> floyd_warshall(
    int n, const vector<tuple<int, int, long long>>& edges) {
  vector<vector<long long>> d(n, vector<long long>(n, INF));
  for (int i = 0; i < n; ++i) d[i][i] = 0;
  for (auto [u, v, w] : edges) d[u][v] = min(d[u][v], w);
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i) {
      if (d[i][k] >= INF) continue;
      for (int j = 0; j < n; ++j)
        if (d[k][j] < INF) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    }
  return d;
}
