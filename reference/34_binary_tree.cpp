#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left = nullptr, *right = nullptr;

  TreeNode(int v) : val(v) {}
};

vector<vector<int>> level_order(TreeNode* root) {
  vector<vector<int>> out;
  if (!root) return out;
  queue<TreeNode*> q;
  q.push(root);
  while (!q.empty()) {
    int sz = q.size();
    out.emplace_back();
    while (sz--) {
      TreeNode* t = q.front();
      q.pop();
      out.back().push_back(t->val);
      if (t->left) q.push(t->left);
      if (t->right) q.push(t->right);
    }
  }
  return out;
}

static bool is_valid_bst(TreeNode* node, long long lower, long long upper) {
  if (!node) return true;
  if (node->val <= lower || node->val >= upper) return false;
  return is_valid_bst(node->left, lower, node->val) &&
         is_valid_bst(node->right, node->val, upper);
}

bool is_valid_bst(TreeNode* root) {
  return is_valid_bst(root, LLONG_MIN, LLONG_MAX);
}

TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q) {
  if (!root || root == p || root == q) return root;
  TreeNode* l = lca(root->left, p, q);
  TreeNode* r = lca(root->right, p, q);
  if (l && r) return root;
  return l ? l : r;
}

static long long best_downward_path(TreeNode* node, long long& best) {
  if (!node) return 0;
  const long long left = max(0LL, best_downward_path(node->left, best));
  const long long right = max(0LL, best_downward_path(node->right, best));
  best = max(best, left + right + node->val);
  return max(left, right) + node->val;
}

long long max_path_sum(TreeNode* root) {
  long long best = LLONG_MIN;
  best_downward_path(root, best);
  return best;
}

static int subtree_height(TreeNode* node, int& best) {
  if (!node) return 0;
  const int left = subtree_height(node->left, best);
  const int right = subtree_height(node->right, best);
  best = max(best, left + right);
  return max(left, right) + 1;
}

int diameter(TreeNode* root) {
  int best = 0;
  subtree_height(root, best);
  return best;
}

static void ser(TreeNode* t, string& s) {
  if (!t) {
    s += "# ";
    return;
  }
  s += to_string(t->val) + ' ';
  ser(t->left, s);
  ser(t->right, s);
}

string serialize(TreeNode* root) {
  string s;
  ser(root, s);
  return s;
}

static TreeNode* de(const string& s, size_t& i) {
  if (s[i] == '#') {
    i += 2;
    return nullptr;
  }
  size_t e = s.find(' ', i);
  TreeNode* t = new TreeNode(stoi(s.substr(i, e - i)));
  i = e + 1;
  t->left = de(s, i);
  t->right = de(s, i);
  return t;
}

TreeNode* deserialize(const string& s) {
  size_t i = 0;
  return de(s, i);
}
