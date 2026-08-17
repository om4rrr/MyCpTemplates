// Depends on: Basic.cpp, PointsAndVectors.cpp

// id of the vertex with maximum dot product with z
// polygon must be convex; top is the upper-right vertex
int extremeVertex(vector<pt> &p, const pt &z, int top) {
    int n = p.size();
    if (n == 1) return 0;
    T ans = dot(p[0], z);
    int id = 0;
    if (dot(p[top], z) > ans) {
        ans = dot(p[top], z);
        id = top;
    }

    int l = 1, r = top - 1;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (dot(p[mid + 1], z) >= dot(p[mid], z)) l = mid + 1;
        else r = mid;
    }
    if (l < top && dot(p[l], z) > ans) {
        ans = dot(p[l], z);
        id = l;
    }

    l = top + 1, r = n - 1;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (dot(p[(mid + 1) % n], z) >= dot(p[mid], z)) l = mid + 1;
        else r = mid;
    }
    l %= n;
    if (top + 1 < n && dot(p[l], z) > ans) id = l;
    return id;
}
