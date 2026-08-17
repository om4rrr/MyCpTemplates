// Depends on: Basic.cpp

// lowercase names are sides, uppercase names are opposite angles in radians

// Law of sines: given side b and angles A, B, returns side a.
T sideBySideAndAngles(T b, T A, T B) {
    return sinl(A) * b / sinl(B);
}

// Law of cosines: given sides b, c and included angle A, returns opposite side a.
T sideByTwoSidesAndAngle(T b, T c, T A) {
    return sqrtl(b * b + c * c - 2 * b * c * cosl(A));
}

// Law of sines: given sides a, b and angle B, returns opposite angle A.
T angleByTwoSidesAndAngle(T a, T b, T B) {
    return asinl(a * sinl(B) / b);
}

// Law of cosines: given sides a, b, c, returns angle A opposite side a.
T angleByThreeSides(T a, T b, T c) {
    return acosl((b * b + c * c - a * a) / (2 * b * c));
}

// Half of the triangle perimeter.
T semiPerimeter(T a, T b, T c) {
    return (a + b + c) / 2;
}

// Heron's formula: area from three side lengths.
T areaByThreeSides(T a, T b, T c) {
    T s = semiPerimeter(a, b, c);
    return sqrtl(max<T>(0, s * (s - a) * (s - b) * (s - c)));
}

// Area from two sides and the included angle.
T areaByTwoSidesAndAngle(T b, T c, T A) {
    return b * c * sinl(A) / 2;
}

// Radius of the inscribed circle.
T inRadius(T a, T b, T c) {
    return areaByThreeSides(a, b, c) / semiPerimeter(a, b, c);
}

// Radius of the circumscribed circle.
T circumRadius(T a, T b, T c) {
    return a * b * c / (4 * areaByThreeSides(a, b, c));
}

// Length of the median from angle A to side a.
T medianToSide(T a, T b, T c) {
    return sqrtl((2 * b * b + 2 * c * c - a * a) / 4);
}

// Height from angle A to side a.
T altitudeToSide(T a, T b, T c) {
    return 2 * areaByThreeSides(a, b, c) / a;
}
