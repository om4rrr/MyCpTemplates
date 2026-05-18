// Original Tree
vector<vector<int>> adj;
vector<bool> deleted;
int cnt = 0;
vector<int> sub_size;

// Centroid Tree
const int LOG = 20;
vector<int> par, level;
vector<vector<ll>> dist;

// Calculate the size of each subtree
void dfs1(int u, int p) {
  sub_size[u] = 1;
  ++cnt;

  for (auto &v : adj[u]) {
    if (v == p || deleted[v])
      continue;
    dfs1(v, u);
    sub_size[u] += sub_size[v];
  }
}

int find_centroid(int u, int p) {
  for (auto &v : adj[u]) {
    if (deleted[v] || v == p || sub_size[v] <= cnt / 2)
      continue;
    return find_centroid(v, u);
  }
  return u;
}

void add_edge_to_centroid_tree(int u, int p) {
  par[u] = p;
  level[u] = level[p] + 1;
}

void dfs2(int u, int p, int level) {
  for (auto &v : adj[u]) {
    if (v == p || deleted[v])
      continue;
    dist[level][v] = dist[level][u] + 1;
    dfs2(v, u, level);
  }
}

void decompose(int u, int p) {
  cnt = 0;
  dfs1(u, p);

  int curr_centroid = find_centroid(u, u);

  if (p == -1)
    p = curr_centroid;
  add_edge_to_centroid_tree(curr_centroid, p);

  // The process part to calculate the desired property
  dfs2(curr_centroid, curr_centroid, level[curr_centroid]);

  // Delete the current_centroid from the tree and search for the remainings
  deleted[curr_centroid] = 1;
  for (auto &v : adj[curr_centroid]) {
    if (deleted[v])
      continue;
    decompose(v, curr_centroid);
  }
}

int compute_the_answer(int u, int v) {

  // Get the level of their common ancestor in the centroid tree
  int lca_level = 0;
  for (int i = u, j = v; (lca_level = level[i]) && i != j;)
    level[i] < level[j] ? (j = par[j]) : (i = par[i]);

  // merge the two paths
  return dist[lca_level][u] + dist[lca_level][v];
}

void init(int n) {

  cnt = 0;
  adj.assign(n + 1, {});
  deleted.assign(n + 1, 0);
  sub_size.assign(n + 1, 0);

  par.assign(n + 1, 0);
  level.assign(n + 1, 0);
  dist.assign(LOG, vector<int>(n + 1));
}

void solve(int tc) {
  int n;
  cin >> n;
  int m;
  cin >> m;

  init(n);

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  decompose(1, -1);

  while (m--) {
    int u, v;
    cin >> u >> v;
    cout << compute_the_answer(u, v) << "\n";
  }
}
