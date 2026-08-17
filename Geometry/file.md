# Geometry Templates

Recommended base order when combining snippets:

1. `Basic.cpp`
2. `PointsAndVectors.cpp`
3. Add only the topic files needed by the problem, following each file's `Depends on` comment.

| File | What it is used for | Practice problem |
| --- | --- | --- |
| `Basic.cpp` | Shared numeric type, epsilon, point type `pt = complex<T>`, input helper, and sign handling. Include it before most 2D geometry snippets. | Use with any geometry problem, for example CSES `Point Location Test`. |
| `PointsAndVectors.cpp` | Dot/cross products, orientation, area of triangle, angle containment, perpendicular vector, translation, scaling, and rotation. | CSES `Point Location Test`, Kattis `ccw`. |
| `Angles.cpp` | Angle between vectors, oriented angle, signed angle travelled, and angular sorting helpers. | Kattis `robotprotection`, Codeforces problems needing polar sort around a point. |
| `Transformations.cpp` | Linear transformation from two source points and their images, then applying it to another point. | Problems about affine transforms or mapping one segment to another; test manually with known rotations/scales. |
| `Lines.cpp` | Line representation, side test, distance, projection, reflection, translation, line intersection, and angle bisector. | LightOJ `1305 - Area of a Parallelogram`, UVA `378 - Intersecting Lines`. |
| `Segments.cpp` | Point on segment, proper segment intersection, all segment intersection points, point-segment distance, and segment-segment distance. | CSES `Line Segment Intersection`, Kattis `intersectinglines`. |
| `Rays.cpp` | Point on ray, point-ray distance, ray-ray intersection, and ray-ray distance. | Problems involving laser/ray visibility; test with custom ray intersection cases. |
| `Polygons.cpp` | Polygon area, point in polygon, forcing CCW order, point in convex polygon, Minkowski sum, and minimum distance between convex polygons. | CSES `Point in Polygon`, Kattis `pointinpolygon`, Codeforces `87E - Mogohu-Rea Idol`. |
| `PolygonProperties.cpp` | Convexity check, central symmetry check, and polygon centroid. | Codeforces `1299B - Aerodynamic`, Kattis `convexpolygonarea`. |
| `ConvexHull.cpp` | Static convex hull with optional collinear boundary points. | Kattis `convexhull`, SPOJ `BSHEEP`, UVA `10065 - Useless Tile Packers`. |
| `PolygonClipping.cpp` | Sutherland-Hodgman clipping by one half-plane. Useful for clipping a polygon by a convex polygon edge by edge. | Kattis `convexintersection`, POJ `1279 - Art Gallery`. |
| `ConvexPolygonIntersection.cpp` | Intersection polygon of two convex polygons using repeated half-plane clipping. | Kattis `convexintersection`. |
| `RotatingCalipers.cpp` | Antipodal pairs, convex polygon diameter, and max distance between two convex polygons. | Kattis `roberthood`, SPOJ `FENCE3` style diameter checks. |
| `ExtremeVertex.cpp` | Binary search for the convex polygon vertex with maximum dot product in a direction. Useful in distance/support-function problems. | Problems requiring support function queries on a convex polygon; test with KACTL-style convex support queries. |
| `Circles.cpp` | Circumcircle, circle-line intersection, circle-circle intersection, tangents, minimum enclosing circle, and maximum circle cover. | Kattis `smallestcalculatedcircle`, Kattis `biggest`, UVA `10005 - Packing polygons`. |
| `CircleArea.cpp` | Area of intersection between two circles. | SPOJ `CIRU - The area of the union of circles` for pair testing, HDU `5120 - Intersection`. |
| `HalfPlane.cpp` | Static half-plane intersection and maximum inscribed circle in a convex polygon. | POJ `1279 - Art Gallery`, CodeChef `ALLPOLY`. |
| `GeometricMedian.cpp` | Approximate point minimizing sum of Euclidean distances to given points, using ternary search. | UVA `10256` variants are not exact; best tested with custom convex-distance cases or problems asking Fermat-Weber approximation. |
| `TriangleTools.cpp` | Law of sines/cosines helpers for recovering triangle sides and angles. | Use as helper in triangle formula problems; test with UVA `10110`-style geometry formula tasks or custom known triangles. |
| `Geometry3D.cpp` | 3D points, planes, lines, distances, projection/reflection, face orientation, polyhedron volume, spherical geometry helpers, 3D convex hull, smallest enclosing sphere, and 3D segment distances. | Kattis `convexhull3d`, Timus/ICPC 3D hull-volume problems, custom plane-line intersection tests. |
| `ManhattanDistance.cpp` | Maximum Manhattan distance among points using `max(x + y) - min(x + y)` and `max(x - y) - min(x - y)`. Has `pt` and integer-pair overloads. | CSES `Maximum Manhattan Distance`, AtCoder ABC `178 E - Dist Max`. |
| `ClosestPairOfPoints.cpp` | Closest pair among integer points, returning original indices. | Kattis `closestpair1`, Kattis `closestpair2`. |
| `AnySegmentIntersection.cpp` | Sweep line to find any intersecting pair among many segments. | Codeforces `1359F - RC Kaboom Show`. |
| `OddRectangleUnionArea.cpp` | Sweep line area covered by an odd number of axis-aligned rectangles. This is XOR/odd coverage, not normal union area. | Gym `101982F`, or custom tests with overlapping rectangles. |
| `RectangleUnionArea.cpp` | Standard sweep-line area covered by at least one axis-aligned rectangle. Uses coordinate compression and a segment tree over active y-length. | Kattis `unrealestate`, SPOJ `NKMARS`, HDU `1542 - Atlantis`. |
| `MaxPointsOnLine.cpp` | Maximum number of points lying on one line by fixing each point and counting normalized directions. | LeetCode `149 - Max Points on a Line`, Codeforces/ICPC collinear-points tasks. |
| `DelaunayTriangulation.cpp` | Integer Delaunay triangulation using quad-edge and exact `__int128` predicates. Returns triangle vertices in triples. | DMOJ `cco08p6`, Kattis/ICPC problems requiring nearest-site triangulation or Voronoi construction. |

Notes:

- Most 2D snippets assume `#include <bits/stdc++.h>` and `using namespace std;` are already present.
- Files using `pt` usually depend on `Basic.cpp`; files using `dot`, `cross`, `orient`, `sq`, or `perp` also need `PointsAndVectors.cpp`.
- `DelaunayTriangulation.cpp` intentionally uses its own integer point type because the algorithm depends on exact predicates.
- `OddRectangleUnionArea.cpp` computes odd coverage. For standard union area, change the segment tree to maintain coverage count and covered length.
