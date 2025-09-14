struct MergeSortTree {
    // change the data type suitable for the problem only for the seg_data.first
    vector<vector<pair<int, pair<int, int>>>> treeData;

    int treeSize;

    MergeSortTree(int n) {
        treeSize = 1;
        while (treeSize < n) treeSize <<= 1;
        treeData.resize(treeSize << 1);
    }

    void merge(int ni, int lf, int ri) {
        int l = 0, r = 0, n = treeData[lf].size(), m = treeData[ri].size(), k = 0;
        treeData[ni].resize(n + m);

        while (l < n && r < m) {
            if (treeData[lf][l].first < treeData[ri][r].first)
                treeData[ni][k++] = {treeData[lf][l].first, {l++, r - 1}};
            else
                treeData[ni][k++] = {treeData[ri][r].first, {l - 1, r++}};
        }

        while (l < n)
            treeData[ni][k++] = {treeData[lf][l].first, {l++, r - 1}};
        while (r < m)
            treeData[ni][k++] = {treeData[ri][r].first, {l - 1, r++}};
    }

    void init(vector<int> &nums, int ni, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < (int) nums.size())
                treeData[ni] = {{nums[lx], {-1, -1}}};
            return;
        }

        int mid = (lx + rx) >> 1;
        init(nums, (ni << 1) + 1, lx, mid);
        init(nums, (ni << 1) + 2, mid, rx);

        merge(ni, (ni << 1) + 1, (ni << 1) + 2);
    }

    void init(vector<int> &nums) {
        init(nums, 0, 0, treeSize);
    }

    int less_equal(int l, int r, int ni, int lx, int rx, int pos) {
        if (pos == -1)
            return 0;

        if (lx >= l && rx <= r) {
            return pos + 1;
        }
        if (lx >= r || rx <= l)
            return 0;

        int mid = (lx + rx) >> 1;

        int ans = less_equal(l, r, (ni << 1) + 1, lx, mid, treeData[ni][pos].second.first);
        ans += less_equal(l, r, (ni << 1) + 2, mid, rx, treeData[ni][pos].second.second);

        return ans;
    }

    int less_equal(int val, int l, int r) {
        auto it = upper_bound(treeData[0].begin(), treeData[0].end(), make_pair(val, make_pair(treeSize, treeSize)));
        int ans = 0;
        if (it != treeData[0].begin()) {
            --it;
            ans = less_equal(l, r, 0, 0, treeSize, it - treeData[0].begin());
        }
        return ans;
    }

    int less(int val, int l, int r) {
        return less_equal(val - 1, l, r);
    }

    int equal(int val, int l, int r) {
        return less_equal(val, l, r) - less_equal(val - 1, l, r);
    }

    int greater(int val, int l, int r) {
        return (r - l) - less_equal(val, l, r);
    }

    int greater_equal(int val, int l, int r) {
        return (r - l) - less_equal(val - 1, l, r);
    }
};