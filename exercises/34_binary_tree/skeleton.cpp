#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left = nullptr, *right = nullptr;

  TreeNode(int v) : val(v) {}
};

vector<vector<int>> level_order(TreeNode* root) { return {}; }

bool is_valid_bst(TreeNode* root) { return false; }

TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q) { return root; }

long long max_path_sum(TreeNode* root) { return 0; }

int diameter(TreeNode* root) { return 0; }

string serialize(TreeNode* root) { return ""; }

TreeNode* deserialize(const string& s) { return nullptr; }
