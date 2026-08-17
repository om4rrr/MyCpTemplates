// Depends on: Basic.cpp, PointsAndVectors.cpp

// all antipodal pairs of a convex polygon in counter-clockwise order
vector<pair<int, int>> allAntipodalPairs(vector<pt> &p) {
    int n = p.size();
    vector<pair<int, int>> ret;
    auto nxt = [&](int i) { return (i + 1) % n; };
    auto prv = [&](int i) { return (i - 1 + n) % n; };
    vector<bool> vis(n, false);

    for (int p1 = 0, p2 = 0; p1 < n; ++p1) {
        pt base = p[nxt(p1)] - p[p1];
        while (p2 == p1 || p2 == nxt(p1)
            || sgn(cross(base, p[nxt(p2)] - p[p2])) == sgn(cross(base, p[p2] - p[prv(p2)]))) {
            p2 = nxt(p2);
        }

        if (vis[p1]) continue;
        vis[p1] = true;

        ret.push_back({p1, p2});
        ret.push_back({nxt(p1), p2});

        if (!sgn(cross(base, p[nxt(p2)] - p[p2]))) {
            ret.push_back({p1, nxt(p2)});
            ret.push_back({nxt(p1), nxt(p2)});
            vis[p2] = true;
        }
    }
    return ret;
}

T diameterConvexPolygon(vector<pt> &p) {
    T ret = 0;
    for (auto [i, j] : allAntipodalPairs(p))
        ret = max(ret, abs(p[i] - p[j]));
    return ret;
}

T maxDistBetweenConvexPolygons(vector<pt> &a, vector<pt> &b) {
    T ret = 0;
    for (pt x : a)
        for (pt y : b)
            ret = max(ret, abs(x - y));
    return ret;
}
