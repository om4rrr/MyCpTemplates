struct Articulation {
    vector<vector<int>> adj, tree, Components;
    vector<int> low, vis, depth, is_art, id, done;
    stack<pair<int, int>> st;
    int n;

    Articulation(int n, vector<vector<int>> &adj) :
            adj(adj), n(n),
            low(n + 1, 0), vis(n + 1, 0), depth(n + 1, 0),
            is_art(n + 1, 0), id(n + 1, 0), done(n + 1, 0) {
        build();
    }

    void saturate(int u = -1, int v = -1) {
        vector<int> comp;
        while (!st.empty()) {
            auto [x, y] = st.top();
            st.pop();
            if (!done[x]) done[x] = 1, comp.emplace_back(x);
            if (!done[y]) done[y] = 1, comp.emplace_back(y);
            if (x == u and y == v) break;
        }
        for (int x: comp) done[x] = 0;
        Components.emplace_back(comp);
    }

    void dfs(int u, int p) {
        vis[u] = 1;
        low[u] = depth[u];
        int children = 0;

        for (auto v: adj[u]) {
            if (v == p) continue;
            if (!vis[v]) {
                st.emplace(u, v);
                children++;
                depth[v] = depth[u] + 1;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if ((p and low[v] >= depth[u]) || (!p and children > 1))
                    is_art[u] = 1;

                if (low[v] >= depth[u])saturate(u, v);
            } else if (vis[v] and depth[v] < depth[u]) {
                st.emplace(u, v);
                low[u] = min(low[u], depth[v]);
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; ++i) {
            if (vis[i]) continue;
            depth[i] = 1;
            dfs(i, 0);
            if (!st.empty())
                saturate();
        }

        int m = Components.size();
        int nextId = m;
        tree.assign(m + 1, {});

        for (int u = 1; u <= m; ++u)
            for (int v: Components[u - 1])
                if (id[v] == 0) id[v] = u;

        for (int u = 1; u <= m; ++u) {
            for (int v: Components[u - 1]) {
                if (!is_art[v]) continue;
                if (id[v] <= m) {
                    id[v] = ++nextId;
                    if (tree.size() <= id[v]) tree.resize(id[v] + 1);
                }
                tree[u].emplace_back(id[v]);
                tree[id[v]].emplace_back(u);
            }
        }
    }
};
