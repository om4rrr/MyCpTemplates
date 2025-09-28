// The Graph must be DAG

const int N = 1e6 + 5, OO = 1e18 + 5;

vector<int> adj[N];
int inDegree[N], vis[N];

int n, m;

void bfs_topo()
{
    queue<int>q; // use priority_queue to get the min sort or max

    for (int i = 1; i <= n; ++i) {
        if(!inDegree[i])
        {
            vis[i] = 1;
            q.push(i);
        }

    }

    vector<int>topo;

    while(!q.empty())
    {
        int u = q.front();
        q.pop();

        topo.push_back(u);

        for(auto v : adj[u])
        {
            if(!vis[v])
            {
                inDegree[v]--;

                if(!inDegree[v])
                {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
    }

    if(topo.size() != n)
        cout << -1 << '\n';
    else {
        for(auto node : topo)
            cout << node << ' ';
    }
}