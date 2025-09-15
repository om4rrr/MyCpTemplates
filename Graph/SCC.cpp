struct SCC {
    int n;
    vector<vector<int>> adj, r_adj, scc, dag;
    vector<int> vis, id;

    SCC (int n, const vector<vector<int>> adj, const vector<vector<int>> r_adj)
        : n(n), adj(adj), r_adj(r_adj), dag(n + 1), vis(n + 1, 0), id(n + 1, 0) {}


    vector<int> order, comp;
    void dfs(int u, vector<vector<int>> & _adj) {
        vis[u] = 1;
        for (int v : _adj[u]) if(!vis[v]) dfs(v, _adj);
        comp.push_back(u);
    }

    void build() {
        for (int u = 1; u <= n; ++u) if(!vis[u]) dfs(u, adj);

        order = comp;
        reverse(begin(order), end(order));

        vis.assign(n + 1, 0);

        for(int u : order) {
            if (vis[u]) continue;
            comp.clear();
            dfs(u, r_adj);
            scc.emplace_back(comp);
            for (int v : comp) id[v] = (int)scc.size();
        }

        for (int u = 1; u <= n; ++u) {
            for(int v : adj[u]) {
                if(id[v] != id[u]) dag[id[u]].push_back(id[v]);
            }
        }
    }
};