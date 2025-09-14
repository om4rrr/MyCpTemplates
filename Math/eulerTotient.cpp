// O( sqrt(n) )
int _phi(int n) {
    int ret = n;
    for (int i = 2; i * i <= n; ++i) {
        if(n % i == 0) {
            while( !(n % i) ) n /= i;
            ret -= ret / i;
        }
    }
    if(n > 1) ret -= ret / n;
    return ret;
}

const int N = 1'000'00;

int phi[N + 1];

// O( n * log( log(n) ) )
void phi_1_to_n() {
    for (int i = 0; i <= N; i++) {
        phi[i] = i;
    }
    for (int i = 2; i <= N; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= N; j += i)
                phi[j] -= phi[j] / i;
        }
    }
}


// Segmented phi

const int N = 1e7 + 1, M = 1e5 + 3;

int smPfct[N];
int prm[664579], nOfPrm;

void sieve() {
    for (int i = 2; i < N; ++i) {
        if (!smPfct[i]) {
            smPfct[i] = i;
            prm[nOfPrm++] = i;
        }
        int j = 0;
        if (i * 2 >= N)continue;
        do {
            smPfct[i * prm[j]] = prm[j];
        } while (smPfct[i] != prm[j++] && i * prm[j] < N);
    }
}

long  long _phi[M], _rem[M], *rem, *phi;

void segSieve(long long int a, long long int b) {
    rem = _rem - a;
    phi = _phi - a;
    for (long long i = a; i <= b; ++i) {
        rem[i] = i;
        phi[i] = i;
    }
    for (int i = 0; i < nOfPrm && prm[i] * 1ll * prm[i] <= b; ++i) {
        for (long long j = (a + prm[i] - 1) / prm[i] * prm[i]; j <= b; j += prm[i]) {
            phi[j] -= phi[j] / prm[i];
            while (!(rem[j] % prm[i]))
                rem[j] /= prm[i];
        }
    }
    for (long long i = a; i <= b; ++i)
        if (rem[i] > 1)
            phi[i] -= phi[i] / rem[i];
}



//// a ^ x = b (% mod) --- O( sqrt(mod) )
bool DiscreteLog(int a, int b, int M, int &x) {
    a %= M; //// b %= mod ???
    int alpha = 1, add = 0, g;
    while ((g = __gcd(a, M)) > 1) {
        if (b == alpha)
            return x = add, true;
        if (b % g)return false;
        b /= g;
        M /= g;
        ++add;
        alpha = alpha * (a / g % M) % M;
    }
 
    int ph = phi(M);
    int inv = power(a, ph - 1, M);
    unordered_map<int, int> mp;
    int m = ceil(sqrt(M));
    int k = 1;
    for (int i = 0, j = alpha; i < m; ++i) {
        mp.emplace(j, i);
        j = j * a % M;
        k = k * inv % M;
    }
    x = -1;
    for (int i = 0, j = b; i < m; ++i) {
        auto it = mp.find(j);
        if (it != mp.end()) {
            int cur = it->second + i * m + add;
            if (x == -1 || cur < x)
                x = cur;
        }
        j = j * k % M;
    }
    return x != -1;
}