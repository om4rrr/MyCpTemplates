// Original Tree
vector<vector<int>> adj;
vector<bool> deleted;
int cnt = 0;
vector<int> sub_size;

// Current centroid component data
int freq[(int)2e5 + 5];

// The total answer for all the centroids
int k;
ll tot = 0;

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

void calc(int u, int p, int value) {
  if (value > k)
    return;
  tot += freq[k - value];
  for (auto &v : adj[u]) {
    if (v == p || deleted[v])
      continue;
    calc(v, u, value + 1);
  }
}
void apply_operation(int u, int p, int value, int choice) {
  if (choice)
    ++freq[value];
  else
    --freq[value];

  for (auto &v : adj[u]) {
    if (v == p || deleted[v])
      continue;
    apply_operation(v, u, value + 1, choice);
  }
}

void centroid_solve(int u, int p) {

  // Add yourself
  ++freq[0];
  for (auto &v : adj[u]) {
    if (v == p || deleted[v])
      continue;
    calc(v, u, 1);
    apply_operation(v, u, 1, 1);
  }

  apply_operation(u, p, 0, 0);
}

void decompose(int u, int p) {
  cnt = 0;
  dfs1(u, p);

  int curr_centroid = find_centroid(u, u);
  if (p == -1)
    p = curr_centroid;

  // The process part to calculate the desired property
  centroid_solve(curr_centroid, p);

  // Delete the current_centroid from the tree and search for the remainings
  deleted[curr_centroid] = 1;
  for (auto &v : adj[curr_centroid]) {
    if (deleted[v])
      continue;
    decompose(v, curr_centroid);
  }
}

void init(int n) {

  cnt = 0;
  adj.assign(n + 1, {});
  deleted.assign(n + 1, 0);
  sub_size.assign(n + 1, 0);
}

void solve(int tc) {
  int n;
  cin >> n >> k;

  init(n);

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  decompose(1, -1);

  cout << tot << "\n";
}
