#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

struct automat {
    int n;
    int k;

    vector< vector<int> > d;

    vector< vector<bool> > DP;
    vector< vector<pair<int, int> > > prev;

    int q0;

    vector<bool> F;

    vector<bool> viz;

    void dynamic(int len) {
        DP.resize(n);
        prev.resize(n);

        for(size_t i = 0; i < n; ++i) {
            DP[i].resize(len + 1);
            prev[i].resize(len + 1);
        }

        DP[q0][0] = 1;

        for(int i = 0; i < len; ++i) {
            for(int j = 0; j < n; ++j) {
                if(!DP[j][i])
                    continue;
                for(int t = 0; t < k; ++t) {
                    if(d[j][t] == -1)
                        continue;
                    int to = d[j][t];
                    DP[to][i + 1] = 1;
                    prev[to][i + 1] = make_pair(j, t);
                }
            }

        }
    }

    string reconstruct(int state, int len, string s = "") {
        if(!len) {
            reverse(s.begin(), s.end());
            return s;
        }

        s += char(prev[state][len].second + 'a');
        return reconstruct(prev[state][len].first, len - 1, s);
    }

    string wordLength(int len) {
        dynamic(len);
        
        for(int i = 0; i < n; ++i) {
            if(F[i] && DP[i][len] == 1) {
                return reconstruct(i, len);
            }
        }
        return "N/A";
    }

    vector<int> reachable() {
        viz.resize(n);
        for(size_t i = 0; i < n; ++i)
            viz[i] = 0;
        queue<int> Q;
        Q.push(q0);
        viz[q0] = 1;

        while(!Q.empty()) {
            const int now = Q.front();
            Q.pop();
            for(int i = 0; i < k; ++i) {
                int to = d[now][i];
                if(to == -1)
                    continue;
                if(viz[to])
                    continue;

                viz[to] = 1;
                Q.push(to);
            }
        }

        vector<int> ret;
        for(size_t i = 0; i < n; ++i)
            if(viz[i])
                ret.push_back(i);

        return ret;
    }

    void init(int n, int k, vector<vector<int> > d, int q, vector<int> f) {
        this->n = n;
        this->k = k;
        this->d = d;
        this->q0 = q;

        F.resize(n);
        for(size_t i = 0; i < n; ++i)
            this->F[i] = 0;

        for(size_t i = 0; i < f.size(); ++i)
            F[f[i]] = 1;
    }

    bool match(const int &q, const int &at, const vector<int> &w) {
        if((size_t)at == w.size())
            return F[q];

        if(d[q][w[at]] == -1)
            return false;
        else
            return match(d[q][w[at]], at + 1, w);
    }


    bool match(const vector<int> &w) {
        return match(q0, 0, w);
    }

};

automat a;

int main() {
    int n, k;
    cin >> n >> k;
    vector<vector<int> > d;

    d.resize(n);

    for(size_t i = 0; i < n; ++i) {
        d[i].resize(k);
        for(size_t j = 0; j < k; ++j)
            d[i][j] = -1;
    }

    int cnt;
    cin >> cnt;

    for(int i = 0; i < cnt; ++i) {
        int st, fin;
        char t;
        cin >> st >> fin >> t;
        d[st][t - 'a'] = fin;

    }

    int q;
    cin >> q;


    int sz;
    cin >> sz;
    vector<int> F;
    F.resize(sz);
    for(size_t i = 0; i < sz; ++i) {
        cin >> F[i];
    }

    a.init(n, k, d, q, F);

    string s;
    cin >> s;

    vector<int> w(s.length());

    for(size_t i = 0; i < s.length(); ++i)
        w[i] = s[i] - 'a';

    cout << s << ": "<< ( a.match(w) ? "MATCHED" : "DID NOT MATCH" ) << "\n";

    vector<int> viz = a.reachable();

    cout << "reachable states: \n";

    for(auto i: viz)
        cout << i << " ";
    cout << "\n";

    int qlen;
    cin >> qlen;

    cout << "testing for a word of lenth " << qlen << "\n";

    cout << a.wordLength(qlen) << "\n";


    return 0;
}
