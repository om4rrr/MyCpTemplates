struct LCA {
  int n;
  int lg;
  vector<vector<int>> adj;
  vector<int> parent;
  vector<int> depth;
  vector<vector<int>> jump;

  LCA(int n, vector<vector<int>> adj) {
    this->n = n;
    this->adj = adj;
    this->lg = __lg(n) + 2;

    parent.assign(n + 1, 0);
    depth.assign(n + 1, 0);
    jump.assign(lg, vector<int>(n + 1, -1));

    build();
  }

  void dfs(int u, int p, int dep) {
    parent[u] = p;
    depth[u] = dep;
    for (int &v : adj[u]) {
      if (v != p)
        dfs(v, u, dep + 1);
    }
  }

  void build(int root = 1) {
    dfs(root, 0, 0);

    for (int i = 1; i <= n; ++i) {
      jump[0][i] = parent[i];
    }

    for (int msk = 1; msk < lg; ++msk) {
      for (int i = 1; i <= n; ++i) {
        if (jump[msk - 1][i] == -1)
          jump[msk][i] = -1;
        else
          jump[msk][i] = jump[msk - 1][jump[msk - 1][i]];
      }
    }
  }

  int get_kth_ancestor(int u, int k) {
    for (int i = lg - 1; i >= 0; --i) {
      if ((k >> i) & 1) {
        if (u == -1)
          return -1;
        u = jump[i][u];
      }
    }
    return u;
  }

  int get_lca(int u, int v) {
    if (depth[v] > depth[u])
      swap(u, v);

    int k = depth[u] - depth[v];
    u = get_kth_ancestor(u, k);

    if (u == v)
      return u;

    for (int i = lg - 1; i >= 0; --i) {
      if (jump[i][v] != jump[i][u]) {
        u = jump[i][u];
        v = jump[i][v];
      }
    }
    return jump[0][u];
  }

  int get_distance(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
  }
};
