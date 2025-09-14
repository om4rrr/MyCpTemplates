struct Node {
    int sum, lazy, isLazy;

    Node() {
        sum = lazy = isLazy = 0;
    }

    Node(int x) {
        sum = lazy = x;
        isLazy = 1;
    }

    void operation(int val, int lx, int rx) {
        sum += val * (rx - lx);
        lazy += val;
        isLazy = 1;
    }
};


struct segTree {
    vector<Node> segData;
    int treeSize;

    segTree(int n) {
        treeSize = 1;
        while (treeSize < n)
            treeSize *= 2;
        segData.assign(2 * treeSize, Node());
    }

    Node merge(Node &lf, Node &ri) {
        Node ans = Node();
        ans.sum = lf.sum + ri.sum;
        return ans;
    }

    void propagate(int nx, int lx, int rx) {
        if (!segData[nx].isLazy || rx - lx == 1)
            return;

        int mid = (rx + lx) / 2;

        segData[2 * nx + 1].operation(segData[nx].lazy, lx, mid);
        segData[2 * nx + 2].operation(segData[nx].lazy, mid, rx);

        segData[nx].isLazy = segData[nx].lazy = 0;
    }

    void init(vector<int> &num, int nx, int lx, int rx) // 0 - indexed
    {

        if (rx - lx == 1) {
            if (lx < num.size())
                segData[nx] = Node(num[lx]);
            return;
        }

        int mid = (lx + rx) / 2;
        init(num, 2 * nx + 1, lx, mid);
        init(num, 2 * nx + 2, mid, rx);

        segData[nx] = merge(segData[2 * nx + 1], segData[2 * nx + 2]);
    }

    void init(vector<int> &num) {
        init(num, 0, 0, treeSize);
    }

    void update(int l, int r, int val, int nx, int lx, int rx) // 0 - indexed
    {
        if (lx >= r || rx <= l)
            return;

        if (lx >= l && rx <= r) {
            segData[nx].operation(val, lx, rx);
            return;
        }

        propagate(nx, lx, rx);

        int mid = (lx + rx) / 2;

        update(l, r, val, 2 * nx + 1, lx, mid);
        update(l, r, val, 2 * nx + 2, mid, rx);

        segData[nx] = merge(segData[2 * nx + 1], segData[2 * nx + 2]);
    }

    void update(int l, int r, int val) {
        update(l, r, val, 0, 0, treeSize);
    }

    Node query(int l, int r, int nx, int lx, int rx) // 0 - indexed && r not included
    {
        if (lx >= l && rx <= r)
            return segData[nx];

        if (lx >= r || rx <= l)
            return Node();

        propagate(nx, lx, rx);

        int mid = (lx + rx) / 2;

        Node lf = query(l, r, 2 * nx + 1, lx, mid);
        Node ri = query(l, r, 2 * nx + 2, mid, rx);

        return merge(lf, ri);
    }

    Node query(int l, int r) {
        return query(l, r, 0, 0, treeSize);
    }
};
