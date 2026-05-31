static const int OO = 4e18;

struct Edge {
  int u, v, cap, flow;
  Edge(int u, int v, int cap) : u(u), v(v), cap(cap), flow(0) {}
};

struct Dinic {
  int n, m = 0, s, t;
  vector<Edge> edges;
  vector<vector<int>> adj;
  vector<int> lvl, ptr, q;

  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
    adj.resize(n + 1);
    lvl.resize(n + 1);
    ptr.resize(n + 1);
    q.resize(n + 1);
  }

  void addEdge(int u, int v, int cap) {
    edges.emplace_back(u, v, cap);
    edges.emplace_back(v, u, 0);
    adj[u].push_back(m++);
    adj[v].push_back(m++);
  }

  bool bfs(int lim) {
    fill(lvl.begin(), lvl.end(), -1);

    int qh = 0, qt = 0;
    q[qt++] = s;
    lvl[s] = 0;

    while (qh < qt) {
      int u = q[qh++];
      for (int id : adj[u]) {
        auto &[_, v, cap, flow] = edges[id];

        if (lvl[v] == -1 && cap - flow >= lim) {
          lvl[v] = lvl[u] + 1;
          q[qt++] = v;
        }
      }
    }

    return lvl[t] != -1;
  }

  int dfs(int u, int pushed) {
    if (!pushed)
      return 0;
    if (u == t)
      return pushed;

    for (int &cid = ptr[u]; cid < (int)adj[u].size(); cid++) {
      int id = adj[u][cid];
      auto &[_, v, cap, flow] = edges[id];

      if (lvl[v] != lvl[u] + 1 || cap - flow <= 0)
        continue;

      int tr = dfs(v, min(pushed, cap - flow));
      if (!tr)
        continue;

      edges[id].flow += tr;
      edges[id ^ 1].flow -= tr;

      return tr;
    }

    return 0;
  }

  int maxflow() {
    int flow = 0;

    for (int bit = 60; bit >= 0; bit--) {
      int lim = (1LL << bit);
      while (bfs(lim)) {
        fill(ptr.begin(), ptr.end(), 0);
        while (int pushed = dfs(s, OO))
          flow += pushed;
      }
    }

    return flow;
  }

  vector<array<int, 2>> minCutEdges() {
    vector<int> vis(n + 1, 0);
    int qh = 0, qt = 0;
    q[qt++] = s;
    vis[s] = 1;

    while (qh < qt) {
      int u = q[qh++];
      for (int id : adj[u]) {
        auto [_, v, cap, flow] = edges[id];
        if (!vis[v] && cap - flow > 0) {
          vis[v] = 1;
          q[qt++] = v;
        }
      }
    }

    vector<array<int, 2>> cut;
    for (int i = 0; i < (int)edges.size(); i += 2) {
      auto [u, v, cap, flow] = edges[i];
      if (vis[u] && !vis[v])
        cut.push_back({u, v});
    }
    return cut;
  }
};

void solve() {
  int n, m;
  cin >> n >> m;
  Dinic F(n, 1, n);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    F.addEdge(u, v, 1);
    F.addEdge(v, u, 1);
  }
  cout << F.maxflow() << '\n';
  auto res = F.minCutEdges();
  for (auto [u, v] : res)
    cout << u << ' ' << v << '\n';
}
