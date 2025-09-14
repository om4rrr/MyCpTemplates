
struct MergeSortTree {
    vector<vector<int>> treeData;
    int treeSize;

    MergeSortTree(int n) {
        treeSize = 1;
        while (treeSize < n)
            treeSize <<= 1;
        treeData.assign(2 * treeSize, {});
    }

    void merge(vector<int> &ret, vector<int> &lf, vector<int> &ri) {

        int n = lf.size(), m = ri.size();

        ret.reserve(n + m);

        int l = 0, r = 0;
        while (l < n && r < m) {
            if (lf[l] < ri[r])
                ret.push_back(lf[l++]);
            else
                ret.push_back(ri[r++]);
        }

        while (l < n)
            ret.push_back(lf[l++]);

        while (r < m)
            ret.push_back(ri[r++]);

    }

    void init(vector<int> &v, int ni, int lx, int rx) {

        if (rx - lx == 1) {
            if (lx < v.size())
                treeData[ni] = {v[lx]};
            return;
        }

        int mid = (lx + rx) >> 1;
        init(v, 2 * ni + 1, lx, mid);
        init(v, 2 * ni + 2, mid, rx);
        merge(treeData[ni], treeData[2 * ni + 1], treeData[2 * ni + 2]);
    }

    void init(vector<int> &v) {
        init(v, 0, 0, treeSize);
    }


    int less_equal(int val, int l, int r, int ni, int lx, int rx) {
        if (r <= lx || l >= rx) {
            return 0;
        }

        if (l <= lx && r >= rx) {
            return upper_bound(treeData[ni].begin(), treeData[ni].end(), val) - treeData[ni].begin();
        }

        int mid = (lx + rx) >> 1;
        return less_equal(val, l, r, 2 * ni + 1, lx, mid) + less_equal(val, l, r, 2 * ni + 2, mid, rx);
    }

    int less_equal(int val, int l, int r) {
        return less_equal(val, l, r, 0, 0, treeSize);
    }

    int less(int val, int l, int r) {
        return less_equal(val - 1, l, r);
    }

    int equal(int val, int l, int r) {
        return less_equal(val, l, r) - less_equal(val - 1, l, r);
    }

    int greater_equal(int val, int l, int r) {
        return (r - l) - less_equal(val - 1, l, r);
    }

    int greater(int val, int l, int r) {
        return (r - l) - less_equal(val, l, r);
    }

};