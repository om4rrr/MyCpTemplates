// Depends on: Basic.cpp, PointsAndVectors.cpp, Segments.cpp, PolygonClipping.cpp

// Both polygons must be convex and counter-clockwise.
vector<pt> convexConvexIntersection(vector<pt> a, vector<pt> b) {
    for (int i = 0, n = b.size(); i < n && !a.empty(); ++i) {
        a = clipPolygon(a, b[i], b[(i + 1) % n]);
    }
    return a;
}
