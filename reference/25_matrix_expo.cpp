#include <bits/stdc++.h>
using namespace std;
using Mat = vector<vector<long long>>;

Mat mat_mul(const Mat& A, const Mat& B, long long mod) {
  const int rows = static_cast<int>(A.size());
  const int inner = static_cast<int>(B.size());
  const int columns = static_cast<int>(B[0].size());
  Mat result(rows, vector<long long>(columns, 0));
  for (int i = 0; i < rows; ++i) {
    for (int k = 0; k < inner; ++k) {
      if (A[i][k] == 0) continue;
      for (int j = 0; j < columns; ++j) {
        result[i][j] = (result[i][j] + A[i][k] * B[k][j]) % mod;
      }
    }
  }
  return result;
}

Mat mat_pow(Mat A, long long e, long long mod) {
  const int n = static_cast<int>(A.size());
  Mat result(n, vector<long long>(n, 0));
  for (int i = 0; i < n; ++i) result[i][i] = 1 % mod;
  while (e > 0) {
    if (e & 1) result = mat_mul(result, A, mod);
    A = mat_mul(A, A, mod);
    e >>= 1;
  }
  return result;
}

long long fib(long long n, long long mod) {
  return mat_pow({{1, 1}, {1, 0}}, n, mod)[0][1];
}

long long count_walks(const vector<vector<int>>& adj, int s, int t, long long k,
                      long long mod) {
  int n = adj.size();
  Mat matrix(n, vector<long long>(n));
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) matrix[i][j] = adj[i][j] % mod;
  return mat_pow(matrix, k, mod)[s][t];
}
