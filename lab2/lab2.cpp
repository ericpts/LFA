/** Input:
 * N, Sigma, Q0
 * M, the number of edges
 * M edges of the form Qx Qy C
 * C == L => Lambda
 *
 * F, the number of final states
 * F number, the final states
 *
 */
#include <bits/stdc++.h>

using namespace std;

const int lambda = 0;
const int MAX_N = 20;
const int MAX_K = 100;

struct autmat{
	bool go[MAX_N][MAX_N][MAX_K];
	bool fin[MAX_N];
	int n;
	int k;
	int q0;

	void addEdge(int x, int sig, int y) {
		go[x][y][sig] = 1;
	}
	
	void addFin(int x) {
		fin[x] = 1;
	}

	void init(int n, int k, int q0) {
		this->n = n;
		this->k = k;
		this->q0 = q0;
	}
	
	void lambdaClose() {
		for(int k = 0; k < n; ++k) {
			for(int i = 0; i < n; ++i) {
				for(int j = 0; j < n; ++j) {
					if(go[i][k][0] && go[k][j][0])
						go[i][j][0] = 1;
				}
			}
		}
	}

	int take[1 << MAX_N][MAX_K];

	bool nfin[1 << MAX_N];

	int nq0;

	string stateBin(int s) {
		stringstream ret;
		ret << "{";
		for(int i = 0; i < n; ++i) {
			if(s & (1 << i))
				ret << i << ", ";
		}
		return ret.str().substr(0, ret.str().length() - 2) + "}";
	}

	void generateDeterminist() {
		int q = 1 << q0;
		
		for(int i = 0; i < n; ++i) {
			if(go[q0][i][0])
				q |= 1 << i;
		}
		queue<int> Q;
		Q.push(q);
		nfin[q] = 1;

		nq0 = q;
		
		while(!Q.empty()) {
			q = Q.front(); Q.pop();


			for(int s = 1; s < k; ++s) {
				int next = 0;
				for(int i = 0; i < n; ++i) {
					if(q & (1 << i)) {
						for(int j = 0; j < n; ++j) {
							if(go[i][j][s]) {
								next |= 1 << j;	
							}
						}
					}
				}

				for(int i = 0; i < n; ++i) 
					if(next & (1 << i))
					for(int j = 0; j < n; ++j)
						if(go[i][j][0])
							next |= 1 << j;
						


				if(!nfin[next]) {
					nfin[next] = 1;
					Q.push(next);
				}

				take[q][s] = next;

			}
			
		}

		for(int i = 0; i < (1 << n); ++i) {
			nfin[i] = 0;
			for(int j = 0; j < n; ++j) {
				if( (i & (1 << j)) && fin[j])
					nfin[i] = 1;
			}
		}

		for(int i = 0; i < (1 << n); ++i) {
			for(int j = 1; j < k; ++j) {
				if(take[i][j]) {
					cout << stateBin(i) << " +  " << char(j + 'a' - 1) << " => " << stateBin(take[i][j]) << "\n";
				}
			}
		}

	}
	
} aut;


int main() {
	int n, k, q0;
	cin >> n >> k >> q0;
	aut.init(n, k, q0);
	
	int m;
	cin >> m;
	int a, b, c;
	char s;
	for(int i = 0; i < m; ++i) {
		cin >> a >> b >> s;
		if(s == 'L')
			c = 0;
		else
			c = s - 'a' + 1;
		aut.addEdge(a, c, b);
	}

	int fins;
	
	cin >> fins;

	for(int i = 0; i < fins; ++i) {
		cin >> a;
		aut.addFin(a);
	}
	aut.lambdaClose();
	aut.generateDeterminist();
	return 0;
}
