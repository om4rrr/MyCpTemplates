const int N = 1e5 + 5; //change this
int mod1, mod2, base1, base2;

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

int pw1[N], inv1[N], pw2[N], inv2[N];

int add(int a, int b, int mod)
{
    int ans = a + b;
    if(ans >= mod)
        ans -= mod;
    if(ans < 0)
        ans += mod;
    return ans;
}

int mul(int a, int b, int mod)
{
    int ans = (a * 1ll * b) % mod;
    if(ans < 0)
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
    gen_rand_mods();
    pw1[0] = inv1[0] = pw2[0] = inv2[0] = 1;
    int pw_inv1 = power(base1, mod1 - 2, mod1);
    int pw_inv2 = power(base2, mod2 - 2, mod2);

    for (int i = 1; i < N; ++i) {
        pw1[i] = mul(pw1[i - 1], base1, mod1);
        inv1[i] = mul(inv1[i - 1], pw_inv1, mod1);
        pw2[i] = mul(pw2[i - 1], base2, mod2);
        inv2[i] = mul(inv2[i - 1], pw_inv2, mod2);
    }
}


int n;
vector<vector<int>> adj, UP;
vector<pair<int, int>> h, h_r;
vector<int> parent, depth;
vector<char> vals;

void dfs(int u, int p, pair<int, int> curr_h, pair<int, int> curr_h_r, int l)
{
    parent[u] = p;
    depth[u] = l;

    curr_h.first = add(curr_h.first, mul(pw1[l], vals[u] - 'a' + 1, mod1), mod1);
    curr_h.second = add(curr_h.second, mul(pw2[l], vals[u] - 'a' + 1, mod2), mod2);

    curr_h_r.first = mul(curr_h_r.first, pw1[1], mod1);
    curr_h_r.first = add(curr_h_r.first, mul(pw1[0], vals[u] - 'a' + 1, mod1), mod1);
    curr_h_r.second = mul(curr_h_r.second, pw2[1], mod2);
    curr_h_r.second = add(curr_h_r.second, mul(pw2[0], vals[u] - 'a' + 1, mod2), mod2);

    h[u] = curr_h;
    h_r[u] = curr_h_r;

    for(auto & v : adj[u])
    {
        if(v != p)
        {

            dfs(v, u, curr_h, curr_h_r, l + 1);
        }
    }
}


void Build(int n)
{
    dfs(1, -1, {0, 0}, {0, 0}, 0);

    for (int i = 1; i <= n; ++i) {
        UP[i][0] = parent[i];
    }

    for (int i = 1; 1<<i <= n ; ++i) {
        for (int j = 1; j <= n; ++j) {
            if(UP[j][i-1] == -1)
                UP[j][i] = -1;
            else
                UP[j][i] = UP[UP[j][i-1]][i-1];
        }
    }

}

int Get_Kth_Anc(int u, int k)
{
    for (int i = 30; i >= 0 ; --i) { // for integer k <= 1e9, make it 60 for bigger k
        if(k&(1<<i))
        {
            if(i >= (int)UP[u].size() || UP[u][i] == -1) return -1;
            u = UP[u][i];
        }
    }

    return u;
}

int Get_LCA(int u, int v)
{
    if(depth[u] < depth[v])
        swap(u, v);

    int k = depth[u] - depth[v];

    u = Get_Kth_Anc(u, k);

    if(u == v) return u;

    for (int i = (int)UP[u].size() - 1; i >= 0 ; --i) {
        if(UP[u][i] != UP[v][i])
        {
            u = UP[u][i];
            v = UP[v][i];
        }
    }

    return UP[u][0];
}



pair<int, int> Get_Hash(int u, int v)
{
    int common = Get_LCA(u, v);

    pair<int, int> ansL = {0, 0};
    ansL.first = add(ansL.first, h_r[u].first, mod1);
    ansL.second = add(ansL.second, h_r[u].second, mod2);

    int dist = depth[u] - depth[common];

    ansL.first = add(ansL.first, mul(-h_r[common].first, pw1[dist], mod1), mod1);
    ansL.second = add(ansL.second, mul(-h_r[common].second, pw2[dist], mod2), mod2);


    ansL.first = add(ansL.first, mul(pw1[dist], vals[common] - 'a' + 1, mod1), mod1);
    ansL.second = add(ansL.second, mul(pw2[dist], vals[common] - 'a' + 1, mod2), mod2);



    pair<int, int> ansR = {0, 0};
    ansR.first = add(ansR.first, h[v].first, mod1);
    ansR.second = add(ansR.second, h[v].second, mod2);

    int dist2 = depth[common];
    ansR.first = add(ansR.first, -h[common].first, mod1);
    ansR.second = add(ansR.second, -h[common].second, mod2);

    ansR.first = mul(ansR.first, inv1[dist2 + 1], mod1);
    ansR.second = mul(ansR.second, inv2[dist2 + 1], mod2);

    ansR.first = mul(ansR.first, pw1[dist + 1], mod1);
    ansR.second = mul(ansR.second, pw2[dist + 1], mod2);

    return {add(ansL.first, ansR.first, mod1), add(ansL.second, ansR.second, mod2)};
}
void solve()
{
    cin >> n;

    adj.assign(n + 1, {});
    h.resize(n + 1);
    h_r.resize(n + 1);
    vals.resize(n + 1);
    parent.resize(n + 1);
    depth.resize(n + 1);
    UP.assign(n + 1, vector<int>(__lg(n) + 5));

    for (int i = 1; i <= n; ++i) {
        char c; cin >> c;
        vals[i] = c;
    }

    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    Build(n);

    int q; cin >> q;
    while (q--)
    {
        int u, v; cin >> u >> v;
        cout << (Get_Hash(u, v) == Get_Hash(v, u)) << endl;
    }
}