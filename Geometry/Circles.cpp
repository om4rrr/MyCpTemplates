// Depends on: Basic.cpp, PointsAndVectors.cpp, Lines.cpp

pair<pt, T> circumCircle(pt a, pt b, pt c) {
    b = b - a, c = c - a; // consider coordinates relative to A
    assert(cross(b,c) != 0); // no circumcircle if A,B,C aligned
    return {a + perp(b * sq(c) - c * sq(b)) / cross(b,c)/(T)2, abs(perp(b * sq(c) - c * sq(b))/cross(b,c)/(T)2)};
}

int circleLine(pt o, T r, Line l, pair<pt,pt> &out) {
    T h2 = r * r - l.dist(o) * l.dist(o);
    if (h2 >= 0) { // the line touches the circle
        pt p = l.proj(o); // point P
        pt h = l.v * (T)(sqrt(h2)/abs(l.v)); // vector parallel to l, of length h
        out = {p-h, p+h};
    }
    return 1 + sgn(h2);
}

int circleCircle(pt o1, T r1, pt o2, T r2, pair<pt,pt> &out) {
    pt d=o2-o1; T d2=sq(d);
    if (d2 == 0) {assert(r1 != r2); return 0;} // concentric circles
    T pd = (d2 + r1*r1 - r2*r2)/2; // = |O_1P| * d
    T h2 = r1*r1 - pd*pd/d2; // = hˆ2
    if (h2 >= 0) {
        pt p = o1 + d*pd/d2, h = perp(d)*sqrt(h2/d2);
        out = {p-h, p+h};
    }
    return 1 + sgn(h2);
}

int tangents(pt o1, T r1, pt o2, T r2, bool inner, vector<pair<pt,pt>> &out) {
    if (inner) r2 = -r2;
    pt d = o2-o1;
    T dr = r1-r2, d2 = sq(d), h2 = d2-dr*dr;
    if (d2 == 0 || h2 < 0) {assert(h2 != 0); return 0;}
    for (T sign : {-1,1}) {
        pt v = (d*dr + perp(d)*sqrt(h2)*sign)/d2;
        out.push_back({o1 + v*r1, o2 + v*r2});
    }
    return 1 + (h2 > 0);
}


// given n points, find the minimum enclosing circle of the points
// call convex_hull() before this for faster solution
// expected O(n)
pair<pt, T> minimum_enclosing_circle(vector<pt> &p) {
    static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(p.begin(), p.end(), rng);
    int n = p.size();
    pt c = p[0];
    T r = 0;
    for (int i = 1; i < n; i++) {
        if (sgn(abs(c - p[i]) - r) > 0) {
            c = p[i], r = 0;
            for (int j = 0; j < i; j++) {
                if (sgn(abs(c - p[j]) - r) > 0) {
                    c = (p[i] + p[j]) / (T)2.0, r = abs(p[i] - p[j]) / 2;
                    for (int k = 0; k < j; k++) {
                        if (sgn(abs(c - p[k]) - r) > 0) {
                            auto [curC, curR] = circumCircle(p[i], p[j], p[k]);
                            c = curC, r = curR;
                        }
                    }
                }
            }
        }
    }
    return {c, r};
}


// find a circle of radius r that contains as many points as possible
// O(n^2 log n);
pair<int, pt> maximum_circle_cover(vector<pt> p, T r) {
    int n = p.size();
    int ans = 0;
    int id = 0; T th = 0;
    for (int i = 0; i < n; ++i) {
        // maximum circle cover when the circle goes through this point
        vector<pair<T, int>> events = {{-PI, +1}, {PI, -1}};
        for (int j = 0; j < n; ++j) {
            if (j == i) continue;
            T d = abs(p[i] - p[j]);
            if (d > r * 2) continue;
            T dir = arg(p[j] - p[i]);
            T ang = acos(d / 2 / r);
            T st = dir - ang, ed = dir + ang;
            if (st > PI) st -= PI * 2;
            if (st <= -PI) st += PI * 2;
            if (ed > PI) ed -= PI * 2;
            if (ed <= -PI) ed += PI * 2;
            events.push_back({st - EPS, +1}); // take care of precisions!
            events.push_back({ed, -1});
            if (st > ed) {
                events.push_back({-PI, +1});
                events.push_back({+PI, -1});
            }
        }
        sort(events.begin(), events.end());
        int cnt = 0;
        for (auto &&e: events) {
            cnt += e.second;
            if (cnt > ans) {
                ans = cnt;
                id = i; th = e.first;
            }
        }
    }
    pt w = pt(p[id].X + r * cos(th), p[id].Y + r * sin(th));
    return {ans, w};
}
