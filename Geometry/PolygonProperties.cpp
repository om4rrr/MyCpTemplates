// Depends on: Basic.cpp, PointsAndVectors.cpp, Polygons.cpp

bool isConvex(vector<pt> p) {
    bool hasPos = false, hasNeg = false;
    for (int i = 0, n = p.size(); i < n; ++i) {
        int o = orient(p[i], p[(i + 1) % n], p[(i + 2) % n]);
        if (o > 0) hasPos = true;
        if (o < 0) hasNeg = true;
    }
    return !(hasPos && hasNeg);
}

bool isCentrallySymmetric(vector<pt> p) {
    int n = p.size();
    if (n & 1) return false;
    pt sum = p[0] + p[n / 2];
    for (int i = 0; i < n; ++i) {
        if (p[i] + p[(i + n / 2) % n] != sum) return false;
    }
    return true;
}

pt centroidPolygon(vector<pt> &p) {
    int n = p.size();
    pt c(0, 0);
    T sum = 0;
    for (int i = 0; i < n; ++i) sum += cross(p[i], p[(i + 1) % n]);
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        c += (p[i] + p[j]) * cross(p[i], p[j]);
    }
    return c / (3 * sum);
}
