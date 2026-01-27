const int B = 31;

struct XorBasis {
    int size;
    vector<int> basis;

    XorBasis () { size = 0; basis.assign(B, 0); }

    void add(int x) {
        for (int i = B - 1; i >= 0; --i) {
            if ( !(x >> i & 1) ) continue;
            if ( !basis[i] ) {
                basis[i] = x;
                ++size;
                return;
            }
            x ^= basis[i];
        }
    }

    int maxXor() {
        int ret = 0;
        for (int i = B - 1; i >= 0; --i)
            if (basis[i] && !(ret >> i & 1)) ret ^= basis[i];
        return ret;
    }

    int getKth(int k) {
        if ( k < 1 || k > 1LL << size ) return -1;

        int ret = 0, c = size;
        for (int i = B - 1; i >= 0; --i) {
            if ( !basis[i] ) continue;
            --c;
            if ( ret >> i & 1 ) {
                if ( 1LL << c >= k ) ret ^= basis[i];
                else k -= 1LL << c;
            }
            else if (1LL << c < k) {
                k -= 1LL << c;
                ret ^= basis[i];
            }
        }
        return ret;
    }
};