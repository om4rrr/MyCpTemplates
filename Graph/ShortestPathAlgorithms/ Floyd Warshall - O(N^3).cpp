const int OO = 1e18;

struct floydWarshall {
  vector<vector<int>> d, nxt;
  int n;

  floydWarshall(int n_, vector<array<int, 3>> edge) {
    n = n_;
    d.assign(n + 1, vector<int>(n + 1, OO));
    nxt.assign(n + 1, vector<int>(n + 1));

    for (auto [u, v, w] : edge)
      d[u][v] = min(d[u][v], w), nxt[u][v] = v;

    for (int i = 1; i <= n; ++i)
      d[i][i] = 0, nxt[i][i] = i;

    for (int k = 1; k <= n; ++k) {
      for (int u = 1; u <= n; ++u) {
        for (int v = 1; v <= n; ++v) {
          if (d[u][k] == OO or d[k][v] == OO)
            continue;
          if (d[u][k] + d[k][v] >= d[u][v])
            continue;
          d[u][v] = d[u][k] + d[k][v];
          nxt[u][v] = nxt[u][k];
        }
      }
    }

    for (int k = 1; k <= n; k++) {
      if (d[k][k] >= 0)
        continue;
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
          if (d[i][k] < OO and d[k][j] < OO)
            d[i][j] = -OO;
        }
      }
    }
  }

  vector<int> get_path(int u, int v) {
    vector<int> path;
    if (!nxt[u][v])
      return path;
    path.emplace_back(u);
    while (u != v) {
      u = nxt[u][v];
      path.emplace_back(u);
    }
    return path;
  }

  vector<int> negative_cycle() {
    vector<int> cycle;
    for (int i = 1; i <= n; i++) {
      if (d[i][i] >= 0)
        continue;
      int x = i;
      for (int k = 1; k <= n; k++)
        x = nxt[x][i];

      int cur = x;
      do {
        cycle.push_back(cur);
        cur = nxt[cur][i];
      } while (cur != x and cur);
      break;
    }
    return cycle;
  }
};
