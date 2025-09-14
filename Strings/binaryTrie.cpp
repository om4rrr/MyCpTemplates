struct binaryTrie
{
    struct Node
    {
        int pref;
        int childs[2];
        Node(){
            pref = 0;
            childs[0] = childs[1] = -1;
        }
    };

    vector<Node> trie;

    binaryTrie()
    {
        trie.push_back(Node());
    }

    void addNode(int & num)
    {
        bitset<32> bits = num;
        int curr = 0;
        for (int i = 31; i >= 0; --i) {
            if(trie[curr].childs[bits[i]] == -1)
            {
                trie.push_back(Node());
                trie[curr].childs[bits[i]] = trie.size() - 1;
            }
            curr = trie[curr].childs[bits[i]];
            ++trie[curr].pref;
        }
    }

    int solve(int & num)  // Calc Minimum
    {
        bitset<32> bits = num;
        int curr = 0, ans = 0;
        for (int i = 31; i >= 0; --i) {
            int toSame = trie[curr].childs[bits[i]];
            int toOpposite = trie[curr].childs[!bits[i]];
            if(toSame != -1 && trie[toSame].pref)
            {
                curr = toSame;
            }
            else
            {
                ans |= (1 << i);
                curr = toOpposite;
            }
            --trie[curr].pref;
        }
        return ans;
    }

};