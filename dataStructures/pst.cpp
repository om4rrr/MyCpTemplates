struct Node {
  int val;
  Node *left, *right;
  Node() : val(0), left(nullptr), right(nullptr) {}
  Node(int val) : val(val), left(nullptr), right(nullptr) {}
  Node(Node *l, Node *r) {
    left = l, right = r;
    if (left)
      val += left->val;
    if (right)
      val += right->val;
  }
};

struct PST {
  int n;
  vector<Node *> roots;

  PST(int n, int m) : n(n), roots(m) {}

  static Node merge(Node a, Node b) {
    Node res;
    res.val = a.val + b.val;
    return res;
  }

  Node *build(int l, int r, vector<int> &v) {
    if (l == r)
      return new Node(v[l]);
    int mid = (r + l) / 2;
    Node *left = build(l, mid, v), *right = build(mid + 1, r, v);
    return new Node(left, right);
  }

  Node *update(Node *node, int l, int r, int idx, int val) {
    if (l == r)
      return new Node(val);
    int mid = (r + l) / 2;
    if (idx <= mid) {
      return new Node(update(node->left, l, mid, idx, val), node->right);
    }
    return new Node(node->left, update(node->right, mid + 1, r, idx, val));
  }

  Node *update(int id, int idx, int val) {
    return update(roots[id], 0, n - 1, idx, val);
  }

  Node query(Node *node, int l, int r, int ql, int qr) {
    if (l > qr || r < ql)
      return Node();
    if (l >= ql && r <= qr)
      return *node;
    int mid = (r + l) / 2;
    return merge(query(node->left, l, mid, ql, qr),
                 query(node->right, mid + 1, r, ql, qr));
  }

  int query(int id, int l, int r) {
    return query(roots[id], 0, n - 1, l, r).val;
  }

  int kth(Node *u, Node *v, int l, int r, int k) {
    if (l == r)
      return l;
    int mid = (l + r) / 2;
    int sum_left = u->left->val - v->left->val;
    if (sum_left >= k)
      return kth(u->left, v->left, l, mid, k);
    return kth(u->right, v->right, mid + 1, r, k - sum_left);
  }

  int kth(int u, int v, int k) { // kth after sorting from v to u , u > v
    return kth(roots[u], roots[v], 0, n - 1, k);
  }
};
