// Integer points. Returns indices of a closest pair.

long long dist2(pair<long long, long long> a, pair<long long, long long> b) {
    return (a.first - b.first) * (a.first - b.first)
         + (a.second - b.second) * (a.second - b.second);
}

pair<int, int> closestPairOfPoints(vector<pair<long long, long long>> p) {
    int n = p.size();
    assert(n >= 2);

    vector<pair<pair<long long, long long>, int>> a(n);
    for (int i = 0; i < n; ++i) a[i] = {p[i], i};
    sort(a.begin(), a.end());

    set<pair<long long, int>> active;
    long long best = dist2(a[0].first, a[1].first);
    pair<int, int> ret = {a[0].second, a[1].second};

    for (int l = 0, r = 0; r < n; ++r) {
        while (l < r && (a[r].first.first - a[l].first.first) * (a[r].first.first - a[l].first.first) >= best) {
            active.erase({a[l].first.second, l});
            ++l;
        }

        long long delta = sqrtl(best) + 1;
        auto it = active.lower_bound({a[r].first.second - delta, -1});
        while (it != active.end() && it->first <= a[r].first.second + delta) {
            int i = it->second;
            long long cur = dist2(a[i].first, a[r].first);
            if (cur < best) {
                best = cur;
                ret = {a[i].second, a[r].second};
                delta = sqrtl(best) + 1;
            }
            ++it;
        }

        active.insert({a[r].first.second, r});
    }

    return ret;
}
