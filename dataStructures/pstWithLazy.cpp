struct Node {
  int val{};
  int lazy{};
  Node *left = nullptr, *right = nullptr;
  Node() {}
  Node(long long val) : val(val) {}
};

struct PST {
  int n;
  vector<Node *> roots;

  PST(int n) : n(n) {}

  Node *build(int l, int r, const vector<int> &v) {
    if (l == r)
      return new Node(v[l]);
    int mid = l + (r - l) / 2;
    Node *leftNode = build(l, mid, v);
    Node *rightNode = build(mid + 1, r, v);

    Node *cur = new Node();
    cur->left = leftNode;
    cur->right = rightNode;
    cur->val = leftNode->val + rightNode->val;
    return cur;
  }

  void build(const vector<int> &v) { roots.push_back(build(0, n - 1, v)); }

  Node *update(Node *node, int l, int r, int ql, int qr, int delta) {
    if (l > qr || r < ql)
      return node;
    Node *cur = new Node();
    if (node)
      *cur = *node;

    if (l >= ql && r <= qr) {
      cur->lazy += delta;
      cur->val += (r - l + 1) * delta;
      return cur;
    }
    int mid = l + (r - l) / 2;
    cur->left = update(node ? node->left : nullptr, l, mid, ql, qr, delta);
    cur->right =
        update(node ? node->right : nullptr, mid + 1, r, ql, qr, delta);

    cur->val = (cur->left ? cur->left->val : 0) +
               (cur->right ? cur->right->val : 0) + (r - l + 1) * cur->lazy;
    return cur;
  }

  void update(int id, int ql, int qr, int delta) {
    roots.push_back(update(roots[id], 0, n - 1, ql, qr, delta));
  }

  int query(Node *node, int l, int r, int ql, int qr, int acc_lazy = 0) {
    if (!node || l > qr || r < ql)
      return 0;
    if (l >= ql && r <= qr) {
      return node->val + acc_lazy * (r - l + 1);
    }
    int mid = l + (r - l) / 2;
    return query(node->left, l, mid, ql, qr, acc_lazy + node->lazy) +
           query(node->right, mid + 1, r, ql, qr, acc_lazy + node->lazy);
  }

  int query(int id, int ql, int qr) {
    return query(roots[id], 0, n - 1, ql, qr, 0);
  }
};
