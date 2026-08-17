// Depends on: Basic.cpp, PointsAndVectors.cpp

struct Line {
    pt v; T c;

    // Constructor from direction vector and constant
    // Represents the line cross(v, p) = c
    Line(pt v, T c) : v(v), c(c) {}

    // Constructor from coefficients ax + by = c
    // Represents line using vector (b, -a)
    Line(T a, T b, T c) : v(b, -a), c(c) {}

    // Constructor from two points a --> b
    // Direction vector v = b - a, and c = cross(v, a)
    Line (pt a, pt b) : v(b - a), c(cross(v, a)) {}

    T sideVal(pt p) { return cross(v, p) - c; }

    // in which side p lies
    int side(pt p) { return sgn(sideVal(p)); }

    // distance of p from the line
    T dist(pt p) { return abs(sideVal(p)) / abs(v); }

    T sqDist(pt p) { return sideVal(p) * sideVal(p) / sq(v); }

    // return perpendicular line touching p
    Line perpTrough(pt p) { return {p, p + perp(v)}; }

    // translate line using vector
    Line translate(pt t) { return {v, c + cross(v, t)}; }

    // translate line to the left using dist
    Line translate(T d) { return {v, c + d * abs(v)}; }

    // compare projection, used to sort point through line
    bool cmpProj(pt a, pt b) { return dot(v, a) < dot(v, b); }

    pt proj(pt p) { return p - perp(v) * sideVal(p) / sq(v); }

    pt refl(pt p) { return p - perp(v) * T(2.0) * sideVal(p) / sq(v); }
};

// intersection betwen two lines
pair<bool, pt> inter(Line a, Line b) {
    T d = cross(a.v, b.v);
    if (!sgn(d)) return {false, {}};
    return {true, (b.v * a.c - a.v * b.c) / d};
}

// Line bisector between a and b
Line bisector(Line a, Line b, bool interior) {
    assert(sgn(cross(a.v, b.v)) != 0); // a and b cannot be parallel
    T sign = interior ? 1 : -1;
    return {b.v / abs(b.v) + a.v / abs(a.v) * sign, b.c / abs(b.v) + a.c / abs(a.v) * sign};
}
