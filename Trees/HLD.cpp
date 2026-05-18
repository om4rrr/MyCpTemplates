struct HLD {
  vector<vector<int>> adj;
  vector<int> big, head, par, sz, dep, id;

  HLD(int n, vector<vector<int>> &adj)
      : adj(adj), big(n + 1), head(n + 1), par(n + 1), sz(n + 1), dep(n + 1),
        id(n + 1) {
    dfs(1, 0);
    int timer = 0;
    head[1] = 1;
    hld(1, timer);
  }

  void dfs(int u, int p) {
    par[u] = p, dep[u] = dep[p] + 1, sz[u] = 1;
    for (int &v : adj[u]) {
      if (v == p)
        continue;
      dfs(v, u);
      if (!big[u] || sz[v] > sz[big[u]])
        big[u] = v;
      sz[u] += sz[v];
    }
  }

  void hld(int u, int &timer) {
    id[u] = timer++;
    if (big[u]) {
      head[big[u]] = head[u];
      hld(big[u], timer);
    }
    for (int &v : adj[u]) {
      if (v == par[u] || v == big[u])
        continue;
      head[v] = v;
      hld(v, timer);
    }
  }

  vector<array<int, 2>> path(int u, int v) {
    vector<array<int, 2>> ret;
    while (true) {
      if (head[u] == head[v]) {
        if (dep[u] > dep[v])
          swap(u, v);
        ret.push_back({id[u], id[v]});
        return ret;
      }
      if (dep[head[u]] > dep[head[v]])
        swap(u, v);
      ret.push_back({id[head[v]], id[v]});
      v = par[head[v]];
    }
  }
};
