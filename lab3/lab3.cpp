#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <string>
#include <utility>

#define end(x, y) (make_pair(x,  y))

using std::cerr;
using std::string;
using std::pair;
using std::make_pair;

const char lambda = '#';


struct node {
  static int qAt;
  int q;
  std::unordered_multimap<char, node*> T;

  node() {
    q = qAt;
    qAt++;
  }

  node* addTransition(char c, node* n) {
   // cerr << "connected " << q << " + " << c << " => " << n->q <<  "\n";
    T.insert(make_pair(c, n));
    return n;
  }

  node* addTransition(char c) {
    node* add = new node();
    return addTransition(c, add);
  }
};

typedef pair<node*, node*> pnn;

int node::qAt = 0;

string s;

int i = 0;

pnn parse();
pnn concat();

pnn cycle(node* st, node* fin) {
  fin->addTransition(lambda, st);
  i++;
  return make_pair(st, fin);
}

pnn concat() {

 // cerr << "entering concat\n";

  node* st = new node();
  node* now = st;

  while(i < s.length()) {
    if(s[i] == '(') {
      i++;
      pnn next = parse();
      now->addTransition(lambda, next.first);
      now = next.second;
    } else 
    if(s[i] == ')') {
     // cerr << "ending concat because of )\n";
      return end(st, now);
    } else
    if(std::isalpha(s[i])) {
      now = now->addTransition(s[i]);
      i++;
    } else
    if(s[i] == '|') {
     // cerr << "end concat because of |\n";
      return end(st, now);
    } else
    if(s[i] == '*') {
      now->addTransition(lambda, st);
      now = st;
      i++;
    }
  }

 // cerr << "end concat\n";
  return end(st, now);
}

pnn parse() {
//returns the start node and the end node
  std::vector<pnn> nxt;
 // cerr << "entering parse\n";

  while(i < s.length()) {
    if(s[i] == ')') {
     // cerr << "found )\n";
      i++;
      break;
    } else
    if(s[i] == '(') {
     // cerr << "found (\n";
      i++;
      nxt.push_back(parse());
    } else
    if(s[i] == '|') {
     // cerr << "found |\n";
      i++;
      nxt.push_back(concat());
    } else
    if(s[i] == '*') {
     // cerr << "found * in parse\n";
      nxt.back().second->addTransition(lambda, nxt.back().first);
      nxt.back().second = nxt.back().first;
      i++;
    } else {
      pnn next = concat();
     // cerr << "extending nxt with concat\n";
      if(!nxt.empty()) {
        nxt.back().second->addTransition(lambda, next.first);
        nxt.back().second = next.second;
      } else {
        nxt.push_back(next);
      }
    }
  } 

  node* st = new node();
  node* fin = new node();

 // cerr << "nxt has size " << nxt.size() << "\n";

  for(auto i: nxt) {
    st->addTransition(lambda, i.first);
    i.second->addTransition(lambda, fin);
  }

 // cerr << "return from parse\n";

  return make_pair(st, fin);
}

std::unordered_set<int> visited;

int fin;

struct edge {
  int a, b;
  char c;

  edge() = delete;

  edge(int aa, char cc, int bb):
    a(aa),
    b(bb),
    c(cc) {
    }
};

int mmap[100];

std::vector<edge> edges;

void show(node* st) {
  visited.insert(st->q);
  for(auto i: st->T) {
    edges.emplace_back(st->q, i.first, i.second->q);
  }
  for(auto i: st->T) {
    if(!visited.count(i.second->q)) {
      show(i.second);
    }
  }
}

int main() {
  std::cin >> s;
  pnn automat = parse();

  fin = automat.first->q;
  show(automat.first);

  for(int i = 0; i < node::qAt; ++i)
    mmap[i] = i;

  mmap[fin] = 0;
  mmap[0] = fin;

  std::cout << node::qAt << "\n";
  std::cout << 0 << "\n";
  std::cout << edges.size() << "\n";
  for(auto &i: edges) {
    std::cout << mmap[i.a] << " " << mmap[i.b] << " " << i.c << "\n";
  }
  std::cout << "1\n" << automat.second->q << "\n";
  return 0;
}
