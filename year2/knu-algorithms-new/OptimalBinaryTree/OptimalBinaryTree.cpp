// http://webgraphviz.com/
#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static const int debug = 0;

static const string kDefaultFileName = "output.txt";
static const string kKeyPrintfSpecifier = "%d";
static const int kNmax = 100;
static const int kInfinity = 1000000000;

template<typename TKey, typename TValue>
struct Data {
	TKey key;
	int index;
	TValue value;

	explicit Data(TKey key = TKey(), TValue value = TValue()) :key(key), value(value) {}
};

template<typename TKey, typename TValue>
struct Node : Data<TKey, TValue> {
	Node* left;
	Node* right;

	explicit Node(TKey key = TKey(), TValue value = TValue(), Node* left = nullptr, Node* right = nullptr) : Data<TKey, TValue>(key, value), left(left), right(right) {}
};

template<typename TKey, typename TValue>
class OptimalBinaryTree {
public:
	OptimalBinaryTree() {
		root = nullptr;
	}
	void build(vector<Data<TKey, TValue>>& data, vector<double>& probs_smart, vector<double>& probs_dummy) {
		root = nullptr;
		int n = (int)data.size() - 1;

		double e[kNmax][kNmax] = { 0 };
		int root_array[kNmax][kNmax] = { 0 };
		double w[kNmax][kNmax] = { 0 };
		for (int i = 1; i <= n + 1; ++i) {
			e[i][i - 1] = probs_dummy[i - 1];
			w[i][i - 1] = probs_dummy[i - 1];
		}

		for (int d = 1; d <= n; ++d) {
			for (int i = 1; i <= n - d + 1; ++i) {
				int j = i + d - 1;
				e[i][j] = kInfinity;
				w[i][j] = w[i][j - 1] + probs_smart[j] + probs_dummy[j];
				for (int r = i; r <= j; ++r) {
					double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
					if (t < e[i][j]) {
						e[i][j] = t;
						root_array[i][j] = r;
					}
				}
			}
		}

		int globalRootId = root_array[1][n];
		root = new Node<TKey, TValue>(data[globalRootId].key, data[globalRootId].value);
		build(root, data, root_array, 1, n);
	}
	void log(string filename = kDefaultFileName) {
		ofstream logfile;
		logfile.open(filename);
		logfile << "digraph G {\n";

		log(root, logfile);

		logfile << "}\n";
		logfile.close();
	}
	void show() {
		show(root);
		cout << endl;
	}
	TValue view(TKey key) {
		return view(root, key);
	}

private:
	void build(Node<TKey, TValue>* parent, std::vector<Data<TKey, TValue>>& data, int root_array[][kNmax], int i, int j) {
		if (i > j)
			return;
		if (i == j) {
			parent->left = nullptr;
			parent->right = nullptr;
			return;
		}

		int localRootId = root_array[i][j];
		int leftRootId = root_array[i][localRootId - 1];
		int rightRootId = root_array[localRootId + 1][j];

		if (leftRootId) {
			Node<TKey, TValue>* left = new Node<TKey, TValue>(data[leftRootId].key, data[leftRootId].value);
			build(left, data, root_array, i, localRootId - 1);
			parent->left = left;
		}

		if (rightRootId) {
			Node<TKey, TValue>* right = new Node<TKey, TValue>(data[rightRootId].key, data[rightRootId].value);
			build(right, data, root_array, localRootId + 1, j);
			parent->right = right;
		}
	}
	void log(Node<TKey, TValue>* node, ofstream& logStream) {
		if (!node)
			return;
		
		if (node->left) {
			logStream << node->key << " -> ";
			logStream << node->left->key << "\n";
		}
		else {
			logStream << node->key << " -> ";
			logStream << "-" << "\n";
		}
		if (node->right) {
			logStream << node->key << " -> ";
			logStream << node->right->key << "\n";
		}
		else {
			logStream << node->key << " -> ";
			logStream << "-" << "\n";
		}
		log(node->left, logStream);
		log(node->right, logStream);
	}
	void show(Node<TKey, TValue>* node, int indent = 0) {
		if (!node)
			return;

		show(node->right, indent + 4);
		string format = "%*s%d %s\n";
		printf(format.c_str(), indent, "", node->key, node->value.c_str());
		show(node->left, indent + 4);
	}
	TValue view(Node<TKey, TValue>* node, TKey key) {
		if (!node)
			return TValue();
		return node->value;
		if (key < node->key)
			return view(node->left, key);
		if (key > node->key)
			return view(node->right, key);
		return TValue();
	}

	Node<TKey, TValue>* root;
};

string random_string(int n = 3) {
	string res = "";
	for (int i = 0; i < n; ++i)
		res.push_back('a' + rand() % 26);
	return res;
}

vector <int> generate_ascending_numbers(int n = 5) {
	vector<int> res;
	int last_number = 1;
	for (int i = 0; i < n; ++i) {
		int next_number = last_number + (rand() % 10 + 1);
		res.push_back(next_number);
		last_number = next_number;
	}
	return res;
}

inline double rand01() {
	return double(rand() % 101) / 100.0;
}

inline double random(double a, double b) {
	return a + (b - a) * (double(rand()) / RAND_MAX);
}

vector<double> generate_probabilities(int n = 5) {
	double sum = 0;
	vector<double> res;
	bool done = false;
	while (!done) {
		res.clear();
		for (int i = 0; i < n - 1; ++i) {
			double x = random(0.0, 1.0 - sum);
			res.push_back(x);
			sum += x;
		}
		if (sum <= 1.0) {
			res.push_back(1.0 - sum);
			done = true;
		}
	}
	random_shuffle(res.begin(), res.end());
	return res;
}

int main()
{
	cout << "starting...\n";
	srand((int)time(0));

	OptimalBinaryTree<int, string> obt;
	int n = 10;
	vector<int> ascending_numbers = generate_ascending_numbers(n);
	vector<Data<int, string>> data;
	data.push_back(Data<int, string>());
	for (int i = 0; i < n; ++i) {
		data.push_back(Data<int, string>(ascending_numbers[i], random_string()));
	}

	vector<double> probs = generate_probabilities(n * 2 + 1);
	vector<double> probs_smart;
	probs_smart = vector<double>(probs.begin(), probs.begin() + n);
	probs_smart.insert(probs_smart.begin(), 0);
	probs_smart.push_back(0);
	for (size_t i = 1; i < probs_smart.size() - 1; ++i)
		cout << data[i].value << " " << fixed << setprecision(2) << probs_smart[i] << "\n";
	cout << "\n";
	vector<double> probs_dummy;
	probs_dummy = vector<double>(probs.begin() + n, probs.end());
	probs_dummy.push_back(0);

	if (debug) {
		cout << "here\n";
		for (int i = 1; i <= 5; ++i)
			cout << data[i].key << " ";
		cout << "\n";
		for (int i = 0; i < (int)probs_smart.size(); ++i)
			cout << fixed << setprecision(2) << probs_smart[i] << " ";
		cout << "\n";
		for (int i = 0; i < (int)probs_dummy.size(); ++i)
			cout << fixed << setprecision(2) << probs_dummy[i] << " ";
		cout << "\n";
	}

	obt.build(data, probs_smart, probs_dummy);
	obt.show();
	obt.log("output.gv");
	cout << obt.view(data[0].key) << endl;
	return 0;
}

