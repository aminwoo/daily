# 34 — Binary trees

```cpp
struct TreeNode {
    int val;
    TreeNode *left = nullptr, *right = nullptr;
    TreeNode(int v) : val(v) {}
};

vector<vector<int>> level_order(TreeNode* root);              // BFS, one vector per depth
bool      is_valid_bst(TreeNode* root);                       // strict: left < node < right for the whole subtree.  vals may be INT_MIN / INT_MAX
TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q);      // p and q are nodes of the tree (possibly p == q or one an ancestor of the other)
long long max_path_sum(TreeNode* root);                       // max sum over paths of >= 1 node (any node to any node through parents).  Tree is non-empty.
int       diameter(TreeNode* root);                           // number of edges on the longest path; 0 for one node
string    serialize(TreeNode* root);                          // any format you like ...
TreeNode* deserialize(const string& s);                       // ... as long as this inverts it exactly (structure + values)
```

The perf test uses a chain of 1e6 nodes, so O(n) with recursion is fine (the
stack limit is raised) but O(n · depth) is not. For `is_valid_bst`, pass
bounds down as `long long` or as optional pointers — `INT_MIN` is a real
value in the tests. `max_path_sum` and `diameter` share one shape: a post-
order that returns the best *downward* arm while updating a global best with
`left_arm + right_arm (+ val)`.
