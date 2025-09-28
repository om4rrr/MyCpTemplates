struct DSU {
    int n, sets = 0;
    vector<int> setParent, setSize;

    DSU(int _n) {
        n = _n;
        setParent.resize(n + 1);
        setSize.resize(n + 1);
        for (int i = 1; i <= n; ++i) setParent[i] = i, setSize[i] = 1;
        sets = n;
    }

    int findSet(int u) {
        if(setParent[u] == u) return u;
        return setParent[u] = findSet(setParent[u]);
    }

    bool sameSet(int u, int v) {
        return findSet(u) == findSet(v);
    }

    void unionSet(int u, int v) {
        int a = findSet(u), b = findSet(v);

        if(a == b) return;

        if(setSize[a] > setSize[b]) {
            setParent[b] = a;
            setSize[a] += setSize[b];
        }
        else {
            setParent[a] = b;
            setSize[b] += setSize[a];
        }
        --sets;
    }
};

struct edge {
    int u, v, cost;
};


pair<vector<pair<int, int>>, int> MST(vector<edge> & edges, int n) {
    sort(edges.begin(), edges.end(), [](edge A, edge B){ return A.cost < B.cost; } );

    DSU D(n);
    vector<pair<int, int>> mstEdges;
    long long mstCost = 0;
    for(auto & [u, v, c] : edges) {
        if( !D.sameSet(u, v) ) {
            D.unionSet(u, v);
            mstEdges.push_back({u, v});
            mstCost += c;
        }
    }

    return {mstEdges, mstCost};
}