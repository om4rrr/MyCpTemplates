const int OO = 1e18;

struct SPFA {
    vector<long long> d;
    vector<int> p, in_queue, relax_count;
    int n, has_negative_cycle = -1;

    SPFA(int s, int _n, vector<vector<pair<int,int>>> &adj) {
        n = _n;
        d.assign(n + 1, OO);
        p.assign(n + 1, -1);
        in_queue.assign(n + 1, 0);
        relax_count.assign(n + 1, 0);

        queue<int> q;
        d[s] = 0;
        q.push(s);
        in_queue[s] = 1;

        bool neg_cycle_found = false;

        while (!q.empty() && !neg_cycle_found) {
            int u = q.front(); q.pop();
            in_queue[u] = 0;

            for (auto [v, w] : adj[u]) {
                if (d[u] == OO) continue;

                if (d[v] > d[u] + w) {
                    d[v] = max(-OO, d[u] + w);
                    p[v] = u;

                    if (!in_queue[v]) {
                        q.push(v);
                        in_queue[v] = 1;
                        relax_count[v]++;

                        if (relax_count[v] >= n) {
                            has_negative_cycle = v;
                            neg_cycle_found = true;
                            break;
                        }
                    }
                }
            }
        }

        if (has_negative_cycle != -1) {
            queue<int> qq;
            vector<int> vis(n + 1, 0);
            qq.push(has_negative_cycle);
            vis[has_negative_cycle] = 1;
            while (!qq.empty()) {
                int u = qq.front(); qq.pop();
                d[u] = -OO;
                for (auto [v, w] : adj[u]) {
                    if (!vis[v]) {
                        qq.push(v);
                        vis[v] = 1;
                    }
                }
            }
        }
    }

    vector<int> negative_cycle() {
        if (has_negative_cycle == -1) return {};
        int y = has_negative_cycle;
        for (int i = 1; i < n; ++i) y = p[y];

        vector<int> path;
        for (int cur = y;; cur = p[cur]) {
            path.push_back(cur);
            if (cur == y && path.size() > 1) break;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};
