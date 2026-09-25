#include <bits/stdc++.h>
using namespace std;

using Iv = pair<long long, long long>;

vector<Iv> merge_intervals(vector<Iv> v) {
  sort(v.begin(), v.end());
  vector<Iv> result;
  for (auto [start, end] : v) {
    if (!result.empty() && result.back().second >= start) {
      result.back().second = max(result.back().second, end);
    } else {
      result.push_back({start, end});
    }
  }
  return result;
}

vector<Iv> insert_interval(const vector<Iv>& v, Iv x) {
  vector<Iv> result;
  size_t i = 0;
  while (i < v.size() && v[i].second < x.first) result.push_back(v[i++]);
  while (i < v.size() && v[i].first <= x.second) {
    x.first = min(x.first, v[i].first);
    x.second = max(x.second, v[i].second);
    ++i;
  }
  result.push_back(x);
  result.insert(result.end(), v.begin() + i, v.end());
  return result;
}

int min_rooms(const vector<Iv>& v) {
  vector<long long> starts, ends;
  for (auto [start, end] : v) {
    starts.push_back(start);
    ends.push_back(end);
  }
  sort(starts.begin(), starts.end());
  sort(ends.begin(), ends.end());
  int rooms = 0;
  int best = 0;
  size_t next_end = 0;
  for (long long start : starts) {
    while (next_end < ends.size() && ends[next_end] <= start) {
      ++next_end;
      --rooms;
    }
    best = max(best, ++rooms);
  }
  return best;
}

int max_non_overlapping(vector<Iv> v) {
  sort(v.begin(), v.end(),
       [](const Iv& a, const Iv& b) { return a.second < b.second; });
  int count = 0;
  long long last_end = LLONG_MIN;
  for (auto [start, end] : v) {
    if (start >= last_end) {
      ++count;
      last_end = end;
    }
  }
  return count;
}
