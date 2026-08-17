// For geometry point type pt.
T maxManhattanDistance(vector<pt> p) {
    T mx1 = -2e18, mn1 = 2e18;
    T mx2 = -2e18, mn2 = 2e18;
    for (pt q : p) {
        mx1 = max(mx1, q.X + q.Y);
        mn1 = min(mn1, q.X + q.Y);
        mx2 = max(mx2, q.X - q.Y);
        mn2 = min(mn2, q.X - q.Y);
    }
    return max(mx1 - mn1, mx2 - mn2);
}

// For integer points stored as {x, y}.
long long maxManhattanDistance(vector<pair<long long, long long>> p) {
    long long mx1 = LLONG_MIN, mn1 = LLONG_MAX;
    long long mx2 = LLONG_MIN, mn2 = LLONG_MAX;
    for (auto [x, y] : p) {
        mx1 = max(mx1, x + y);
        mn1 = min(mn1, x + y);
        mx2 = max(mx2, x - y);
        mn2 = min(mn2, x - y);
    }
    return max(mx1 - mn1, mx2 - mn2);
}

// Sum of Manhattan distances over all pairs of integer points.
__int128 sumManhattanDistances(vector<pair<long long, long long>> p) {
    vector<long long> xs, ys;
    for (auto [x, y] : p) {
        xs.push_back(x);
        ys.push_back(y);
    }
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());

    __int128 ret = 0, pref = 0;
    for (int i = 0; i < (int)xs.size(); ++i) {
        ret += (__int128)xs[i] * i - pref;
        pref += xs[i];
    }

    pref = 0;
    for (int i = 0; i < (int)ys.size(); ++i) {
        ret += (__int128)ys[i] * i - pref;
        pref += ys[i];
    }

    return ret;
}

void printInt128(__int128 x) {
    if (x < 0) {
        cout << '-';
        x = -x;
    }
    if (x >= 10) printInt128(x / 10);
    cout << char('0' + x % 10);
}
