// Maximum number of points on one line.

long long gcdAbs(long long a, long long b) {
    return gcd(abs(a), abs(b));
}

pair<long long, long long> normalizedDirection(long long dx, long long dy) {
    if (dx == 0 && dy == 0) return {0, 0};
    long long g = gcdAbs(dx, dy);
    dx /= g;
    dy /= g;
    if (dx < 0 || (dx == 0 && dy < 0)) {
        dx = -dx;
        dy = -dy;
    }
    return {dx, dy};
}

int maxPointsOnLine(vector<pair<long long, long long>> p) {
    int n = p.size();
    if (n <= 2) return n;

    int ans = 1;
    for (int i = 0; i < n; ++i) {
        map<pair<long long, long long>, int> cnt;
        int same = 0, best = 0;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            long long dx = p[j].first - p[i].first;
            long long dy = p[j].second - p[i].second;
            if (dx == 0 && dy == 0) {
                ++same;
                continue;
            }
            best = max(best, ++cnt[normalizedDirection(dx, dy)]);
        }
        ans = max(ans, best + same + 1);
    }
    return ans;
}
