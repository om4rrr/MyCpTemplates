// Depends on: Basic.cpp

T sq(pt p) { return p.X * p.X + p.Y * p.Y; }

T dot(pt v, pt w) { return v.X * w.X + v.Y * w.Y; }

T cross(pt v, pt w) { return v.X * w.Y - v.Y * w.X; }

pt perp(pt v) { return {-v.Y, v.X}; }

// 1: c is LEFT of ab, -1: RIGHT, 0: touching
int orient(pt a, pt b, pt c) { return sgn(cross(b - a, c - a)); }

// area of triangle (bac)
T area(pt a, pt b, pt c) { return fabs(cross(b - a, c - a) / 2); }

// check if p is in angle(bac) counter clockwise
bool inAngle(pt a, pt b, pt c, pt p) {
  if (orient(a, b, c) < 0)
    swap(b, c);
  return orient(a, b, p) >= 0 && orient(a, c, p) <= 0;
}

// check if angle is 90
bool isPerp(pt v, pt w) { return !sgn(dot(v, w)); }

pt translate(pt p, pt v) { return p + v; }

pt scale(pt p, pt c, T factor) { return c + (p - c) * factor; }

// rotate p around c
pt rotate(pt p, pt c, T theta) { return c + (p - c) * polar(T(1), theta); }
