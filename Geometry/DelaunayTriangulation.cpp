// Integer Delaunay triangulation. No duplicate points.
// Returns triangles as triples in counter-clockwise order.

struct DelaunayPoint {
    long long x, y;

    DelaunayPoint() : x(0), y(0) {}
    DelaunayPoint(long long x, long long y) : x(x), y(y) {}

    DelaunayPoint operator+(DelaunayPoint p) const { return {x + p.x, y + p.y}; }
    DelaunayPoint operator-(DelaunayPoint p) const { return {x - p.x, y - p.y}; }
    bool operator==(DelaunayPoint p) const { return x == p.x && y == p.y; }
    bool operator<(DelaunayPoint p) const { return x == p.x ? y < p.y : x < p.x; }

    long long norm2() const { return x * x + y * y; }
    long long cross(DelaunayPoint p) const { return x * p.y - y * p.x; }
    long long cross(DelaunayPoint a, DelaunayPoint b) const { return (a - *this).cross(b - *this); }
};

using DelaunayInt = __int128_t;
using DelaunayEdge = struct DelaunayQuad*;

const DelaunayPoint DELAUNAY_ARBITRARY_POINT(LLONG_MAX, LLONG_MAX);

struct DelaunayQuad {
    bool mark;
    DelaunayEdge o, rot;
    DelaunayPoint p;

    DelaunayPoint F() { return r()->p; }
    DelaunayEdge r() { return rot->rot; }
    DelaunayEdge prev() { return rot->o->rot; }
    DelaunayEdge next() { return r()->prev(); }
};

bool inCircumcircle(DelaunayPoint p, DelaunayPoint a, DelaunayPoint b, DelaunayPoint c) {
    DelaunayInt p2 = p.norm2();
    DelaunayInt A = a.norm2() - p2;
    DelaunayInt B = b.norm2() - p2;
    DelaunayInt C = c.norm2() - p2;
    return (DelaunayInt)p.cross(a, b) * C
         + (DelaunayInt)p.cross(b, c) * A
         + (DelaunayInt)p.cross(c, a) * B > 0;
}

DelaunayEdge makeDelaunayEdge(DelaunayPoint orig, DelaunayPoint dest) {
    DelaunayEdge q[] = {
        new DelaunayQuad{false, nullptr, nullptr, orig},
        new DelaunayQuad{false, nullptr, nullptr, DELAUNAY_ARBITRARY_POINT},
        new DelaunayQuad{false, nullptr, nullptr, dest},
        new DelaunayQuad{false, nullptr, nullptr, DELAUNAY_ARBITRARY_POINT}
    };
    for (int i = 0; i < 4; ++i) {
        q[i]->o = q[-i & 3];
        q[i]->rot = q[(i + 1) & 3];
    }
    return q[0];
}

void splice(DelaunayEdge a, DelaunayEdge b) {
    swap(a->o->rot->o, b->o->rot->o);
    swap(a->o, b->o);
}

DelaunayEdge connect(DelaunayEdge a, DelaunayEdge b) {
    DelaunayEdge q = makeDelaunayEdge(a->F(), b->p);
    splice(q, a->next());
    splice(q->r(), b);
    return q;
}

pair<DelaunayEdge, DelaunayEdge> delaunayRec(const vector<DelaunayPoint>& s) {
    if (s.size() <= 3) {
        DelaunayEdge a = makeDelaunayEdge(s[0], s[1]);
        DelaunayEdge b = makeDelaunayEdge(s[1], s.back());
        if (s.size() == 2) return {a, a->r()};
        splice(a->r(), b);
        long long side = s[0].cross(s[1], s[2]);
        DelaunayEdge c = side ? connect(b, a) : nullptr;
        return {
            side < 0 ? c->r() : a,
            side < 0 ? c : b->r()
        };
    }

    #define H(e) e->F(), e->p
    #define VALID(e) (e->F().cross(H(base)) > 0)

    DelaunayEdge A, B, ra, rb;
    int half = s.size() / 2;
    tie(ra, A) = delaunayRec({s.begin(), s.end() - half});
    tie(B, rb) = delaunayRec({s.size() - half + s.begin(), s.end()});

    while ((B->p.cross(H(A)) < 0 && (A = A->next()))
        || (A->p.cross(H(B)) > 0 && (B = B->r()->o)));

    DelaunayEdge base = connect(B->r(), A);
    if (A->p == ra->p) ra = base->r();
    if (B->p == rb->p) rb = base;

    #define DELETE_CANDIDATE(e, init, dir) DelaunayEdge e = init->dir; if (VALID(e)) \
        while (inCircumcircle(e->dir->F(), H(base), e->F())) { \
            DelaunayEdge t = e->dir; \
            splice(e, e->prev()); \
            splice(e->r(), e->r()->prev()); \
            e = t; \
        }

    for (;;) {
        DELETE_CANDIDATE(lc, base->r(), o);
        DELETE_CANDIDATE(rc, base, prev());
        if (!VALID(lc) && !VALID(rc)) break;
        if (!VALID(lc) || (VALID(rc) && inCircumcircle(H(rc), H(lc))))
            base = connect(rc, base->r());
        else
            base = connect(base->r(), lc->r());
    }

    #undef DELETE_CANDIDATE
    #undef VALID
    #undef H

    return {ra, rb};
}

vector<DelaunayPoint> delaunayTriangulation(vector<DelaunayPoint> p) {
    sort(p.begin(), p.end());
    assert(unique(p.begin(), p.end()) == p.end());
    if (p.size() < 2) return {};

    DelaunayEdge e = delaunayRec(p).first;
    vector<DelaunayEdge> q = {e};
    int qi = 0;
    while (e->o->F().cross(e->F(), e->p) < 0) e = e->o;

    #define ADD_DELAUNAY_FACE { \
        DelaunayEdge c = e; \
        do { \
            c->mark = true; \
            p.push_back(c->p); \
            q.push_back(c->r()); \
            c = c->next(); \
        } while (c != e); \
    }

    ADD_DELAUNAY_FACE;
    p.clear();
    while (qi < (int)q.size()) {
        e = q[qi++];
        if (!e->mark) ADD_DELAUNAY_FACE;
    }

    #undef ADD_DELAUNAY_FACE

    return p;
}
