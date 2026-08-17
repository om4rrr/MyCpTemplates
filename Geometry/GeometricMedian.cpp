// Depends on: Basic.cpp

pt geometricMedian(vector<pt> &p) {
    pt best(0, 0);

    auto totalDist = [&](T x, T y) {
        T ret = 0;
        best = {x, y};
        for (pt q : p) ret += abs(q - best);
        return ret;
    };

    auto bestY = [&](T x) {
        T l = 0, r = 1e9;
        for (int i = 0; i < 200; ++i) {
            T m1 = l + (r - l) / 3;
            T m2 = r - (r - l) / 3;
            if (totalDist(x, m1) > totalDist(x, m2)) l = m1;
            else r = m2;
        }
        return totalDist(x, l);
    };

    T l = 0, r = 1e9;
    for (int i = 0; i < 200; ++i) {
        T m1 = l + (r - l) / 3;
        T m2 = r - (r - l) / 3;
        if (bestY(m1) > bestY(m2)) l = m1;
        else r = m2;
    }

    return best;
}
