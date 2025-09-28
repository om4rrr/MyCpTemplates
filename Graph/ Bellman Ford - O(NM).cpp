const int OO = 1e18;

struct bellmanFord {
    vector<int> d, p;
    int has_negative_cycle = -1, n;

    bellmanFord(int s, int _n, vector<array<int, 3>> &edge) {
        n = _n;
        p.assign(n + 1, -1);
        d.assign(n + 1, OO);
        d[s] = 0;

        for (int i = 1; i <= n; ++i) {
            has_negative_cycle = -1;
            for (auto [u, v, w]: edge) {
                if (d[u] == OO) continue; // remove if you want to check for negative cycle only
                if (d[v] > d[u] + w) {
                    d[v] = max(-OO, d[u] + w);
                    p[v] = u;
                    has_negative_cycle = v;
                }
            }
        }

        if (has_negative_cycle != -1) {
            for (int i = 1; i <= n; i++) {
                for (auto [u, v, w]: edge) {
                    if (d[u] == -OO) d[v] = -OO;
                    else if (d[u] != OO and d[v] > d[u] + w) {
                        d[v] = -OO;
                    }
                }
            }
        }
    };

    vector<int> negative_cycle() {
        if (has_negative_cycle == -1) return {};
        int y = has_negative_cycle;
        for (int i = 1; i < n; ++i) y = p[y];

        vector<int> path;
        for (int cur = y; ; cur = p[cur]) {
            path.push_back(cur);
            if (cur == y and path.size() > 1) break;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};