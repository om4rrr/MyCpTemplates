# Geometry Help

Recommended base order when combining snippets:

1. `Basic.cpp`
2. `PointsAndVectors.cpp`
3. Add only the topic files needed by the problem, following each file's `Depends on` comment.

| Template | Used for | Apps / practice |
| --- | --- | --- |
| `Basic.cpp` | Shared numeric type, epsilon, point type `pt = complex<T>`, input helper, and sign handling. Include before most 2D geometry snippets. | CSES `Point Location Test`; general geometry setup. |
| `PointsAndVectors.cpp` | Dot/cross products, orientation, triangle area, angle containment, perpendicular vector, translation, scaling, and rotation. | CSES `Point Location Test`; Kattis `ccw`. |
| `Angles.cpp` | Angle between vectors, oriented angle, signed angle travel, and angular sorting helpers. | Kattis `robotprotection`; polar-sort problems. |
| `Transformations.cpp` | Linear transformation from two source points and their images, then applying it to another point. | Affine-transform and segment-mapping problems; custom rotation/scale tests. |
| `Lines.cpp` | Line representation, side test, distance, projection, reflection, translation, line intersection, and angle bisector. | LightOJ `1305 - Area of a Parallelogram`; UVA `378 - Intersecting Lines`. |
| `Segments.cpp` | Point on segment, proper segment intersection, all segment intersection points, point-segment distance, and segment-segment distance. | CSES `Line Segment Intersection`; Kattis `intersectinglines`. |
| `Rays.cpp` | Ray containment, point-ray distance, ray-ray intersection, and ray-ray distance. | Laser/ray visibility problems; custom ray-intersection tests. |
| `Polygons.cpp` | Polygon area, point in polygon, forcing CCW order, point in convex polygon, Minkowski sum, and convex-polygon distance. | CSES `Point in Polygon`; Kattis `pointinpolygon`; Codeforces `87E - Mogohu-Rea Idol`. |
| `PolygonProperties.cpp` | Convexity check, central symmetry check, and polygon centroid. | Codeforces `1299B - Aerodynamic`; Kattis `convexpolygonarea`. |
| `ConvexHull.cpp` | Static convex hull with optional collinear boundary points. | Kattis `convexhull`; SPOJ `BSHEEP`; UVA `10065 - Useless Tile Packers`. |
| `PolygonClipping.cpp` | Sutherland-Hodgman clipping by one half-plane. Useful for clipping by a convex polygon edge by edge. | Kattis `convexintersection`; POJ `1279 - Art Gallery`. |
| `ConvexPolygonIntersection.cpp` | Intersection polygon of two convex polygons using repeated half-plane clipping. | Kattis `convexintersection`. |
| `RotatingCalipers.cpp` | Antipodal pairs, convex polygon diameter, and max distance between two convex polygons. | Kattis `roberthood`; diameter checks. |
| `ExtremeVertex.cpp` | Binary search for the convex polygon vertex with maximum dot product in a direction. | Support-function queries on convex polygons; KACTL-style convex support tests. |
| `Circles.cpp` | Circumcircle, circle-line intersection, circle-circle intersection, tangents, minimum enclosing circle, and maximum circle cover. | Kattis `smallestcalculatedcircle`; Kattis `biggest`; UVA `10005 - Packing polygons`. |
| `CircleArea.cpp` | Area of intersection between two circles. | SPOJ `CIRU`; HDU `5120 - Intersection`. |
| `HalfPlane.cpp` | Static half-plane intersection and maximum inscribed circle in a convex polygon. | POJ `1279 - Art Gallery`; CodeChef `ALLPOLY`. |
| `GeometricMedian.cpp` | Approximate point minimizing sum of Euclidean distances to given points. | Fermat-Weber approximation problems; custom convex-distance tests. |
| `TriangleTools.cpp` | Law of sines/cosines helpers for recovering triangle sides and angles. | Triangle formula problems; custom known-triangle tests. |
| `Geometry3D.cpp` | 3D points, planes, lines, distances, projection/reflection, polyhedron volume, spherical helpers, 3D convex hull, smallest enclosing sphere, and 3D segment distances. | Kattis `convexhull3d`; Timus/ICPC 3D hull-volume problems. |
| `ManhattanDistance.cpp` | Maximum Manhattan distance and sum of Manhattan distances over all pairs. | CSES `Maximum Manhattan Distance`; AtCoder ABC `178 E - Dist Max`. |
| `ClosestPairOfPoints.cpp` | Closest pair among integer points, returning original indices. | Kattis `closestpair1`; Kattis `closestpair2`. |
| `AnySegmentIntersection.cpp` | Sweep line to find any intersecting pair among many segments. | Codeforces `1359F - RC Kaboom Show`. |
| `OddRectangleUnionArea.cpp` | Sweep-line area covered by an odd number of axis-aligned rectangles. | Gym `101982F`; custom overlapping-rectangle tests. |
| `RectangleUnionArea.cpp` | Standard sweep-line area covered by at least one axis-aligned rectangle. | Kattis `unrealestate`; SPOJ `NKMARS`; HDU `1542 - Atlantis`. |
| `MaxPointsOnLine.cpp` | Maximum number of points lying on one line by fixing each point and counting normalized directions. | LeetCode `149 - Max Points on a Line`; collinear-points tasks. |
| `DelaunayTriangulation.cpp` | Integer Delaunay triangulation using quad-edge and exact `__int128` predicates. Returns triangle vertices in triples. | DMOJ `cco08p6`; nearest-site triangulation or Voronoi-construction problems. |

Notes:

- Most 2D snippets assume `#include <bits/stdc++.h>` and `using namespace std;` are already present.
- Files using `pt` usually depend on `Basic.cpp`.
- Files using `dot`, `cross`, `orient`, `sq`, or `perp` also need `PointsAndVectors.cpp`.
- `DelaunayTriangulation.cpp` is standalone and uses its own integer point type for exact predicates.
- `OddRectangleUnionArea.cpp` computes odd coverage. Use `RectangleUnionArea.cpp` for normal union area.
