// Depends on: Basic.cpp, PointsAndVectors.cpp, Lines.cpp

// checks if point p inside disk with diameter ab
bool inDisk(pt a, pt b, pt p) { return sgn(dot(a - p, b - p)) < 1; }

// checks if point p on segment ab
bool onSegment(pt a, pt b, pt p) { return a == p || b == p || (!sgn(orient(a, b, p)) && inDisk(a, b, p)); }

// proper intersection of 2 segments
pair<bool, pt> properInter(pt a, pt b, pt c, pt d) {
    T oa = orient(c,d,a), ob = orient(c,d,b);
    T oc = orient(a,b,c), od = orient(a,b,d);
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
        return {true, (a * ob - b * oa) / (ob - oa)};
    return {false, {}};
}

// all intersection points between 2 segments
set<pair<T, T>> segmentInterPoints(pt a, pt b, pt c, pt d) {
    set<pair<T, T>> ret;
    auto [hasProper, p] = properInter(a, b, c, d);
    if (hasProper) return {{p.X, p.Y}};
    if (onSegment(c, d, a)) ret.insert({a.X, a.Y});
    if (onSegment(c, d, b)) ret.insert({b.X, b.Y});
    if (onSegment(a, b, c)) ret.insert({c.X, c.Y});
    if (onSegment(a, b, d)) ret.insert({d.X, d.Y});
    return ret;
}

// distance point to segment
T segDist(pt a, pt b, pt p) {
    Line l(a, b);
    if(l.cmpProj(a, p) && l.cmpProj(p, b)) return l.dist(p);
    return min(abs(a - p), abs(b - p));
}

// distance between 2 segments
T segSegDist(pt a, pt b, pt c, pt d) {
    if (properInter(a,b,c,d).first) return 0;
    return min({segDist(a,b,c), segDist(a,b,d), segDist(c,d,a), segDist(c,d,b)});
}
