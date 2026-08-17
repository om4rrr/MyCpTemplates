using T = long double;
const T EPS = 1E-9, PI = acos(-1);

using pt = complex<T>;
#define X real()
#define Y imag()

void input(pt &p) { int a, b; cin >> a >> b; p = pt(a, b); }

int sgn(T val) { return (val > EPS) - (val < -EPS); }

auto fix = [](T x) { return abs(x) < EPS ? 0.0 : x; };
