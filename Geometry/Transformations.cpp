// Depends on: Basic.cpp, PointsAndVectors.cpp

// point p has image fp, point q has image fq, returns image of point r
pt linearTransform(pt p, pt q, pt r, pt fp, pt fq) {
    pt pq = q - p;
    pt num(cross(pq, fq - fp), dot(pq, fq - fp));
    return fp + pt(cross(r - p, num), dot(r - p, num)) / sq(pq);
}
