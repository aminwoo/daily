#include "harness.h"
#include SOLUTION

// tree with parent pointers kept on the side, for brute forces
struct T {
  std::vector<TreeNode*> nodes;
  std::map<TreeNode*, TreeNode*> parent;
  TreeNode* root = nullptr;
};

static T rand_tree(int n, int lo, int hi) {
  T t;
  using harness::rnd;
  for (int i = 0; i < n; ++i) {
    auto* nd = new TreeNode((int)rnd(lo, hi));
    if (!t.root) {
      t.root = nd;
    } else {
      for (;;) {
        TreeNode* p = t.nodes[rnd(0, t.nodes.size() - 1)];
        TreeNode*& slot = rnd(0, 1) ? p->left : p->right;
        if (!slot) {
          slot = nd;
          t.parent[nd] = p;
          break;
        }
      }
    }
    t.nodes.push_back(nd);
  }
  return t;
}

// BST via sorted values + random-shaped tree, optionally corrupted
static T rand_bst(int n) {
  T t = rand_tree(n, 0, 0);
  std::vector<int> vals;
  std::set<int> seen;
  while ((int)vals.size() < n) {
    int v = (int)harness::rnd(INT_MIN, INT_MAX);
    if (seen.insert(v).second) vals.push_back(v);
  }
  std::sort(vals.begin(), vals.end());
  size_t i = 0;
  std::function<void(TreeNode*)> inorder = [&](TreeNode* x) {
    if (!x) return;
    inorder(x->left);
    x->val = vals[i++];
    inorder(x->right);
  };
  inorder(t.root);
  return t;
}

static std::vector<TreeNode*> path_to_root(const T& t, TreeNode* x) {
  std::vector<TreeNode*> p;
  for (; x; x = t.parent.count(x) ? t.parent.at(x) : nullptr) p.push_back(x);
  return p;
}

static TreeNode* slow_lca(const T& t, TreeNode* a, TreeNode* b) {
  auto pa = path_to_root(t, a);
  std::set<TreeNode*> sa(pa.begin(), pa.end());
  for (TreeNode* x : path_to_root(t, b))
    if (sa.count(x)) return x;
  return nullptr;
}

static bool same(TreeNode* a, TreeNode* b) {
  if (!a || !b) return a == b;
  return a->val == b->val && same(a->left, b->left) && same(a->right, b->right);
}

static int slow_depth(const T& t, TreeNode* x) {
  return path_to_root(t, x).size() - 1;
}

TEST(basic) {
  auto* r = new TreeNode(3);
  r->left = new TreeNode(9);
  r->right = new TreeNode(20);
  r->right->left = new TreeNode(15);
  r->right->right = new TreeNode(7);
  CHECK_EQ(level_order(r),
           (std::vector<std::vector<int>>{{3}, {9, 20}, {15, 7}}));
  CHECK_EQ(level_order(nullptr), (std::vector<std::vector<int>>{}));
  CHECK(!is_valid_bst(r));
  CHECK(is_valid_bst(nullptr));
  auto* b = new TreeNode(2);
  b->left = new TreeNode(1);
  b->right = new TreeNode(3);
  CHECK(is_valid_bst(b));
  b->right->val = 2;
  CHECK(!is_valid_bst(b));
  auto* c = new TreeNode(INT_MIN);
  c->right = new TreeNode(INT_MAX);
  CHECK(is_valid_bst(c));
  auto* d = new TreeNode(5);
  d->left = new TreeNode(1);
  d->right = new TreeNode(6);
  d->right->left = new TreeNode(4);  // 4 < 5 but in right subtree
  CHECK(!is_valid_bst(d));
  CHECK(lca(r, r->right->left, r->right->right) == r->right);
  CHECK(lca(r, r->left, r->right->right) == r);
  CHECK(lca(r, r->right, r->right->right) == r->right);
  CHECK(lca(r, r->left, r->left) == r->left);
  CHECK_EQ(max_path_sum(r), 47LL);
  auto* neg = new TreeNode(-3);
  CHECK_EQ(max_path_sum(neg), -3LL);
  neg->left = new TreeNode(-1);
  CHECK_EQ(max_path_sum(neg), -1LL);
  CHECK_EQ(diameter(r), 3);
  CHECK_EQ(diameter(neg), 1);
  CHECK_EQ(diameter(new TreeNode(1)), 0);
  CHECK(same(deserialize(serialize(r)), r));
  CHECK(same(deserialize(serialize(nullptr)), nullptr));
  CHECK(same(deserialize(serialize(c)), c));
}

TEST(random_vs_bruteforce) {
  using harness::rnd;
  for (int it = 0; it < 200; ++it) {
    int n = (int)rnd(1, 25);
    T t = rand_tree(n, -10, 10);
    // level order
    std::vector<std::vector<int>> lv;
    std::map<int, std::vector<int>> by_depth;
    std::function<void(TreeNode*, int)> walk = [&](TreeNode* x, int d) {
      if (!x) return;
      by_depth[d].push_back(x->val);
      walk(x->left, d + 1);
      walk(x->right, d + 1);
    };
    walk(t.root, 0);  // preorder visits a level left-to-right
    for (auto& [d, v] : by_depth) lv.push_back(v);
    CHECK_EQ(level_order(t.root), lv);

    for (int k = 0; k < 5; ++k) {
      TreeNode* p = t.nodes[rnd(0, n - 1)];
      TreeNode* q = t.nodes[rnd(0, n - 1)];
      CHECK(lca(t.root, p, q) == slow_lca(t, p, q));
    }

    long long best = LLONG_MIN;
    int diam = 0;
    for (TreeNode* a : t.nodes)
      for (TreeNode* b : t.nodes) {
        TreeNode* l = slow_lca(t, a, b);
        long long s = 0;
        for (TreeNode* x = a; x != l; x = t.parent.at(x)) s += x->val;
        for (TreeNode* x = b; x != l; x = t.parent.at(x)) s += x->val;
        best = std::max(best, s + l->val);
        diam = std::max(
            diam, slow_depth(t, a) + slow_depth(t, b) - 2 * slow_depth(t, l));
      }
    CHECK_EQ(max_path_sum(t.root), best);
    CHECK_EQ(diameter(t.root), diam);

    CHECK(same(deserialize(serialize(t.root)), t.root));

    T b = rand_bst(n);
    CHECK(is_valid_bst(b.root));
    if (n >= 2 && rnd(0, 1)) {
      // swap two values -> invalid (distinct values guarantee it)
      int i = (int)rnd(0, n - 1), j = (int)rnd(0, n - 1);
      if (i != j) {
        std::swap(b.nodes[i]->val, b.nodes[j]->val);
        CHECK(!is_valid_bst(b.root));
      }
    } else if (n >= 2) {
      // duplicate the root's value into a child -> invalid (strict)
      b.nodes[1]->val = b.nodes[0]->val;
      CHECK(!is_valid_bst(b.root));
    }
  }
}

TEST(perf) {
  int n = 1000000;
  TreeNode* root = new TreeNode(0);
  TreeNode* cur = root;
  std::vector<TreeNode*> chain{root};
  for (int i = 1; i < n; ++i) {
    cur = (i & 1 ? cur->left : cur->right) = new TreeNode(i % 7 - 3);
    chain.push_back(cur);
  }
  CHECK_EQ(level_order(root).size(), (size_t)n);
  CHECK(lca(root, chain[n - 1], chain[n - 2]) == chain[n - 2]);
  CHECK(lca(root, chain[n - 1], chain[n / 2]) == chain[n / 2]);
  CHECK(max_path_sum(root) > 0);
  CHECK_EQ(diameter(root), n - 1);
  CHECK(!is_valid_bst(root));
  TreeNode* d = deserialize(serialize(root));
  int cnt = 0;
  for (TreeNode* x = d; x; x = x->left ? x->left : x->right) ++cnt;
  CHECK_EQ(cnt, n);
  // balanced BST with 2^20-1 nodes
  std::function<TreeNode*(int, int)> mk = [&](int lo, int hi) -> TreeNode* {
    if (lo > hi) return nullptr;
    int m = (lo + hi) / 2;
    auto* t = new TreeNode(m);
    t->left = mk(lo, m - 1);
    t->right = mk(m + 1, hi);
    return t;
  };
  TreeNode* bst = mk(0, (1 << 20) - 2);
  CHECK(is_valid_bst(bst));
  CHECK_EQ(diameter(bst), 38);
}
