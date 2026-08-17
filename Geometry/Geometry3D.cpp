using T3 = long double;
const T3 EPS3 = 1E-9, PI3 = acosl(-1);

int sgn3(T3 val) { return (val > EPS3) - (val < -EPS3); }

struct p3 {
    T3 x, y, z;

    p3() {}
    p3(T3 x, T3 y, T3 z) : x(x), y(y), z(z) {}

    p3 operator+(p3 p) const { return {x + p.x, y + p.y, z + p.z}; }
    p3 operator-(p3 p) const { return {x - p.x, y - p.y, z - p.z}; }
    p3 operator*(T3 d) const { return {x * d, y * d, z * d}; }
    p3 operator/(T3 d) const { return {x / d, y / d, z / d}; }

    bool operator==(p3 p) const {
        return !sgn3(x - p.x) && !sgn3(y - p.y) && !sgn3(z - p.z);
    }

    bool operator!=(p3 p) const { return !(*this == p); }
};

const p3 zero3 = {0, 0, 0};

T3 dot(p3 v, p3 w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

T3 sq(p3 v) { return dot(v, v); }

T3 abs(p3 v) { return sqrtl(sq(v)); }

p3 unit(p3 v) { return v / abs(v); }

p3 cross(p3 v, p3 w) {
    return {
        v.y * w.z - v.z * w.y,
        v.z * w.x - v.x * w.z,
        v.x * w.y - v.y * w.x
    };
}

T3 angle(p3 v, p3 w) {
    T3 cosTheta = dot(v, w) / abs(v) / abs(w);
    return acosl(max<T3>(-1, min<T3>(1, cosTheta)));
}

T3 orient(p3 p, p3 q, p3 r, p3 s) {
    return dot(cross(q - p, r - p), s - p);
}

T3 orientByNormal(p3 p, p3 q, p3 r, p3 n) {
    return dot(cross(q - p, r - p), n);
}

struct Plane {
    p3 n;
    T3 d;

    Plane() {}
    Plane(p3 n, T3 d) : n(n), d(d) {}
    Plane(p3 n, p3 p) : n(n), d(dot(n, p)) {}
    Plane(p3 p, p3 q, p3 r) : Plane(cross(q - p, r - p), p) {}

    T3 sideVal(p3 p) const { return dot(n, p) - d; }
    int side(p3 p) const { return sgn3(sideVal(p)); }
    T3 dist(p3 p) const { return abs(sideVal(p)) / abs(n); }
    Plane translate(p3 t) const { return {n, d + dot(n, t)}; }
    Plane shiftUp(T3 dist) const { return {n, d + dist * abs(n)}; }
    p3 proj(p3 p) const { return p - n * sideVal(p) / sq(n); }
    p3 refl(p3 p) const { return p - n * 2 * sideVal(p) / sq(n); }

    p3 pointOnPlane() const {
        if (sgn3(n.x)) return {d / n.x, 0, 0};
        if (sgn3(n.y)) return {0, d / n.y, 0};
        return {0, 0, d / n.z};
    }
};

struct Coords3D {
    p3 o, dx, dy, dz;

    Coords3D(p3 p, p3 q, p3 r) : o(p) {
        dx = unit(q - p);
        dz = unit(cross(dx, r - p));
        dy = cross(dz, dx);
    }

    Coords3D(p3 p, p3 q, p3 r, p3 s) : o(p), dx(q - p), dy(r - p), dz(s - p) {}

    p3 pos3d(p3 p) const {
        return {dot(p - o, dx), dot(p - o, dy), dot(p - o, dz)};
    }
};

struct Line3D {
    p3 d, o;

    Line3D() {}
    Line3D(p3 p, p3 q) : d(q - p), o(p) {}

    Line3D(Plane p1, Plane p2) {
        d = cross(p1.n, p2.n);
        o = cross(p2.n * p1.d - p1.n * p2.d, d) / sq(d);
    }

    T3 sqDist(p3 p) const { return sq(cross(d, p - o)) / sq(d); }
    T3 dist(p3 p) const { return sqrtl(sqDist(p)); }
    bool cmpProj(p3 p, p3 q) const { return dot(d, p) < dot(d, q); }
    p3 proj(p3 p) const { return o + d * dot(d, p - o) / sq(d); }
    p3 refl(p3 p) const { return proj(p) * 2 - p; }
    p3 inter(Plane p) const { return o - d * p.sideVal(o) / dot(p.n, d); }
};

T3 dist(Line3D l1, Line3D l2) {
    p3 n = cross(l1.d, l2.d);
    if (n == zero3) return l1.dist(l2.o);
    return abs(dot(l2.o - l1.o, n)) / abs(n);
}

p3 closestOnL1(Line3D l1, Line3D l2) {
    p3 n2 = cross(l2.d, cross(l1.d, l2.d));
    return l1.o + l1.d * dot(l2.o - l1.o, n2) / dot(l1.d, n2);
}

T3 smallAngle(p3 v, p3 w) {
    return acosl(min<T3>(abs(dot(v, w)) / abs(v) / abs(w), 1));
}

T3 angle(Plane p1, Plane p2) { return smallAngle(p1.n, p2.n); }
T3 angle(Line3D l1, Line3D l2) { return smallAngle(l1.d, l2.d); }
T3 angle(Plane p, Line3D l) { return PI3 / 2 - smallAngle(p.n, l.d); }

bool isParallel(Plane p1, Plane p2) { return cross(p1.n, p2.n) == zero3; }
bool isParallel(Line3D l1, Line3D l2) { return cross(l1.d, l2.d) == zero3; }
bool isParallel(Plane p, Line3D l) { return !sgn3(dot(p.n, l.d)); }

bool isPerpendicular(Plane p1, Plane p2) { return !sgn3(dot(p1.n, p2.n)); }
bool isPerpendicular(Line3D l1, Line3D l2) { return !sgn3(dot(l1.d, l2.d)); }
bool isPerpendicular(Plane p, Line3D l) { return cross(p.n, l.d) == zero3; }

Line3D perpThrough(Plane p, p3 o) { return Line3D(o, o + p.n); }
Plane perpThrough(Line3D l, p3 o) { return Plane(l.d, o); }

p3 vectorArea2(vector<p3> p) {
    p3 ret = zero3;
    for (int i = 0, n = p.size(); i < n; ++i)
        ret = ret + cross(p[i], p[(i + 1) % n]);
    return ret;
}

T3 area(vector<p3> p) {
    return abs(vectorArea2(p)) / 2;
}

bool operator<(p3 p, p3 q) {
    return tie(p.x, p.y, p.z) < tie(q.x, q.y, q.z);
}

struct Edge3D {
    int v;
    bool same;
};

void reorientFaces(vector<vector<p3>> &faces) {
    int n = faces.size();
    vector<vector<Edge3D>> g(n);
    map<pair<p3, p3>, int> edges;

    for (int u = 0; u < n; ++u) {
        for (int i = 0, m = faces[u].size(); i < m; ++i) {
            p3 a = faces[u][i], b = faces[u][(i + 1) % m];
            if (edges.count({a, b})) {
                int v = edges[{a, b}];
                g[u].push_back({v, true});
                g[v].push_back({u, true});
            } else if (edges.count({b, a})) {
                int v = edges[{b, a}];
                g[u].push_back({v, false});
                g[v].push_back({u, false});
            } else {
                edges[{a, b}] = u;
            }
        }
    }

    vector<bool> vis(n, false), flip(n, false);
    queue<int> q;
    vis[0] = true;
    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (Edge3D e : g[u]) {
            if (vis[e.v]) continue;
            vis[e.v] = true;
            flip[e.v] = flip[u] ^ e.same;
            q.push(e.v);
        }
    }

    for (int i = 0; i < n; ++i)
        if (flip[i]) reverse(faces[i].begin(), faces[i].end());
}

T3 volume(vector<vector<p3>> faces) {
    T3 vol6 = 0;
    for (vector<p3> f : faces)
        vol6 += dot(vectorArea2(f), f[0]);
    return abs(vol6) / 6;
}

p3 sphericalToCartesian(T3 r, T3 lat, T3 lon) {
    lat *= PI3 / 180;
    lon *= PI3 / 180;
    return {r * cosl(lat) * cosl(lon), r * cosl(lat) * sinl(lon), r * sinl(lat)};
}

int sphereLine(p3 o, T3 r, Line3D l, pair<p3, p3> &out) {
    T3 h2 = r * r - l.sqDist(o);
    if (h2 < 0) return 0;
    p3 p = l.proj(o);
    p3 h = l.d * sqrtl(h2) / abs(l.d);
    out = {p - h, p + h};
    return 1 + (h2 > 0);
}

T3 greatCircleDist(p3 o, T3 r, p3 a, p3 b) {
    return r * angle(a - o, b - o);
}

bool validSphericalSegment(p3 a, p3 b) {
    return cross(a, b) != zero3 || dot(a, b) > 0;
}

bool properInterSpherical(p3 a, p3 b, p3 c, p3 d, p3 &out) {
    p3 ab = cross(a, b), cd = cross(c, d);
    int oa = sgn3(dot(cd, a));
    int ob = sgn3(dot(cd, b));
    int oc = sgn3(dot(ab, c));
    int od = sgn3(dot(ab, d));
    out = cross(ab, cd) * od;
    return oa != ob && oc != od && oa != oc;
}

bool onSphericalSegment(p3 a, p3 b, p3 p) {
    p3 n = cross(a, b);
    if (n == zero3) return cross(a, p) == zero3 && dot(a, p) > 0;
    return !sgn3(dot(n, p)) && dot(n, cross(a, p)) >= 0 && dot(n, cross(b, p)) <= 0;
}

struct DirectionSet : vector<p3> {
    using vector::vector;
    void insert(p3 p) {
        for (p3 q : *this)
            if (cross(p, q) == zero3) return;
        push_back(p);
    }
};

DirectionSet intersSpherical(p3 a, p3 b, p3 c, p3 d) {
    assert(validSphericalSegment(a, b) && validSphericalSegment(c, d));
    p3 out;
    if (properInterSpherical(a, b, c, d, out)) return {out};
    DirectionSet ret;
    if (onSphericalSegment(c, d, a)) ret.insert(a);
    if (onSphericalSegment(c, d, b)) ret.insert(b);
    if (onSphericalSegment(a, b, c)) ret.insert(c);
    if (onSphericalSegment(a, b, d)) ret.insert(d);
    return ret;
}

T3 angleSpherical(p3 a, p3 b, p3 c) {
    return angle(cross(a, b), cross(a, c));
}

T3 orientedAngleSpherical(p3 a, p3 b, p3 c) {
    if (dot(cross(a, b), c) >= 0) return angleSpherical(a, b, c);
    return 2 * PI3 - angleSpherical(a, b, c);
}

T3 areaOnSphere(T3 r, vector<p3> p) {
    int n = p.size();
    T3 sum = -(n - 2) * PI3;
    for (int i = 0; i < n; ++i)
        sum += orientedAngleSpherical(p[(i + 1) % n], p[(i + 2) % n], p[i]);
    return r * r * sum;
}

int windingNumber3D(vector<vector<p3>> faces) {
    T3 sum = 0;
    for (vector<p3> f : faces)
        sum += remainderl(areaOnSphere(1, f), 4 * PI3);
    return roundl(sum / (4 * PI3));
}

struct Mat4 {
    T3 m[4][4];

    Mat4() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j);
    }

    Mat4 operator*(const Mat4 &o) const {
        Mat4 ret;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                ret.m[i][j] = 0;
                for (int k = 0; k < 4; ++k)
                    ret.m[i][j] += m[i][k] * o.m[k][j];
            }
        }
        return ret;
    }

    p3 transform(p3 p) const {
        T3 x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
        T3 y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
        T3 z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
        T3 w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3];
        if (sgn3(w - 1) && sgn3(w)) {
            x /= w;
            y /= w;
            z /= w;
        }
        return {x, y, z};
    }

    static Mat4 translate(T3 tx, T3 ty, T3 tz) {
        Mat4 ret;
        ret.m[0][3] = tx;
        ret.m[1][3] = ty;
        ret.m[2][3] = tz;
        return ret;
    }

    static Mat4 scale(T3 sx, T3 sy, T3 sz) {
        Mat4 ret;
        ret.m[0][0] = sx;
        ret.m[1][1] = sy;
        ret.m[2][2] = sz;
        return ret;
    }

    static Mat4 rotateAxis(p3 axis, T3 theta) {
        axis = unit(axis);
        T3 x = axis.x, y = axis.y, z = axis.z;
        T3 c = cosl(theta), s = sinl(theta), t = 1 - c;
        Mat4 ret;
        ret.m[0][0] = t * x * x + c;
        ret.m[0][1] = t * x * y - s * z;
        ret.m[0][2] = t * x * z + s * y;
        ret.m[1][0] = t * x * y + s * z;
        ret.m[1][1] = t * y * y + c;
        ret.m[1][2] = t * y * z - s * x;
        ret.m[2][0] = t * x * z - s * y;
        ret.m[2][1] = t * y * z + s * x;
        ret.m[2][2] = t * z * z + c;
        return ret;
    }
};

struct Face3D {
    int a, b, c;
    bool ok;
};

struct ConvexHull3D {
    int n, num;
    vector<p3> p;
    vector<Face3D> faces;
    vector<vector<int>> g;

    ConvexHull3D() {}
    ConvexHull3D(vector<p3> p) : n(p.size()), num(0), p(p) {
        faces.resize(8 * n + 1);
        g.assign(n + 1, vector<int>(n + 1));
    }

    T3 volume6(p3 a, p3 b, p3 c, p3 d) {
        return dot(cross(b - a, c - a), d - a);
    }

    T3 side(p3 &point, Face3D &face) {
        p3 a = p[face.a], b = p[face.b], c = p[face.c];
        return dot(cross(b - a, c - a), point - a);
    }

    void deal(int point, int a, int b) {
        int f = g[a][b];
        Face3D add;
        if (faces[f].ok) {
            if (side(p[point], faces[f]) > EPS3) {
                dfs(point, f);
            } else {
                add.a = b;
                add.b = a;
                add.c = point;
                add.ok = true;
                g[point][b] = g[a][point] = g[b][a] = num;
                faces[num++] = add;
            }
        }
    }

    void dfs(int point, int now) {
        faces[now].ok = false;
        deal(point, faces[now].b, faces[now].a);
        deal(point, faces[now].c, faces[now].b);
        deal(point, faces[now].a, faces[now].c);
    }

    bool samePlane(int s, int t) {
        p3 &a = p[faces[s].a], &b = p[faces[s].b], &c = p[faces[s].c];
        return abs(volume6(a, b, c, p[faces[t].a])) < EPS3
            && abs(volume6(a, b, c, p[faces[t].b])) < EPS3
            && abs(volume6(a, b, c, p[faces[t].c])) < EPS3;
    }

    void build() {
        num = 0;
        if (n < 4) return;

        bool bad = true;
        for (int i = 1; i < n; ++i) {
            if (abs(p[0] - p[i]) > EPS3) {
                swap(p[1], p[i]);
                bad = false;
                break;
            }
        }
        if (bad) return;

        bad = true;
        for (int i = 2; i < n; ++i) {
            if (abs(cross(p[0] - p[1], p[1] - p[i])) > EPS3) {
                swap(p[2], p[i]);
                bad = false;
                break;
            }
        }
        if (bad) return;

        bad = true;
        for (int i = 3; i < n; ++i) {
            if (abs(dot(cross(p[0] - p[1], p[1] - p[2]), p[0] - p[i])) > EPS3) {
                swap(p[3], p[i]);
                bad = false;
                break;
            }
        }
        if (bad) return;

        for (int i = 0; i < 4; ++i) {
            Face3D add;
            add.a = (i + 1) % 4;
            add.b = (i + 2) % 4;
            add.c = (i + 3) % 4;
            add.ok = true;
            if (side(p[i], add) > 0) swap(add.b, add.c);
            g[add.a][add.b] = g[add.b][add.c] = g[add.c][add.a] = num;
            faces[num++] = add;
        }

        for (int i = 4; i < n; ++i) {
            for (int j = 0; j < num; ++j) {
                if (faces[j].ok && side(p[i], faces[j]) > EPS3) {
                    dfs(i, j);
                    break;
                }
            }
        }

        int old = num;
        num = 0;
        for (int i = 0; i < old; ++i)
            if (faces[i].ok) faces[num++] = faces[i];
        faces.resize(num);
    }
};

struct Sphere {
    p3 c;
    T3 r;
    Sphere() {}
    Sphere(p3 c, T3 r) : c(c), r(r) {}
};

Sphere smallestEnclosingSphere(vector<p3> p) {
    int n = p.size();
    p3 c(0, 0, 0);
    for (p3 point : p) c = c + point;
    c = c / n;

    T3 ratio = 0.1;
    int pos = 0;
    for (int it = 0; it < 100000; ++it) {
        pos = 0;
        for (int i = 1; i < n; ++i)
            if (sq(c - p[i]) > sq(c - p[pos])) pos = i;
        c = c + (p[pos] - c) * ratio;
        ratio *= 0.998;
    }
    return Sphere(c, abs(c - p[pos]));
}

T3 segPointDist2(p3 a, p3 b, p3 c) {
    if (sgn3(dot(b - a, c - a)) < 0) return sq(a - c);
    if (sgn3(dot(a - b, c - b)) < 0) return sq(b - c);
    return sq(cross(b - a, c - a)) / sq(b - a);
}

T3 segSegDist2(p3 a, p3 b, p3 c, p3 d) {
    T3 ans = min({
        segPointDist2(a, b, c),
        segPointDist2(a, b, d),
        segPointDist2(c, d, a),
        segPointDist2(c, d, b)
    });

    Line3D l1(a, b), l2(c, d);
    p3 closest = closestOnL1(l1, l2);
    if (l1.cmpProj(a, closest) && l1.cmpProj(closest, b))
        ans = min(ans, l2.sqDist(closest));

    return ans;
}
