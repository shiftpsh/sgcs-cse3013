#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>

#include "Solver.h"
#define strdup _strdup

int currCase;
int cases;
std::vector<std::unordered_map<int, std::vector<int>>> graphs; // from, to

void Init() {
	currCase = 0;
	cases = 0;
	graphs.clear();
}

void Read_File(const char* filename) {
	std::string _filename = std::string(filename);
	std::ifstream fin;
	fin.open(_filename, std::ios::in);

	fin >> cases;
	graphs.resize(cases);

	for (int tc = 0; tc < cases; tc++) {
		int n; // edge count
		fin >> n;

		auto& graph = graphs[tc];

		for (int i = 0; i < n; i++) {
			int u, v; // edge u --> v
			fin >> u >> v;
			graph[u].emplace_back(v);
		}
	}

	fin.close();
}

char* Check_Tree(int tc) {
	auto& graph = graphs[tc - 1];
	bool flag = true;

	std::unordered_map<int, int> indegree;
	std::unordered_map<int, bool> visit;

	// initializing visit map:
	// check for occuring nodes and set them to false
	// and calculate indegrees
	for (auto& kv : graph) {
		// kv = key (vertex u), value (vector of all adjacent vertices of u)
		int u = kv.first;
		indegree[u] = 0;
		visit[u] = false;
		for (int v : kv.second) {
			indegree[v] = 0;
			visit[v] = false;
		}
	}
	for (auto& kv : graph) {
		// kv = key (vertex u), value (vector of all adjacent vertices of u)
		int u = kv.first;
		for (int v : kv.second) {
			indegree[v]++;
		}
	}

	// check indegree counts
	// vertex of indegree count 0 must be 1 and
	// vertex of indegree count > 1 must be 0 to form a tree
	int indegree_0_count = 0;
	int root = -1;

	for (auto& kv : indegree) {
		// kv = key (vertex u), value (indegree of vertex u)
		if (kv.second == 0) {
			root = kv.first;
			indegree_0_count++;
		}
		else if (kv.second != 1) {
			flag = false;
		}
	}

	if (indegree_0_count != 1) {
		flag = false;
	}

	if (flag) {
		// breadth first search;
		// if the graph is not connected graph,
		// it is definitely not a tree
		std::queue<int> q;
		q.emplace(root);
		visit[root] = true;

		while (q.size()) {
			int u = q.front();
			q.pop();

			for (int v : graph[u]) {
				if (visit[v]) continue;
				visit[v] = true;
				q.emplace(v);
			}
		}

		// check for all nodes and see if
		// the graph is fully connected or not
		for (auto& kv : visit) {
			// kv = key (vertex u), value (visited u or not)
			if (!kv.second) {
				flag = false;
				break;
			}
		}
	}

	std::ostringstream rout; // result string stream
	if (flag) {
		rout << "Case " << tc << " is a tree." << std::endl;
	}
	else {
		rout << "Case " << tc << " is not a tree." << std::endl;
	}

	return strdup(rout.str().c_str());
}

void Error_Exit(char* s) {
	printf("%s\n", s);
	exit(-1);
}