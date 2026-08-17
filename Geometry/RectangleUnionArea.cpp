// Area covered by at least one axis-aligned rectangle.

struct Rectangle {
    long long x1, y1, x2, y2;
};

struct RectangleEvent {
    long long x, y1, y2;
    int add;

    bool operator<(const RectangleEvent &other) const {
        return x < other.x;
    }
};

struct RectangleUnionSegTree {
    vector<long long> ys, len;
    vector<int> cover;

    void init(vector<long long> coords) {
        ys = coords;
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        len.assign(ys.size() * 4, 0);
        cover.assign(ys.size() * 4, 0);
    }

    void pull(int node, int l, int r) {
        if (cover[node]) len[node] = ys[r] - ys[l];
        else if (l + 1 == r) len[node] = 0;
        else len[node] = len[node << 1] + len[node << 1 | 1];
    }

    void update(int node, int l, int r, int ql, int qr, int add) {
        if (r <= ql || qr <= l) return;
        if (ql <= l && r <= qr) {
            cover[node] += add;
            pull(node, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        update(node << 1, l, mid, ql, qr, add);
        update(node << 1 | 1, mid, r, ql, qr, add);
        pull(node, l, r);
    }
};

long long rectangleUnionArea(vector<Rectangle> rects) {
    vector<RectangleEvent> events;
    vector<long long> ys;
    for (Rectangle r : rects) {
        assert(r.x1 < r.x2 && r.y1 < r.y2);
        events.push_back({r.x1, r.y1, r.y2, +1});
        events.push_back({r.x2, r.y1, r.y2, -1});
        ys.push_back(r.y1);
        ys.push_back(r.y2);
    }
    if (events.empty()) return 0;

    sort(events.begin(), events.end());

    RectangleUnionSegTree seg;
    seg.init(ys);

    long long ret = 0;
    for (int i = 0; i < (int)events.size(); ++i) {
        if (i) ret += seg.len[1] * (events[i].x - events[i - 1].x);
        int l = lower_bound(seg.ys.begin(), seg.ys.end(), events[i].y1) - seg.ys.begin();
        int r = lower_bound(seg.ys.begin(), seg.ys.end(), events[i].y2) - seg.ys.begin();
        seg.update(1, 0, seg.ys.size() - 1, l, r, events[i].add);
    }
    return ret;
}
