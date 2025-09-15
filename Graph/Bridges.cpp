struct Bridges {
    int n;
    vector<vector<int>> adj, tree, CCs;
    vector<int> low, vis, dep, id, st;

    vector<array<int, 2>> bridges;

    Bridges (int _n, vector<vector<int>> &g) {
        adj = g;
        n = _n;
        low.assign(n + 1, 1e10), dep.assign(n + 1, 0);
        vis.assign(n + 1, 0), id.assign(n + 1, 0);

        for (int u = 1; u <= n; ++u) if(!vis[u]) dfs(1);

        tree.assign((int)CCs.size() + 1, {});
        for (int u = 1; u <= n; ++u) {
            for (int v : adj[u]) {
                if(id[v] != id[u]) {
                    tree[id[u]].push_back(id[v]);
                }
            }
        }
    }

    void dfs(int u, int p = -1) {
        st.push_back(u);
        vis[u] = 1;
        low[u] = dep[u];
        for (int v : adj[u]) {
            if (v == p) continue;
            if(vis[v]) {
                low[u] = min(low[u], dep[v]);
            }
            else {
                dep[v] = 1 + dep[u];
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > dep[u]) {
                    bridges.push_back( {u, v} );
                }
            }
        }

        if(low[u] == dep[u]) {
            vector<int> comp;
            while (st.back() != u) comp.push_back( st.back() ), st.pop_back();
            comp.push_back( st.back() ), st.pop_back();
            CCs.push_back(comp);
            for (int node : comp) id[node] = (int)CCs.size();
        }

    }
};