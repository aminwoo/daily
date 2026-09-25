#include <bits/stdc++.h>
using namespace std;

vector<int> scc(int n, const vector<vector<int>>& adj) {
  vector<int> discovery_time(n, -1), low_link(n), component(n, -1);
  vector<int> component_stack;
  vector<char> on_stack(n, false);
  int timer = 0, component_count = 0;
  // Tarjan, iterative to be safe on deep graphs
  vector<pair<int, int>> call_stack;  // (vertex, next edge index)
  for (int start = 0; start < n; ++start) {
    if (discovery_time[start] != -1) continue;
    call_stack.push_back({start, 0});
    discovery_time[start] = low_link[start] = timer++;
    component_stack.push_back(start);
    on_stack[start] = true;
    while (!call_stack.empty()) {
      auto& [u, i] = call_stack.back();
      if (i < (int)adj[u].size()) {
        int v = adj[u][i++];
        if (discovery_time[v] == -1) {
          discovery_time[v] = low_link[v] = timer++;
          component_stack.push_back(v);
          on_stack[v] = true;
          call_stack.push_back({v, 0});
        } else if (on_stack[v]) {
          low_link[u] = min(low_link[u], discovery_time[v]);
        }
      } else {
        if (low_link[u] == discovery_time[u]) {
          while (true) {
            int v = component_stack.back();
            component_stack.pop_back();
            on_stack[v] = false;
            component[v] = component_count;
            if (v == u) break;
          }
          ++component_count;
        }
        int completed = u;
        call_stack.pop_back();
        if (!call_stack.empty()) {
          int parent = call_stack.back().first;
          low_link[parent] = min(low_link[parent], low_link[completed]);
        }
      }
    }
  }
  for (int& id : component) {
    id = component_count - 1 - id;  // Tarjan emits reverse topological order.
  }
  return component;
}
