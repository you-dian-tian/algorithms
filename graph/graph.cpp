//
// graph.cpp - Simple implementation of graphs. Can be used as
//    a template for more complex graph implementations.
//
// Implemented functions:
//
//   1. dfs and bfs;
//   2. cycle detection;
//   3. find strong components (for undirected graph)
//      and weak components (for directed graph);
//   4. topological sort (used to detect cycle in directed graph).
//
// History:
//    Created on 2015-8-2 by Yu Hengyang, hengyyu@gmail.com
//

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;


struct Edge {
	int weight;
	int to;
};

struct Vertex {
	int indegree;
	int outdegree;
	vector<Edge*> neighbors;
};


class Graph {
public:
	Graph(size_t n, bool directed):_nvertex(n), _directed(directed)
		{
			_vertex.reserve(n+1);
			_parent.reserve(n+1);

			for (size_t i = 0; i != _vertex.size(); ++i) {
				_vertex[i].indegree = _vertex[i].outdegree = 0;
				_parent[i] = -1;
			}
		}

	~Graph()
		{
			destroy();
		}


	void add_edge(int x, int y, int weight)
		{
			Edge *e = new Edge();

			e->weight = weight;
			e->to = y;
			_vertex[x].neighbors.push_back(e);
			if (_directed) {
				++_vertex[x].outdegree;
				++_vertex[y].indegree;
			}
		}


	void read_graph(istream& ins)
		{
			int x, y;

			while (ins >> x >> y) {
				add_edge(x, y, 0);
				if (!_directed)
					add_edge(y, x, 0);
			}
		}

	void bfs(int start)
		{
			if (start > 0 && start <= _nvertex)
				do_bfs(start);

			for (size_t i = 1; i <= _nvertex; ++i)
				if (!_processed[i])
					do_bfs(i);
		}


	void dfs(int v)
		{
			if (v <= _nvertex && v > 0)
				do_dfs(v);

			for (size_t i = 1; i <= _nvertex; ++i)
				if (!_processed[i])
					do_dfs(i);
		}


	// find strong-connected components for an undirected graph,
	// find weak-connected components for a directed graph.
	void find_components()
		{
			int ncomponent = 1;

			for (size_t i = 1; i <= _nvertex; ++i)
				if (!_discovered[i]) {
					cout << "component " << ncomponent++ << ": ";
					//
					// NOTE: choose whichever you like: dfs or bfs.
					//
					dfs(i);
					//bfs(i);
					cout << '\n';
				}
		}


	bool has_cycle()
		{
			bool cycle = false;

			if (_nvertex > 0) {
				unvisit();
				if (!_directed) {
					for (size_t i = 1; i <= _nvertex; ++i)
						if (!_discovered[i])
							cycle |= detect_undirected_cycle(i);
				} else
					cycle = detect_directed_cycle();
			}

			return cycle;
		}


	inline void unvisit()
		{
			// reset all bits to zeros
			_discovered.reset();
			_processed.reset();
		}

private:

	// returns true if a cycle is found in a undirect graph, false otherwise.
	bool detect_undirected_cycle(int v)
		{
			if (!_discovered[v]) {
				_discovered[v] = true;

				//
				// For a graph that has no cycles, each vertex
				// has exactly one parent.
				//
				for (auto &e: _vertex[v].neighbors) // loop through each edge starting from v
					if (!_discovered[e->to]) {
						_parent[e->to] = v;
						if (detect_undirected_cycle(e->to))
							goto found;
					} else if (_parent[v] != e->to)
						goto found;
			}
			return false;

		found:
			return true;
		}

	// returns true if a cycle is found in a directed graph, false otherwise.
	// do a topological sort.
	bool detect_directed_cycle()
		{
			stack<int> s;
			size_t nprocessed = 0;

			for (size_t i = 1; i <= _nvertex; ++i)
				if (0 == _vertex[i].indegree)
					s.push(i);

			while (!s.empty()) {
				int v = s.top();
				s.pop();

				for (auto &e: _vertex[v].neighbors)
					if (0 == --_vertex[e->to].indegree)
						s.push(e->to);
				++nprocessed;
			}

			return nprocessed != _nvertex;
		}


	void do_dfs(int v)
		{
			if (!_discovered[v]) {
				_discovered[v] = true;

				//
				// NOTE: change the following line to your need, whether a
				// function call or another expression statement.
				//
				cout << v << ' ';

				for (auto &e: _vertex[v].neighbors)
					do_dfs(e->to);

				_processed[v] = true;
			}
		}


	void do_bfs(int start)
		{
			queue<int> q;

			q.push(start);
			_discovered[start] = true;
			while (!q.empty()) {
				int x = q.front();

				for (auto &e: _vertex[x].neighbors)
					if (!_discovered[e->to]) {
						_discovered[e->to] = true;
						q.push(e->to);
					}

				_processed[x] = true;

				//
				// NOTE: change the following line to your need, whether a
				// function call or another expression statement.
				//
				cout << x << ' ';

				q.pop();
			}
		}


	void destroy()
		{
			for (size_t i = 0; i <= _nvertex; ++i) {
				for (auto &e: _vertex[i].neighbors)
					delete e;

				_vertex[i].neighbors.clear();
			}
			_vertex.clear();
			_parent.clear();
		}


private:
	enum { MAX_VERTEX = 10000 };

	vector<Vertex> _vertex;
	vector<int> _parent;
	bitset<MAX_VERTEX+1> _discovered; // mark vertex when the first time we encounter it
	bitset<MAX_VERTEX+1> _processed;  // mark vertex when all its neighbors have been processed
	size_t _nvertex;	       // number of total vertexes of graph
	bool _directed;		       // true for directed graph, false otherwise.
};



int main()
{
	//
	// UNDIRECTED GRAPH TEST
	//
	// int n;
	// cin >> n;
	// Graph *g = new Graph(n, false);

	// g->read_graph(cin);

	// cout << "bfs: ";
	// g->bfs(1);

	// g->unvisit();
	// cout << "\ndfs: ";
	// g->dfs(1);
	// cout << '\n';

	// g->unvisit();
	// g->find_components();

	// if (g->has_cycle())
	// 	cout <<"Cycle detected.\n";

	// delete g;


	//
	// DIRECTED GRAPH TEST
	//
	int n;
	cin >> n;
	Graph *g = new Graph(n, true); // construct a directed graph with n vertexes.

	g->read_graph(cin);

	cout << "bfs: ";
	g->bfs(n/2);

	g->unvisit();
	cout << "\ndfs: ";
	g->dfs(n/2);
	cout << '\n';

	g->unvisit();
	g->find_components();

	if (g->has_cycle())
		cout <<"Cycle detected.\n";

	delete g;

	return 0;
}