// O(V^2 * E)
// O(E * Sqrt(V)) in maximum matching problem (Unit Networks)
static const int OO = 1e18;

struct Edge {
  int u, v, cap = 0, flow = 0;
  Edge(int u, int v) : u(u), v(v) {}
  Edge(int u, int v, int cap) : u(u), v(v), cap(cap) {}
};

struct Dinic {
  int n, m = 0, s, t;
  vector<Edge> edges;
  vector<vector<int>> adj;
  vector<int> lvl, ptr;

  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
    adj.resize(n + 1);
    lvl.resize(n + 1);
    ptr.resize(n + 1);
  }

  void addEdge(int u, int v, int cap) {
    edges.emplace_back(u, v, cap);
    edges.emplace_back(v, u, 0);

    adj[u].push_back(m++);
    adj[v].push_back(m++);
  }

  int bfs() {
    fill(begin(lvl), end(lvl), -1);

    queue<int> q;
    q.push(s);
    lvl[s] = 0;

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int id : adj[u]) {
        auto [_, v, cap, flow] = edges[id];

        if (cap - flow > 0 && lvl[v] == -1) {
          lvl[v] = lvl[u] + 1;
          q.push(v);
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

    for (int &cid = ptr[u]; cid < (int)adj[u].size(); ++cid) {
      int id = adj[u][cid];
      auto [_, v, cap, flow] = edges[id];

      if (lvl[u] + 1 != lvl[v] || cap - flow <= 0)
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
    while (bfs()) {
      fill(begin(ptr), end(ptr), 0);
      while (int pushed = dfs(s, OO))
        flow += pushed;
    }
    return flow;
  }

  vector<array<int, 2>> minCutEdges() {
    vector<int> vis(n + 1, 0);
    queue<int> q;
    q.push(s);
    vis[s] = 1;

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int id : adj[u]) {
        auto [_, v, cap, flow] = edges[id];

        if (!vis[v] && cap - flow > 0) {
          vis[v] = 1;
          q.push(v);
        }
      }
    }

    vector<array<int, 2>> ret;
    for (int i = 0; i < edges.size(); i += 2) {
      auto [u, v, _, __] = edges[i];
      if (vis[u] && !vis[v])
        ret.push_back({u, v});
    }
    return ret;
  }
};

void solve() {
  int n, m;
  cin >> n >> m;
  Dinic F(n, 1, n);
  for (int i = 0; i < m; ++i) {
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
