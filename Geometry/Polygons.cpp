// Depends on: Basic.cpp, PointsAndVectors.cpp, Segments.cpp

T areaPolygon(vector<pt> p) {
    T area = 0;
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        area += cross(p[i], p[(i + 1) % n]);
    }
    return abs(area) / 2;
}

// Pick's theorem for lattice polygons:
// area = InsideLatticePoints + BoundaryLatticePoints / 2 - 1

long long latticeBoundaryPoints(vector<pt> p) {
    long long ret = 0;
    for (int i = 0, n = p.size(); i < n; ++i) {
        long long x1 = llround(p[i].X), y1 = llround(p[i].Y);
        long long x2 = llround(p[(i + 1) % n].X), y2 = llround(p[(i + 1) % n].Y);
        ret += gcd(abs(x1 - x2), abs(y1 - y2));
    }
    return ret;
}

long long latticeDoubleArea(vector<pt> p) {
    long long ret = 0;
    for (int i = 0, n = p.size(); i < n; ++i) {
        long long x1 = llround(p[i].X), y1 = llround(p[i].Y);
        long long x2 = llround(p[(i + 1) % n].X), y2 = llround(p[(i + 1) % n].Y);
        ret += x1 * y2 - y1 * x2;
    }
    return abs(ret);
}

long long latticeInsidePoints(vector<pt> p) {
    long long twiceArea = latticeDoubleArea(p);
    long long boundary = latticeBoundaryPoints(p);
    return (twiceArea - boundary + 2) / 2;
}

pair<long long, long long> latticeInsideBoundaryPoints(vector<pt> p) {
    long long boundary = latticeBoundaryPoints(p);
    long long inside = (latticeDoubleArea(p) - boundary + 2) / 2;
    return {inside, boundary};
}

bool above(pt a, pt p) { return p.Y >= a.Y; }

// if ray A pointing right crosses segment(p, q)
bool crossesRay(pt a, pt p, pt q) {
    return (above(a,q) - above(a,p)) * orient(a,p,q) > 0;
}

// 0: outside, 1: inside, 2: on boundary
int inPolygon(vector<pt> &p, pt a) {
    int cnt = 0;
    for (int i = 0, n = p.size(); i < n; ++i) {
        if (onSegment(p[i], p[(i + 1) % n], a)) return 2;
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
    return inPolygon(ret, v) != 0;
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

    if(inPolygon(p, pt(0, 0))) return 0.0;

    T ret = 2e18;
    for (int i = 0, n = p.size(); i < n; ++i)
        ret = min(ret, segDist(p[i], p[(i + 1) % n], pt(0, 0)));
    return ret;
}
