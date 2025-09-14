const int ign = -1e9;

vector<vector<int>> sparseTable;

int merge(int x, int y) {
    return max(x, y);
}

void build(vector<int> &v) {
    const int n = v.size();
    sparseTable = vector<vector<int>>(__lg(n) + 1, vector<int>(n));

    sparseTable[0] = v;

    for (int msk = 1; (1 << msk) <= n; ++msk) {
        for (int i = 0; i + (1 << msk) <= n; ++i) {
            sparseTable[msk][i] = merge(
                    sparseTable[msk - 1][i],
                    sparseTable[msk - 1][i + (1 << (msk - 1))]
            );
        }
    }
}

int query(int l, int r) { // 0-indexed
    int ans = ign;
    int len = r - l + 1;
    for (int msk = 0; l <= r; ++msk) {
        if ((len >> msk) & 1) {
            ans = merge(sparseTable[msk][l], ans);
            l += (1 << msk);
        }
    }
    return ans;
}

int queryOverlap(int l, int r) { // 0-indexed
    int msk = __lg(r - l + 1);
    return merge(
            sparseTable[msk][l],
            sparseTable[msk][r - (1 << msk) + 1]
    );
}
