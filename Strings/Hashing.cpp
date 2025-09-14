const int N = 2e5 + 5; //change this
int mod1, mod2, base1, base2;
int pow1[N], inv1[N], pow2[N], inv2[N];
pair<int, int> hashing[N];

void gen_rand_mods() {
    random_device rd;
    mt19937 mt(rd());
    auto check = [](int x) {
        for (int i = 2; i * i <= x; ++i)
            if (!(x % i))return false;
        return true;
    };
    auto rnd = [&](int l, int r) {
        return uniform_int_distribution<int>(l, r)(mt);
    };
    mod1 = rnd(1e8, 2e9);
    mod2 = rnd(1e8, 2e9);
    base1 = rnd(10, 300);
    base2 = rnd(10, 300);
    while (!check(mod1))--mod1;
    while (mod1 == mod2 or !check(mod2))--mod2;
}

int add(int a, int b, int mod) {
    int ans = a + b;
    if (ans >= mod)
        ans -= mod;
    if (ans < 0)
        ans += mod;
    return ans;
}

int mul(int a, int b, int mod) {
    int ans = (a * 1ll * b) % mod;
    if (ans < 0)
        ans += mod;
    return ans;
}

int power(int a, int b, int mod) {
    if (b <= 0) return 1;
    int ret = power(mul(a, a, mod), b / 2, mod);
    if (b % 2) ret = mul(ret, a, mod);
    return ret;
}

void PreCalc() // Don't forget to call it before all the test cases
{
    pow1[0] = inv1[0] = pow2[0] = inv2[0] = 1;
    int pw_inv1 = power(base1, mod1 - 2, mod1);
    int pw_inv2 = power(base2, mod2 - 2, mod2);

    for (int i = 1; i < N; ++i) {
        pow1[i] = mul(pow1[i - 1], base1, mod1);
        inv1[i] = mul(inv1[i - 1], pw_inv1, mod1);
        pow2[i] = mul(pow2[i - 1], base2, mod2);
        inv2[i] = mul(inv2[i - 1], pw_inv2, mod2);
    }
}

void BuildHash(string &s) {
    int n = s.length();
    for (int i = 0; i < n; ++i) {
        hashing[i].first = add(((i == 0) ? 0 : hashing[i - 1].first),
                               mul(pow1[i], s[i] - 'a' + 1, mod1), mod1);
        hashing[i].second = add(((i == 0) ? 0 : hashing[i - 1].second),
                                mul(pow2[i], s[i] - 'a' + 1, mod2), mod2);
    }
}

pair<int, int> GetHash(int l, int r) {
    pair<int, int> ans;
    ans.first = add(hashing[r].first, ((l == 0) ? 0 : -hashing[l - 1].first), mod1);
    ans.second = add(hashing[r].second, ((l == 0) ? 0 : -hashing[l - 1].second), mod2);
    ans.first = mul(ans.first, inv1[l], mod1);
    ans.second = mul(ans.second, inv2[l], mod2);
    return ans;
}


pair<int, int> GetFullHash(string &s) {
    int n = s.length();
    pair<int, int> hash[2];
    for (int i = 0; i < n; ++i) {
        hash[i & 1].first = add(((i == 0) ? 0 : hash[(i - 1) & 1].first),
                                mul(pow1[i], s[i] - 'a' + 1, mod1), mod1);
        hash[i & 1].second = add(((i == 0) ? 0 : hash[(i - 1) & 1].second),
                                 mul(pow2[i], s[i] - 'a' + 1, mod2), mod2);
    }
    return hash[(n - 1) & 1];
}