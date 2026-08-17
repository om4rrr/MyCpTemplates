// Depends on: Basic.cpp, PointsAndVectors.cpp

T angle(pt v, pt w) {
    T val = dot(v, w) / abs(v) / abs(w);
    val = min<T>(val, 1);
    val = max<T>(val, -1);
    return acosl(val);
}

// angle BAC in [0, 2 * PI)
T orientedAngle(pt a, pt b, pt c) {
    if (orient(a, b, c) >= 0) return angle(b - a, c - a);
    return 2 * PI - angle(b - a, c - a);
}

// signed angle travelled around a from p to q
T angleTravelled(pt a, pt p, pt q) {
    T ret = angle(p - a, q - a);
    return orient(a, p, q) > 0 ? ret : -ret;
}

bool upperHalf(pt p) {
    return p.Y > 0 || (!sgn(p.Y) && p.X < 0);
}

bool upperHalfFromPositiveX(pt p) {
    return p.Y > 0 || (!sgn(p.Y) && p.X > 0);
}

struct Angle {
    pt d;
    Angle() : d(0, 0) {}
    Angle(pt d) : d(d) {}
};

bool operator<(Angle a, Angle b) {
    bool ha = upperHalfFromPositiveX(a.d);
    bool hb = upperHalfFromPositiveX(b.d);
    if (ha != hb) return ha > hb;
    return cross(a.d, b.d) > 0;
}

Angle operator-(Angle a, Angle b) {
    return Angle({
        a.d.X * b.d.X + a.d.Y * b.d.Y,
        a.d.Y * b.d.X - b.d.Y * a.d.X
    });
}

Angle minimize(Angle ang) {
    if (!upperHalf(ang.d)) ang = Angle({1, 0}) - ang;
    return ang;
}

bool operator==(Angle a, Angle b) {
    return sgn(a.d.X) == sgn(b.d.X)
        && sgn(a.d.Y) == sgn(b.d.Y)
        && !sgn(cross(a.d, b.d));
}
