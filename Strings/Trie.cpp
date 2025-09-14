struct Trie{

    struct Node
    {
        int pref, end;
        int childs[26];
        Node(){
            pref = 0, end = 0;
            memset(childs, -1, sizeof childs);
        }
    };

    vector<Node> trie;

    Trie()
    {
        trie.push_back(Node());
    }

    void addNode(string & s)
    {
        int curr = 0, n = s.size();
        for (int i = 0; i < n; ++i) {
            if(trie[curr].childs[s[i] - 'a'] == -1)
            {
                trie.push_back(Node());
                trie[curr].childs[s[i] - 'a'] = (int)trie.size() - 1;
            }
            curr = trie[curr].childs[s[i] - 'a'];
            ++trie[curr].pref;
        }
        ++trie[curr].end;
    }

    int countPref(string & s)
    {
        int curr = 0, n = s.size(), ans = 0;
        for (int i = 0; i < n; ++i) {
            if(trie[curr].childs[s[i] - 'a'] == -1)
            {
                return 0;
            }
            curr = trie[curr].childs[s[i] - 'a'];

        }
        return trie[curr].pref;
    }

    int countEnd(string & s)
    {
        int curr = 0, n = s.size(), ans = 0;
        for (int i = 0; i < n; ++i) {
            if(trie[curr].childs[s[i] - 'a'] == -1)
            {
                return 0;
            }
            curr = trie[curr].childs[s[i] - 'a'];

        }
        return trie[curr].end;
    }

};