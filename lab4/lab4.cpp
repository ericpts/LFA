#include <string>
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

#define DEBUG

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define debug(...) if(DEBUG_TEST)fprintf(stderr, __VA_ARGS__)

const string lambda = "#";

const int MAX_N = 100;

int n;

string A[MAX_N][MAX_N];
string B[MAX_N];

string concat(string a, string b) {
	if(a.empty() || b.empty())
		return "";

	if(a == lambda) 
		return b;
	
	if(b == lambda)
		return a;

	return a + b;
}

template<typename... Args>
string concat(string a, string b, Args... args) {
	string r = concat(a, b);
	return concat(r, args...);
}

void norm(int x) {
	if(A[x][x].empty()) {
		return;
	}

	for(int i = 0; i < n; ++i) {
		if(i == x)
			continue;
		A[x][i] = concat("(" + A[x][x] + ")*", A[x][i]);
	}

  B[x] = concat("(" + A[x][x] + ")*", B[x]);


	A[x][x].clear();
}

void addEdge(int f, int t, char c) {
	string s;
	s += c;
	if(A[f][t].empty())
		A[f][t] = s;
	else
		A[f][t] += "|" + s;
}

void addFin(int x) {
	if(B[x].empty())
		B[x] += lambda;
}


string add(string s, string b) {
	if(s.empty())
		return b;
	if(b.empty())
		return s;
	return "(" + s + "|" + b + ")";
} 

void dbg() {
  for(int i = 0; i < n; ++i) {
    debug("B[%d] = %s\n", i, B[i].c_str());
  }
  debug("\n");

  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      debug("A[%d][%d] = %s\n", i, j, A[i][j].c_str());
    } 
  }

  debug("\n\n\n");
}

void mul(int dest, int src) {

	B[dest] = add(B[dest], concat(A[dest][src], B[src]));
	for(int i = 0; i < src; ++i) {
		A[dest][i] = add(A[dest][i], concat(A[dest][src], A[src][i]));
	}
}

int main() {
	cin >> n;
	int q0;
	cin >> q0;

	assert(q0 == 0);

	int m;
	cin >> m;

	for(int _ = 0; _ < m; ++_) {
		int i, j;
		char c;
		cin >> i >> j >> c;
		addEdge(i, j, c);
	}

	int nfin;
	cin >> nfin;
	for(int i = 0; i < nfin; ++i) {
		int q;
		cin >> q;
		addFin(q);
	}

	for(int i = n - 1; i > 0; --i, --n) {
	//one by one,
	//disolve the states

    debug("Disolving by %d\n", i);
		norm(i);
    dbg();

    debug("Merging states\n");

		for(int j = i - 1; j >= 0; --j) 
			mul(j, i);
    dbg();
	}

  norm(0);

  dbg();

  cout << B[0] << "\n";

	return 0;
}
