const int B = 31;

struct XorBasis {
    int size;
    int basis[B];
    int closest[B];

    XorBasis () {
        size = 0;
        for (int i = 0; i < B; ++i) basis[i] = 0, closest[i] = -1;
    }

    void add(int x, int ix) {
        for (int i = B - 1; i >= 0; --i) {
            if ( !(x >> i & 1) ) continue;
            if (closest[i] < ix) {
                if (closest[i] == -1) ++size;
                swap(basis[i], x);
                swap(closest[i], ix);
            }
            x ^= basis[i];
        }
    }

    int maxXor(int l) {
        int ret = 0;
        for (int i = B - 1; i >= 0; --i)
            if (basis[i] && !(ret >> i & 1) && closest[i] >= l) ret ^= basis[i];
        return ret;
    }
};