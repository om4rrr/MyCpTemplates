using T = long double;
const T EPS = 1E-9, PI = acos(-1);

using pt = complex<T>;
#define X real()
#define Y imag()

void input(pt &p) { int a, b; cin >> a >> b; p = pt(a, b); }

int sgn(T val) { return (val > EPS) - (val < -EPS); }

auto fix = [](T x) { return abs(x) < EPS ? 0.0 : x; };


/////////////////////////////////////////////// Points and Vectors ///////////////////////////////////////////////

T sq(pt p) { return p.X * p.X + p.Y * p.Y; }

T dot(pt v, pt w) { return v.X * w.X + v.Y * w.Y; }

T cross(pt v, pt w) { return v.X * w.Y - v.Y * w.X; }

pt perp(pt v) { return {-v.Y, v.X}; }

// 1: ac is LEFT of ab, -1: RIGHT, 0: touching
int orient(pt a, pt b, pt c) { return sgn(cross(b - a, c - a)); }

// area of triangle (bac)
T area(pt a, pt b, pt c) { return fabs(cross(b - a, c - a) / 2); }

// check if p is in angle(bac) counter clockwise
bool inAngle(pt a, pt b, pt c, pt p) {
    if(orient(a, b, c) < 0) swap(b, c);
    return orient(a, b, p) >= 0 && orient(a, c, p) <= 0;
}

// check if angle is 90
bool isPerp(pt v, pt w) { return !sgn(dot(v, w)); }

pt translate(pt p, pt v) { return p + v; }

pt scale(pt p, pt c, T factor) { return c + (p - c) * factor; }

// rotate p around c
pt rotate(pt p, pt c, T theta) {
    return c + (p - c) * polar(T(1), theta);
}

/////////////////////////////////////////////// Lines ///////////////////////////////////////////////

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

    // in which side p lies
    int side(pt p) { return sgn(cross(v, p) - c); }

    // distance of p from the line
    T dist(pt p) { return abs((cross(v, p) - c) / abs(v)); }

    // return perpendicular line touching p
    Line perpTrough(pt p) { return {p, p + perp(v)}; }

    // translate line using vector
    Line translate(pt t) { return {v, c + cross(v, t)}; }

    // translate line to the left using dist
    Line translate(T d) { return {v, c + d * abs(v)}; }

    // compare projection, used to sort point through line
    bool cmpProj(pt a, pt b) { return dot(v, a) < dot(v, b); }

    pt proj(pt p) { return p - perp((v) * T(side(p) / sq(v))); }

    pt refl(pt p) { return p - perp((v) * T(2.0 * side(p) / sq(v))); }
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

/////////////////////////////////////////////// Segments ///////////////////////////////////////////////

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

/////////////////////////////////////////////// Rays ///////////////////////////////////////////////

// check if point p on ray
bool onRay(pt s, pt e, pt p) { return !sgn(orient(p, s, e)) && sgn(dot(p - s, e - s)) >= 0; }

// point p distance from ray
T rayDist(pt s, pt e, pt p) {
    if (sgn(dot(p - s, e - s)) <= 0) return abs(s - p);
    return Line(s, e).dist(p);
}

// 0: no intersection, -1: infinite
pair<int, pt> rayInter(pt s1, pt e1, pt s2, pt e2) {
    auto [yes, p] = inter(Line(s1, e1), Line(s2, e2));
    if (yes == 0) return {0, {}};
    if (yes == 1) {
        if (onRay(s1, e1, p) && onRay(s2, e2, p)) return {1, p};
        return {0, {}};
    }
    if (onRay(s2, e2, s1) || onRay(s1, e1, s2))
        return {-1, onRay(s2, e2, s1) ? s1 : s2};
    return {0, {0, 0}};
}

// distance between two rays
T rayRayDist(pt s1, pt e1, pt s2, pt e2) {
    if (rayInter(s1, e1, s2, e2).first) return 0;
    return min(rayDist(s2, e2, s1), rayDist(s1, e1, s2));
}


/////////////////////////////////////////////// Polygons ///////////////////////////////////////////////

T areaPolygon(vector<pt> p) {
    T area = 0;
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        area += cross(p[i], p[(i + 1) % n]);
    }
    return abs(area) / 2;
}

bool above(pt a, pt p) { return p.Y >= a.Y; }

// if ray A pointing right crosses segment(p, q)
bool crossesRay(pt a, pt p, pt q) {
    return (above(a,q) - above(a,p)) * orient(a,p,q) > 0;
}

// check if point a in polygon p
// strict = true --> edges does not count
bool inPolygon(vector<pt> &p, pt a, bool strict = true) {
    int cnt = 0;
    for (int i = 0, n = p.size(); i < n; ++i) {
        if (onSegment(p[i], p[ (i + 1) % n ], a)) return !strict;
        cnt += crossesRay(a, p[i], p[ (i + 1) % n ]);
    }
    return cnt & 1;
}


void CCW(vector<pt> &p) {
    for (int i = 0, n = p.size(); i < n; ++i) {
        if(sgn(orient(p[i], p[(i + 1) % n], p[(i + 2) % n])) > 0) return;
    }
    reverse(begin(p), end(p));
}

bool inConvex(vector<pt> & p, pt v) {
    int l = 1, r = p.size() - 2, mid, ans = 1;
    while(l <= r) {
        mid = (l + r) / 2;
        if(sgn( orient(p[0], p[mid], v) ) >= 0) {
            ans = mid;
            l = mid + 1;
        }
        else {
            r = mid - 1;
        }
    }
    vector<pt> ret = {p[0], p[ans], p[ans + 1]};
    return inPolygon(ret, v, false);
}

void reorder_polygon(vector<pt> & P){
    size_t pos = 0;
    for(size_t i = 1; i < P.size(); i++){
        if(P[i].Y < P[pos].Y || (P[i].Y == P[pos].Y && P[i].X < P[pos].X))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

//p must be counter clockwise
vector<pt> minkowski(vector<pt> P, vector<pt> Q){
    // the first vertex must be the lowest
    reorder_polygon(P);
    reorder_polygon(Q);
    // we must ensure cyclic indexing
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);
    // main part
    vector<pt> result;
    size_t i = 0, j = 0;
    while(i < P.size() - 2 || j < Q.size() - 2){
        result.push_back(P[i] + Q[j]);
        auto c = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        if(c >= 0 && i < P.size() - 2)
            ++i;
        if(c <= 0 && j < Q.size() - 2)
            ++j;
    }
    return result;
}

// minimum distance between two convex ploygons
T minDistBetweenConvexs(vector<pt> p1, vector<pt> p2) {
    for(pt &p : p2) p *= -1;
    auto p = minkowski(p1, p2);

    if(inPolygon(p, pt(0, 0), false)) return 0.0;

    T ret = 2e18;
    for (int i = 0, n = p.size(); i < n; ++i)
        ret = min(ret, segDist(p[i], p[(i + 1) % n], pt(0, 0)));
    return ret;
}

/////////////////////////////////////////////// Circles /////////////////////////////////////////////////////////

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
    random_shuffle(p.begin(), p.end());
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

/////////////////////////////////////////////  Half Plane ////////////////////////////////////////////////////////

// Basic half-plane struct.
struct Halfplane {

    // 'p' is a passing point of the line and 'pq' is the direction vector of the line.
    pt p, pq;
    long double angle;

    Halfplane() {}
    Halfplane(const pt& a, const pt& b) : p(a), pq(b - a) {
        angle = atan2l(pq.Y, pq.X);
    }

    // Check if point 'r' is outside this half-plane.
    // Every half-plane allows the region to the LEFT of its line.
    bool out(const pt& r) {
        return cross(pq, r - p) < -EPS;
    }

    // Comparator for sorting.
    bool operator < (const Halfplane& e) const {
        return angle < e.angle;
    }

    // Intersection point of the lines of two half-planes. It is assumed they're never parallel.
    friend pt inter(const Halfplane& s, const Halfplane& t) {
        long double alpha = cross((t.p - s.p), t.pq) / cross(s.pq, t.pq);
        return s.p + (s.pq * alpha);
    }
};


vector<pt> hp_intersect(vector<Halfplane>& H) {
    const int inf = 1e9;
    pt box[4] = {  // Bounding box in CCW order
            pt(inf, inf),
            pt(-inf, inf),
            pt(-inf, -inf),
            pt(inf, -inf)
    };

    for(int i = 0; i<4; i++) { // Add bounding box half-planes.
        Halfplane aux(box[i], box[(i+1) % 4]);
        H.push_back(aux);
    }

    // Sort by angle and start algorithm
    sort(H.begin(), H.end());
    deque<Halfplane> dq;
    int len = 0;
    for(int i = 0; i < H.size(); ++i) {

        // Remove from the back of the deque while last half-plane is redundant
        while (len > 1 && H[i].out(inter(dq[len-1], dq[len-2]))) {
            dq.pop_back();
            --len;
        }

        // Remove from the front of the deque while first half-plane is redundant
        while (len > 1 && H[i].out(inter(dq[0], dq[1]))) {
            dq.pop_front();
            --len;
        }

        // Special case check: Parallel half-planes
        if (len > 0 && fabsl(cross(H[i].pq, dq[len-1].pq)) < EPS) {
            // Opposite parallel half-planes that ended up checked against each other.
            if (dot(H[i].pq, dq[len-1].pq) < 0.0)
                return vector<pt>();

            // Same direction half-plane: keep only the leftmost half-plane.
            if (H[i].out(dq[len-1].p)) {
                dq.pop_back();
                --len;
            }
            else continue;
        }

        // Add new half-plane
        dq.push_back(H[i]);
        ++len;
    }

    // Final cleanup: Check half-planes at the front against the back and vice-versa
    while (len > 2 && dq[0].out(inter(dq[len-1], dq[len-2]))) {
        dq.pop_back();
        --len;
    }

    while (len > 2 && dq[len-1].out(inter(dq[0], dq[1]))) {
        dq.pop_front();
        --len;
    }

    // Report empty intersection if necessary
    if (len < 3) return vector<pt>();

    // Reconstruct the convex polygon from the remaining half-planes.
    vector<pt> ret(len);
    for(int i = 0; i+1 < len; i++) {
        ret[i] = inter(dq[i], dq[i+1]);
    }
    ret.back() = inter(dq[len-1], dq[0]);
    return ret;
}

pair<T, pt> maximumInscribedCircle(vector<pt> &p) {
    int n = p.size();
    pt c;

    auto can = [&] (T r) {
        vector<Halfplane> hp(n);
        for (int i = 0; i < n; ++i) {
            pt w = perp(p[(i + 1) % n] - p[i]);
            w /= abs(w);
            w *= r;
            hp[i] = Halfplane(p[i] + w, p[(i + 1) % n] + w);
        }
        auto ret = hp_intersect(hp);
        if( ret.size() ) c = ret[0];
        return ret.size();
    };


    T l = 0, r = 1e18, mid;
    for (int i = 0; i < 100; ++i) {
        mid = (l + r) / 2 ;
        if( can(mid) ) l = mid;
        else r = mid;
    }
    return {l, c};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
