// BT20CSE065 - Amaan Khan
// Assignment 2 - Travelling Salesman Problem approximation by A star algorithm using MST heuristics
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <queue>
#include <cmath>
#include <map>
using namespace std;

// structure definition for each node in the state space tree
struct Node {
	int cost, bound;		// total cost to reach current node, bound is the MST heuristic value
	vector<int> path;		// path from the start node to the current node
	map<int, int> visited;	// for faster search of visited nodes
};

// for comparison of nodes when storing in min heap
struct MinHeapNode {
	Node data;
	bool operator > (const MinHeapNode& x) const {
		return data.bound > x.data.bound;
	}
};

// Edge definition
struct Edge {
	int src, dest, weight;
};

// comparison function for sorting all edges
bool compareEdges(const Edge& a, const Edge& b) {
	return a.weight < b.weight;
}

// For Kruskal's algorithm, we will need to know whether a new edge forms a cycle in the graph
// A new edge will form a cycle if both its vertices belong to the same component
// For analysis of different components we use disjoint set data structure
class DisjointSet {
private:
	vector<int> parent, rank;	// stores the immediate ancestor of each node, and rank - distance of the highest ancestor in the set to the furthest child

public:
	// Initialisation
	DisjointSet(int n) {
		parent.resize(n);
		rank.resize(n);
		for (int i = 0; i < n; i++) {
			parent[i] = i;
			rank[i] = 0;
		}
	}

	// Find the highest ancestor of the node
	int findParent(int u) {
		if (u != parent[u])
			parent[u] = findParent(parent[u]);
		return parent[u];
	}

	// Merge two different sets
	// Smaller sets highest ancestor is attached to the larger set's
	void unionSets(int u, int v) {
		int pu = findParent(u);
		int pv = findParent(v);

		if (pu == pv)
			return;

		if (rank[pu] < rank[pv])
			parent[pu] = pv;
		else if (rank[pu] > rank[pv])
			parent[pv] = pu;
		else {
			parent[pv] = pu;
			rank[pu]++;
		}
	}
};

vector<vector<int>> graph;
vector<Edge> edges;
int kruskalsMST(Node& x);

Node tsp_using_astar(int start) {
	// Using a min heap to access the state with the minimum bound first
	priority_queue<MinHeapNode, vector<MinHeapNode>, greater<MinHeapNode>> pq;
	Node v;
	v.path.push_back(start);
	v.cost = 0;
	v.bound = 0;
	v.visited[start] = true;
	pq.push({v});

	bool goal_reached = false;

	while (!goal_reached) {
		MinHeapNode top = pq.top();		// Choose the state with the minimum bound for expansion
		pq.pop();
		cout << top.data.bound << " ";

		Node u = top.data;
		int current = u.path.back();

		if (current == start && u.visited[current] == false) {
			// After visiting all the nodes we have reached the start
			goal_reached = true;
			v = u;
		}
		else {
			u.visited[current] = true;

			// After visiting all nodes now we have to go back to start node so it is marked unvisited
			if (u.path.size() == graph.size()) {
				u.visited[start] = false;
			}

			// consider all the adjacent unvisited nodes as successors for the last visited node
			for (int i = 0; i < graph.size(); i++) {
				if (u.visited[i] == false) {
					Node successor = u;
					successor.cost += graph[current][i];
					successor.path.push_back(i);
					successor.bound = successor.cost + kruskalsMST(successor);
					pq.push({successor});
				}
			}
		}
	}
	return v;
}

// Finding Minimum spanning tree of the remaining nodes in the graph using Kruskal's algorithm
int kruskalsMST(Node& x) {
	DisjointSet ds(graph.size());

	int h_value = 0;

	// consider all edges in the graph for which none of the nodes are visited before
	for (const Edge& edge : edges) {
		int u = edge.src, v = edge.dest;
		if (!x.visited[u] && !x.visited[v]) {
			int setU = ds.findParent(u);
			int setV = ds.findParent(v);

			// If they don't belong to the same set then merge their sets
			if (setU != setV) {
				h_value += edge.weight;
				ds.unionSets(setU, setV);
			}
		}
	}
	return h_value;
}

int32_t main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int n;
	cin >> n;
	graph.resize(n, vector<int> (n, INT_MAX));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> graph[i][j];
			if (j > i) edges.push_back({i, j, graph[i][j]});
		}
	}
	// Sort all edges for using Kruskal's algorithm
	sort(edges.begin(), edges.end(), compareEdges);

	int start = 0;
	Node p = tsp_using_astar(start);
	cout << "Minimum Cost is " << p.cost << "\n";
	cout << "Path: ";

	p.path.pop_back();
	// Printing the path with minimum cost
	for (int x : p.path) {
		cout << x << "->";
	}
	cout << start << "\n";

	return 0;
}