struct query{
    int l, r, idx;
};

vector<query> queries;
vector<int>v, queries_ans;

int n, q, ans;



void add(int x)
{
    // write here the logic you want to happen if you add an element to the range
}

void del(int x)
{
    // write here the logic you want to happen if you remove an element from the range
}

void MO()
{
    int sq = ceil(sqrt(n)) + 1;
    sort(queries.begin(), queries.end(),
         [&](const query &A, const query &B) -> bool{
             return make_pair(A.l /sq, A.r) < make_pair(B.l / sq, B.r);
    });

    int l = queries[0].l, r = queries[0].l;
    add(v[queries[0].l]);

    for(const auto &[lq, rq, iq] : queries)
    {
        while(rq > r)
        {
            add(v[++r]);
        }

        while(lq > l)
        {
            del(v[l++]);
        }

        while(l > lq)
        {
            add(v[--l]);
        }

        while(r > rq)
        {
            del(v[r--]);
        }

        queries_ans[iq] = ans;
    }

}


void solve() {

    cin >> n;
    v = vector<int>(n);
    for(int &x : v)
        cin >> x;

    cin >> q;
    queries = vector<query>(q);
    for (int i = 0; i < q; ++i) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].l--, queries[i].r--;
        queries[i].idx = i;
    }
    queries_ans = vector<int>(q);
    MO();
    for(int &i : queries_ans)
        cout << i << '\n';
}