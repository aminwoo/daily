#include <bits/stdc++.h>
using namespace std;

vector<int> merge_sorted(const vector<int>& a, const vector<int>& b) {
  vector<int> merged;
  merged.reserve(a.size() + b.size());
  size_t left = 0;
  size_t right = 0;
  while (left < a.size() && right < b.size()) {
    if (b[right] < a[left]) {
      merged.push_back(b[right++]);
    } else {
      merged.push_back(a[left++]);
    }
  }
  merged.insert(merged.end(), a.begin() + left, a.end());
  merged.insert(merged.end(), b.begin() + right, b.end());
  return merged;
}

static long long count_and_sort(vector<int>& a, vector<int>& scratch, int first,
                                int last) {
  if (last - first <= 1) return 0;
  const int middle = first + (last - first) / 2;
  long long inversions = count_and_sort(a, scratch, first, middle) +
                         count_and_sort(a, scratch, middle, last);
  int left = first;
  int right = middle;
  int output = first;
  while (left < middle && right < last) {
    if (a[right] < a[left]) {
      inversions += middle - left;
      scratch[output++] = a[right++];
    } else
      scratch[output++] = a[left++];
  }
  while (left < middle) scratch[output++] = a[left++];
  while (right < last) scratch[output++] = a[right++];
  copy(scratch.begin() + first, scratch.begin() + last, a.begin() + first);
  return inversions;
}

long long count_inversions(vector<int> a) {
  vector<int> scratch(a.size());
  return count_and_sort(a, scratch, 0, static_cast<int>(a.size()));
}
