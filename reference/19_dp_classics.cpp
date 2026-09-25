#include <bits/stdc++.h>
using namespace std;

long long knapsack01(const vector<int>& w, const vector<long long>& v, int W) {
  vector<long long> dp(W + 1, 0);
  for (size_t i = 0; i < w.size(); ++i) {
    for (int capacity = W; capacity >= w[i]; --capacity) {
      dp[capacity] = max(dp[capacity], dp[capacity - w[i]] + v[i]);
    }
  }
  return dp[W];
}

long long count_ways(const vector<int>& coins, int target) {
  vector<long long> dp(target + 1, 0);
  dp[0] = 1;
  for (int coin : coins) {
    for (int amount = coin; amount <= target; ++amount) {
      dp[amount] += dp[amount - coin];
    }
  }
  return dp[target];
}

int min_coins(const vector<int>& coins, int target) {
  const int INF = INT_MAX / 2;
  vector<int> dp(target + 1, INF);
  dp[0] = 0;
  for (int amount = 1; amount <= target; ++amount) {
    for (int coin : coins) {
      if (coin <= amount) dp[amount] = min(dp[amount], dp[amount - coin] + 1);
    }
  }
  return dp[target] >= INF ? -1 : dp[target];
}

int lcs(const string& a, const string& b) {
  const int n = static_cast<int>(a.size());
  const int m = static_cast<int>(b.size());
  vector<int> previous(m + 1, 0);
  vector<int> current(m + 1, 0);
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j)
      current[j] = a[i - 1] == b[j - 1] ? previous[j - 1] + 1
                                        : max(previous[j], current[j - 1]);
    swap(previous, current);
  }
  return previous[m];
}
