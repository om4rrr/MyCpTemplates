// Depends on: Basic.cpp, PointsAndVectors.cpp

bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = orient(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}

bool collinear(pt a, pt b, pt c) { return !orient(a, b, c); }

void convex_hull(vector<pt>& a, bool include_collinear = false) {
    if (a.size() <= 1) return;
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.Y, a.X) < make_pair(b.Y, b.X);
    });
    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = orient(p0, a, b);
        if (o == 0) return sq(p0 - a) < sq(p0 - b);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)a.size() - 1;
        while (i >= 0 && collinear(p0, a[i], a.back())) --i;
        reverse(a.begin() + i + 1, a.end());
    }

    vector<pt> st;
    for (pt p : a) {
        while (st.size() > 1 && !cw(st[st.size() - 2], st.back(), p, include_collinear))
            st.pop_back();
        if (st.empty() || p != st.back())
            st.push_back(p);
    }

    if (!include_collinear && st.size() == 2 && st[0] == st[1])
        st.pop_back();

    a = st;
}
