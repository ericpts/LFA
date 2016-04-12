#include <string>
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

typedef string regex;

const regex lambda = "#";

const int MAX_N = 100;

int n;

regex A[MAX_N][MAX_N];
regex B[MAX_N];

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

	for(int i = n - 1; i > 0; --i) {
	//one by one,
	//disolve the states
		norm(i);
		for(int j = i - 1; j >= 0; --j) 
			mul(j, i);
	}

	return 0;
}
