// Depends on: Basic.cpp, PointsAndVectors.cpp, Lines.cpp

// check if point p on ray
bool onRay(pt s, pt e, pt p) { return !sgn(orient(p, s, e)) && sgn(dot(p - s, e - s)) >= 0; }

// point p distance from ray
T rayDist(pt s, pt e, pt p) {
    if (sgn(dot(p - s, e - s)) <= 0) return abs(s - p);
    return Line(s, e).dist(p);
}

// 0: no intersection, 1: one point, 2: infinite overlap
pair<int, pt> rayInter(pt s1, pt e1, pt s2, pt e2) {
    Line l1(s1, e1), l2(s2, e2);
    T d = cross(l1.v, l2.v);
    if (!sgn(d)) {
        if (l1.side(s2)) return {0, {}};
        if (onRay(s1, e1, s2) || onRay(s2, e2, s1))
            return {2, onRay(s1, e1, s2) ? s2 : s1};
        return {0, {}};
    }
    pt p = (l2.v * l1.c - l1.v * l2.c) / d;
    if (onRay(s1, e1, p) && onRay(s2, e2, p)) return {1, p};
    if (onRay(s2, e2, s1) || onRay(s1, e1, s2))
        return {-1, onRay(s2, e2, s1) ? s1 : s2};
    return {0, {}};
}

// distance between two rays
T rayRayDist(pt s1, pt e1, pt s2, pt e2) {
    if (rayInter(s1, e1, s2, e2).first) return 0;
    return min(rayDist(s2, e2, s1), rayDist(s1, e1, s2));
}
