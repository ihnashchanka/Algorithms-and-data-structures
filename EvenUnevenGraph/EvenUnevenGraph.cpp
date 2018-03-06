#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>

/*Task: To find out if each pair of nodes of the graph is connected both with a way of even and unieven length*/

/*Advanced BFS*/
struct node{
	bool even;
	int index;
	bool uneven;
	bool visited;
	node(){
		even = 0;
		uneven = 0;
		visited = 0;
	};
};

int n;
bool** ma;
std::vector<node> graph;
short counter = -1;

struct nodeStruct{
	node value;
	bool evenWay;
	nodeStruct(node n, bool way){
		value = n;
		evenWay = way;
	}
	nodeStruct(){}
};

int BFS(int start){
	int countEven = 0;
	int countUneven = 0;
	std::queue<node> q;
	if (graph[start].even && !graph[start].uneven)
		countEven++;
	if (!graph[start].even && graph[start].uneven)
		countUneven++;
	graph[start].visited = true;
	q.push(graph[start]);
	while (!q.empty()){
		node cur = q.front();
		q.pop();
		for (int i = 0; i < n; ++i){
			if (!graph[i].visited && ma[cur.index][i]){
				graph[i].visited = true;
				if (graph[i].even && !graph[i].uneven)
					countEven++;
				if (!graph[i].even && graph[i].uneven)
					countUneven++;
				q.push(graph[i]);
			}
		}
	}
	return std::max(countEven, countUneven);
}

void BFS_even_uneven(int start){
	std::queue<nodeStruct> q;
	nodeStruct cur;
	graph[start].even = true;
	q.push(nodeStruct(graph[start],true));
	while (!q.empty()){
		cur = q.front();
		q.pop();
		for (int i = 0; i < n; ++i){
			if (ma[cur.value.index][i]){
				if (cur.evenWay == true)
					if (graph[i].uneven == false){
						graph[i].uneven = true;
						q.push(nodeStruct(graph[i],false));
					}
				if (cur.evenWay == false)
					if (graph[i].even == false){
						graph[i].even = true;
						q.push(nodeStruct(graph[i],true));
					}
			}
		}
	}
}

void getMatrix(std::ifstream& in){
	std::string line;
	getline(in, line);
	for (int i = 0; i < n; ++i){
		std::stringstream ss;
		getline(in, line);
		ss << line;
		int j = 0;
		while (ss >> j){
			if (j == 0)
				break;
			ma[i][j - 1] = 1;
			ma[j - 1][i] = 1;
		}
	}
}

void answer(std::ofstream& out){
	for (int i = 0; i < n; ++i){
		if (!graph[i].even && !graph[i].uneven){
			BFS_even_uneven(i);
			counter++;
		}
	}

	bool flag = true;
	for (int i = 0; i < n; ++i){
		if (!graph[i].even || !graph[i].uneven){
			flag = false;
			break;
		}
	}
	if ((counter == 0) && flag) {
		out << "YES" << std::endl;
	}
	else{
		int result = 0;
		out << "NO" << std::endl;
		for (int i = 0; i < n; ++i){
			if (!graph[i].visited){
				result += std::max(1, BFS(i));
			}
		}
		out << result;
	}
}

void main(){
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	in >> n;
	ma = new bool*[n];
	for (int i = 0; i < n; ++i){
		ma[i] = new bool[n];
	}
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			ma[i][j] = 0;
		}
	}

	graph.resize(n);
	for (int i = 0; i < n; ++i)
		graph[i].index = i;

	getMatrix(in);
	answer(out);
}