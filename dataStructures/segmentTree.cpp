const int OO = 1'000'000'000;

struct Node {
    int val;

    Node() {
        val = OO;
    }

    Node(int x) {
        val = x;
    }
};


struct segTree {
    vector<Node>segData;
    int treeSize;
    segTree(int n) {
        treeSize = 1;
        while(treeSize < n)
            treeSize *= 2;
        segData.assign(2 * treeSize, Node());
    }

    Node merge(Node & lf, Node & ri) {
        Node ans = Node();
        ans.val = min(lf.val, ri.val);
        return ans;
    }

    void init(vector<int> &num, int nx, int lx, int rx) {
        if(rx - lx == 1) {
            if(lx < num.size()) segData[nx] = Node(num[lx]);
            return;
        }

        int mid = (lx + rx) / 2;
        init(num, 2 * nx + 1, lx, mid);
        init(num, 2 * nx + 2, mid, rx);

        segData[nx] = merge(segData[2 * nx + 1], segData[2 * nx + 2]);
    }

    void init(vector<int> & num) {
        init(num, 0, 0, treeSize);
    }

    void upd(int idx, int val, int nx, int lx, int rx) {
        if(rx - lx == 1) {
            segData[nx] = Node(val);
            return;
        }

        int mid = (lx + rx) / 2;
        if(idx < mid) {
            upd(idx, val, 2 * nx + 1, lx, mid);
        }
        else {
            upd(idx, val, 2 * nx + 2, mid, rx);
        }

        segData[nx] = merge(segData[2 * nx + 1], segData[2 * nx + 2]);
    }

    void upd(int idx, int val) {
        upd(idx, val, 0, 0, treeSize);
    }

    Node query(int l, int r, int nx, int lx, int rx) {
        if(lx >= l && rx <= r) return segData[nx];
        if(lx >= r || rx <= l) return Node();

        int mid = (lx + rx) / 2;
        Node lf = query(l, r, 2 * nx + 1, lx, mid);
        Node ri = query(l, r, 2 * nx + 2, mid, rx);

        return merge(lf, ri);
    }

    Node query(int l, int r) {
        return query(l, r, 0, 0, treeSize);
    }
};
