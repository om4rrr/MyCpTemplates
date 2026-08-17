// Depends on: Basic.cpp, PointsAndVectors.cpp

struct SweepSegment {
    pt p, q;
    int id;

    SweepSegment() {}
    SweepSegment(pt p, pt q, int id) : p(p), q(q), id(id) {}

    T evalY(T x) const {
        if (!sgn(p.X - q.X)) return p.Y;
        return p.Y + (q.Y - p.Y) * (x - p.X) / (q.X - p.X);
    }
};

bool intersect1D(T l1, T r1, T l2, T r2) {
    if (l1 > r1) swap(l1, r1);
    if (l2 > r2) swap(l2, r2);
    return max(l1, l2) <= min(r1, r2) + EPS;
}

bool segmentsIntersect(const SweepSegment &a, const SweepSegment &b) {
    return intersect1D(a.p.X, a.q.X, b.p.X, b.q.X)
        && intersect1D(a.p.Y, a.q.Y, b.p.Y, b.q.Y)
        && cross(a.q - a.p, b.p - a.p) * cross(a.q - a.p, b.q - a.p) <= EPS
        && cross(b.q - b.p, a.p - b.p) * cross(b.q - b.p, a.q - b.p) <= EPS;
}

bool operator<(const SweepSegment &a, const SweepSegment &b) {
    T x = max(min(a.p.X, a.q.X), min(b.p.X, b.q.X));
    T ya = a.evalY(x), yb = b.evalY(x);
    if (sgn(ya - yb)) return ya < yb;
    return a.id < b.id;
}

struct SegmentEvent {
    T x;
    int type, id;

    bool operator<(const SegmentEvent &e) const {
        if (sgn(x - e.x)) return x < e.x;
        return make_pair(type, id) < make_pair(e.type, e.id);
    }
};

// Returns any intersecting pair, or {-1, -1} if no pair intersects.
pair<int, int> anySegmentIntersection(const vector<SweepSegment> &segments) {
    vector<SegmentEvent> events;
    for (int i = 0; i < (int)segments.size(); ++i) {
        events.push_back({min(segments[i].p.X, segments[i].q.X), +1, i});
        events.push_back({max(segments[i].p.X, segments[i].q.X), -1, i});
    }
    sort(events.begin(), events.end());

    set<SweepSegment> active;
    vector<set<SweepSegment>::iterator> where(segments.size());

    for (SegmentEvent e : events) {
        int id = e.id;
        if (e.type == +1) {
            auto nxt = active.lower_bound(segments[id]);
            auto prv = nxt == active.begin() ? active.end() : prev(nxt);
            if (nxt != active.end() && segmentsIntersect(*nxt, segments[id])) return {nxt->id, id};
            if (prv != active.end() && segmentsIntersect(*prv, segments[id])) return {prv->id, id};
            where[id] = active.insert(nxt, segments[id]);
        } else {
            auto cur = where[id];
            if (cur == active.end()) continue;
            auto nxt = next(cur);
            auto prv = cur == active.begin() ? active.end() : prev(cur);
            if (nxt != active.end() && prv != active.end() && segmentsIntersect(*nxt, *prv))
                return {prv->id, nxt->id};
            active.erase(cur);
        }
    }

    return {-1, -1};
}
