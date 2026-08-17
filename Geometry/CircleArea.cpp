// Depends on: Basic.cpp, PointsAndVectors.cpp

T areaCircleCircleIntersection(pt o1, T r1, pt o2, T r2) {
    T d2 = sq(o1 - o2);

    if ((r1 + r2) * (r1 + r2) <= d2) return 0;

    if ((r1 - r2) * (r1 - r2) >= d2) {
        T r = min(r1, r2);
        return r * r * PI;
    }

    T d = sqrtl(d2);
    T theta1 = 2 * acosl((d2 + r1 * r1 - r2 * r2) / (2 * d * r1));
    T theta2 = 2 * acosl((d2 + r2 * r2 - r1 * r1) / (2 * d * r2));

    return (r1 * r1 * (theta1 - sinl(theta1)) + r2 * r2 * (theta2 - sinl(theta2))) / 2;
}
