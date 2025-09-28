const int N = 1e5 + 5;
vi adj[N];
bool vis[N];
bool stacked[N];
int n,m;
bool can = 1;
stack<int> ans;

void topo(int u)
{
    vis[u] = 1;
    for(auto v : adj[u])
    {
        if(!vis[v])
        {
            topo(v);
        }
        else if(!stacked[v])
        {
            can = 0;
        }
    }
    ans.push(u);
    stacked[u] = 1;
}