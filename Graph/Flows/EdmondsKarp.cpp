// O(E^2 * V)
static const int OO = 1e18;

struct EdmondsKarp {
  int n;
  vector<vector<int>> adj, cap;

  EdmondsKarp(int n) : n(n) {
    adj.assign(n + 1, {});
    cap.assign(n + 1, vector<int>(n + 1));
  }

  void addEdge(int u, int v, int w) {
    if (!cap[u][v] && !cap[v][u]) {
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    cap[u][v] += w;
  }

  int bfs(int s, int t, vector<int> &p) {
    fill(begin(p), end(p), -1);

    queue<array<int, 2>> q;
    q.push({s, OO});
    p[s] = -2;

    while (!q.empty()) {
      auto [u, flow] = q.front();
      q.pop();

      if (u == t)
        return flow;

      for (int v : adj[u]) {
        if (cap[u][v] > 0 && p[v] == -1) {
          p[v] = u;
          q.push({v, min(flow, cap[u][v])});
        }
      }
    }

    return 0;
  }

  int maxflow(int s, int t) {
    int flow = 0, newFlow;
    vector<int> p(n + 1);

    while ((newFlow = bfs(s, t, p))) {
      flow += newFlow;

      for (int v = t; v != s; v = p[v]) {
        int u = p[v];
        cap[u][v] -= newFlow;
        cap[v][u] += newFlow;
      }
    }

    return flow;
  }
};

void solve() {
  int n, m;
  cin >> n >> m;
  EdmondsKarp F(n);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    F.addEdge(u, v, w);
  }

  cout << F.maxflow(1, n) << '\n';
}
