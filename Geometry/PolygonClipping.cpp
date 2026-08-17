// Depends on: Basic.cpp, PointsAndVectors.cpp

// Checks if point p is inside the left side of edge (a, b).
bool insideHalfPlane(pt p, pt a, pt b) {
    return orient(a, b, p) >= 0;
}

pt lineIntersection(pt a, pt b, pt c, pt d) {
    T oa = orient(c, d, a), ob = orient(c, d, b);
    return (a * ob - b * oa) / (ob - oa);
}

// Sutherland-Hodgman clipping by the half-plane to the left of directed edge a -> b.
vector<pt> clipPolygon(vector<pt> poly, pt a, pt b) {
    vector<pt> ret;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        pt cur = poly[i], prev = poly[(i + n - 1) % n];
        bool curInside = insideHalfPlane(cur, a, b);
        bool prevInside = insideHalfPlane(prev, a, b);
        if (curInside) {
            if (!prevInside) ret.push_back(lineIntersection(prev, cur, a, b));
            ret.push_back(cur);
        } else if (prevInside) {
            ret.push_back(lineIntersection(prev, cur, a, b));
        }
    }
    return ret;
}
