// JohnsonsAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>

#include "fibonacciheap.h"

using namespace std;

static const int debug = 0;

static const int kVmax = 8;
static const int kInfinity = 100000000;

typedef vector<vector<int>> Matrix;

ostream& operator << (ostream& out, Matrix mat) {
	for (size_t i = 1; i < mat.size(); ++i) {
		for (size_t j = 1; j < mat[i].size(); ++j) {
			if (mat[i][j] == kInfinity)
				out << setw(15) << "inf";
			else
				out << setw(15) << mat[i][j];
		}
		out << endl;
	}
	return out;
}
ostream& operator << (ostream& out, vector<int> v) {
	for (size_t i = 1; i < v.size(); ++i) {
		if (v[i] == kInfinity)
			out << setw(15) << "inf";
		else
			out << setw(15) << v[i] << " ";
	}
	out << endl;
	return out;
}
bool operator==(vector<vector<int>>& a, vector<vector<int>>& b) {
	if (a.size() != b.size())
		return false;
	for (size_t i = 1; i < a.size(); ++i) {
		if (a[i].size() != b[i].size())
			return false;
		for (size_t j = 1; j < a[i].size(); ++j)
			if (a[i][j] != b[i][j])
				return false;
	}
	return true;
}

class Graph {
public:
	Graph() {}
	void add_edge(int from, int to, int weight) {
		adjacency_list_[from].push_back(make_pair(to, weight));
	}
	bool has_vertex(int v) {
		return adjacency_list_[v].size();
	}
	void set_weight(int from, int to, int weight) {
		for (size_t i = 0; i < adjacency_list_[from].size(); ++i)
			if (adjacency_list_[from][i].first == to) {
				adjacency_list_[from][i].second = weight;
			}
	}
	vector<pair<int, int>> get_adjacent(int v) {
		return adjacency_list_[v];
	}
	friend ostream& operator << (ostream& out, const Graph& graph) {
		for (int i = 1; i < kVmax; ++i) {
			if (!graph.adjacency_list_[i].empty()) {
				out << i << ": ";
				for (auto edge : graph.adjacency_list_[i]) {
					//(v, w), 
					out << edge.first << "(" << edge.second << ") ";
				}
				out << endl;
			}
		}
		return out;
	}

private:
	vector<pair<int, int>> adjacency_list_[kVmax]; //to, weight
};

vector<int> DijkstraAlgorithm(Graph& graph, int start) {
	if (debug) { cout << "entered dijkstra\n"; }
	vector<int> distance(kVmax, kInfinity);
	distance[start] = 0;
	FibonacciHeap<int, int> queue;
	queue.insert(distance[start], start);

	while (!queue.empty()) {
		int min_vertex = queue.extract_min().second;
		if (debug) { cout << "min extracted\n"; }

		vector<pair<int, int>> adjacency_vertices = graph.get_adjacent(min_vertex);
		for (pair<int, int> edge : adjacency_vertices) {
			int to = edge.first;
			int w = edge.second;
			assert(w >= 0);
			if (distance[min_vertex] < kInfinity && distance[min_vertex] + w < distance[to]) {
				distance[to] = distance[min_vertex] + w;
				queue.insert(distance[to], to);
			}
		}
	}

	return distance;
}

vector<int> BellmannFordAlgorithm(Graph& graph, int start) {
	vector<int> distance(kVmax, kInfinity);
	distance[start] = 0;
	int number_of_iterations = 0;
	for (;;)
	{
		if (number_of_iterations > kVmax) {
			return vector<int>();
		}
		number_of_iterations++;
		bool run_next = false;
		for (int v = 0; v < kVmax; ++v) {
			vector<pair<int, int>> adjacent_vertices = graph.get_adjacent(v);
			for (pair<int, int> edge : adjacent_vertices) {
				int to = edge.first;
				int weight = edge.second;
				if (distance[v] < kInfinity && distance[v] + weight < distance[to]) {
					distance[to] = distance[v] + weight;
					run_next = true;
				}
			}
		}
		if (!run_next)
			break;
	}
	return distance;
}

bool hasNegativeCycle(Graph& graph) {
	//TODO implement
	return BellmannFordAlgorithm(graph, 0).empty();
	//return false;
}

Matrix JohnsonsAlgorithm(Graph& graph) {
	Matrix matrix_distance(kVmax, vector<int>(kVmax, kInfinity));
	Graph graph_helper(graph);
	for (int i = 1; i < kVmax; ++i) {
		graph_helper.add_edge(0, i, 0);
	}
	if (hasNegativeCycle(graph)) {
		return Matrix();
	}
	if (debug) {
		cout << "log: does not have negative cycle\n";
		cout << "graph helper:\n";
		cout << graph_helper << '\n';
	}
	vector<int> vector_distance = BellmannFordAlgorithm(graph_helper, 0);
	if (debug) {
		cout << "vector distance:\n";
		cout << vector_distance << "\n";
	}
	if (vector_distance.empty()) {
		if (debug) { cout << "LOG: HAS NEGATIVE CYCLE\n"; }
		return Matrix();
	}
	if (debug) { cout << "log: bellmann ford ran from 0-vertex\n"; }
	vector<int> h(kVmax, kInfinity);
	for (int i = 0; i < kVmax; ++i) {
		h[i] = vector_distance[i];
	}
	if (debug) {
		cout << "log: h array filled\n";
		cout << h << "\n";
	}
	for (int v = 1; v < kVmax; ++v) {
		vector<pair<int, int>> adjacency_vertices = graph.get_adjacent(v);
		for (pair<int, int> edge : adjacency_vertices) {
			int to = edge.first;
			int weight = edge.second;
			//TODO do it in O(1)
			graph_helper.set_weight(v, to, weight + h[v] - h[to]);
			//graph_helper[v][to] = weight + h[v] - h[to];
		}
	}
	if (debug) { cout << "log: graph helper filled with distances\n"; }
	Matrix final_distance(kVmax, vector<int>(kVmax, kInfinity));
	if (debug) { cout << graph_helper << "\n"; }
	for (int v = 1; v < kVmax; ++v) {
		if (debug) { cout << "dijkstra " << v << " "; }
		vector<int> distance_in_helper_graph = DijkstraAlgorithm(graph_helper, v);
		if (debug) {
			cout << "finished\n";
		}
		for (int u = 1; u < kVmax; ++u) {
			if (distance_in_helper_graph[u] != kInfinity) {
				final_distance[v][u] = distance_in_helper_graph[u] + h[u] - h[v];
			}
		}
	}
	if (debug) {
		cout << "log: final distances calculated\n";
	}
	return final_distance;
}

Matrix FloydWarshallAlgorithm(Graph& graph) {
	Matrix distance(kVmax, vector<int>(kVmax, kInfinity));
	for (int i = 0; i < kVmax; ++i) {
		for (pair<int, int> edge : graph.get_adjacent(i)) {
			distance[i][edge.first] = edge.second;
		}
	}
	for (int i = 0; i < kVmax; ++i) {
		distance[i][i] = 0;
	}
	for (int k = 0; k < kVmax; ++k) {
		for (int i = 0; i < kVmax; ++i) {
			for (int j = 0; j < kVmax; ++j) {
				if (distance[i][k] < kInfinity && distance[k][j] < kInfinity)
					if (distance[i][j] > distance[i][k] + distance[k][j]) {
						distance[i][j] = distance[i][k] + distance[k][j];
					}
			}
		}
	}
	return distance;
}

void read(Graph& graph, string filename = "input.txt") {
	ifstream in;
	in.open(filename);
	int u, v, w;
	while (in >> u >> v >> w) {
		graph.add_edge(u, v, w);
	}
}


int main()
{
	Graph graph;
	ofstream log;
	log.open("log.gv");
	log << "digraph G {\n";

	while (true) {
		cout << "1-insert edge, 2-run johnson, 3-show graph, 4-insert many edges, 5-clear graph, 6-exit\n";

		int key;
		cin >> key;
		switch (key) {
		case 1:
		{
			cout << "enter from,to,weight:";
			int v, u, w;
			cin >> v >> u >> w;
			cout << "added " << v << " " << u << "   " << w << "\n";
			log << v << " -> " << u << "[label = \"" << w << "\"]" << "\n";
			graph.add_edge(v, u, w);
			break;
		}
		case 2:
		{
			cout << "calculating shortest paths...\n";
			Matrix m = JohnsonsAlgorithm(graph);
			if (m.empty()) {
				cout << "has negative cycle\n";
			}
			else {
				cout << m << "\n";
				Matrix t = FloydWarshallAlgorithm(graph);
				if (m == t) {
					cout << "true\n";
				}
				else {
					cout << "false\n";
					cout << t << "\n\n";
				}
			}
			break;
		}
		case 3:
		{
			cout << "showing graph...\n";
			cout << graph << "\n";
			break;
		}
		case 4:
		{
			int amount;
			cout << "enter amount of edges to be added...\n";
			cin >> amount;
			for (int i = 0; i < amount; ++i) {
				int v, u, w;
				v = rand() % (kVmax - 1) + 1;
				u = rand() % (kVmax - 1) + 1;
				w = rand() - (RAND_MAX >> 4);
				w = w % 100;
				log << v << " -> " << u << "[label = \"" << w << "\"]" << "\n";
				cout << "added " << v << " " << u << "   " << w << "\n";
				graph.add_edge(v, u, w);
			}
			break;
		}
		case 5: {
			graph = Graph();
			log.close();
			log.open("log.gv");
			break;
		}
		default: {
			log << "}\n";
			log.close();
			exit(0);
		}
		}
	}

	return 0;
}

