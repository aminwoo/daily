#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX / 4;

vector<vector<long long>> floyd_warshall(
    int n, const vector<tuple<int, int, long long>>& edges) {
  return vector<vector<long long>>(n, vector<long long>(n, INF));
}
