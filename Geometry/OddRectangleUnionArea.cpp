// Area covered by an odd number of axis-aligned rectangles.

struct RectLL {
    long long x1, y1, x2, y2;
};

struct RectEvent {
    long long x, y1, y2;
    bool operator<(const RectEvent &other) const { return x < other.x; }
};

struct OddRectangleUnion {
    vector<long long> ys, tree, lazy;

    void init(vector<long long> coords) {
        ys = coords;
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        tree.assign(ys.size() * 4, 0);
        lazy.assign(ys.size() * 4, 0);
    }

    void push(int node, int l, int r) {
        if (!lazy[node]) return;
        tree[node] = ys[r] - ys[l] - tree[node];
        if (l + 1 < r) {
            lazy[node << 1] ^= 1;
            lazy[node << 1 | 1] ^= 1;
        }
        lazy[node] = 0;
    }

    void update(int node, int l, int r, int ql, int qr) {
        push(node, l, r);
        if (r <= ql || qr <= l) return;
        if (ql <= l && r <= qr) {
            lazy[node] ^= 1;
            push(node, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        update(node << 1, l, mid, ql, qr);
        update(node << 1 | 1, mid, r, ql, qr);
        tree[node] = tree[node << 1] + tree[node << 1 | 1];
    }
};

long long oddRectangleUnionArea(vector<RectLL> rects) {
    vector<RectEvent> events;
    vector<long long> ys;
    for (RectLL r : rects) {
        assert(r.x1 < r.x2 && r.y1 < r.y2);
        events.push_back({r.x1, r.y1, r.y2});
        events.push_back({r.x2, r.y1, r.y2});
        ys.push_back(r.y1);
        ys.push_back(r.y2);
    }
    if (events.empty()) return 0;

    sort(events.begin(), events.end());

    OddRectangleUnion seg;
    seg.init(ys);

    long long ret = 0;
    for (int i = 0; i < (int)events.size(); ++i) {
        if (i) ret += seg.tree[1] * (events[i].x - events[i - 1].x);
        int l = lower_bound(seg.ys.begin(), seg.ys.end(), events[i].y1) - seg.ys.begin();
        int r = lower_bound(seg.ys.begin(), seg.ys.end(), events[i].y2) - seg.ys.begin();
        seg.update(1, 0, seg.ys.size() - 1, l, r);
    }
    return ret;
}
