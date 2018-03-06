#include <fstream>
#include <vector>
#include <algorithm>

/*Task: A team has three players (x,y,z).Team i dominates team j, if the following inequation holds : x_i<x_j, y_i<y_j,z_i<z_j.
Clearly, if team i dominates team j, the latter hardly stands a chance to win the Olympics.
It is therefore only sensible to name team i candidate for a win, if no other team dominates it.
Write an algorythm that would allow to determin the number of team candidates for a win based on the results of the singles Olympics*/

const int INF = 1e9;

class SegmentTree{
	std::vector<int> tree;
	int getArraySize(int n){
		int p = 0;
		while (n > 0){
			n >>= 1;
			p++;
		}
		return pow(2, p);
	}
public:
	SegmentTree(int n){
		tree.resize(2 * getArraySize(n));
		fill(tree.begin(), tree.end(), INF);
	}
	SegmentTree(std::vector<std::vector<int>> arr){
		int size = getArraySize(arr.size());
		tree.resize(2 * size);
		fill(tree.begin(), tree.end(), INF);
		for (std::size_t i = 0; i < arr.size(); ++i){
			tree[i + size] = arr[i][1];
		}
		for (int i = size - 1; i > 0; --i){
			tree[i] = std::min(tree[2 * i + 1], tree[2 * i]);
		}
	}
	void add(int v, int pos){
		pos += tree.size() / 2;
		tree[pos] = v;
		while (pos > 0){
			pos /= 2;
			tree[pos] = std::min(tree[2 * pos + 1], tree[2 * pos]);
		}
	}
	int count(int i, int j){
		i += tree.size() / 2;
		j += tree.size() / 2;
		int result = INF;
		if (i == j){
			return tree[i];
		}
		while (i <= j){
			if (i % 2){
				result = std::min(result, tree[i]);
			}
			if (!(j % 2)){
				result = std::min(result, tree[j]);
			}
			i = (i + 1) / 2;
			j = (j - 1) / 2;
		}
		return result;
	}
};

std::vector<std::vector<int>> teams;
int n = 0;
const int IN_TEAM = 3;
int counter = 0;

bool sortZ(const std::vector<int>& v1, const std::vector<int>& v2) {
	return v2[2] - v1[2] > 0;
}
bool sortX(const std::vector<int>& v1, const std::vector<int>& v2) {
	return v2[0] - v1[0] > 0;
}
void changeSequence(){
	//firstly teams are sorted by z player's score to reduce the number of features
	sort(teams.begin(), teams.end(), sortZ);
	for (int i = 0; i < n; ++i){
		teams[i][2] = i;
	}
	sort(teams.begin(), teams.end(), sortX);
}

void getData(std::ifstream& in){
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < IN_TEAM; ++j){
			in >> teams[i][j];
		}
	}
}

void solve(SegmentTree tree){
	//other features are compared using segment tree
	for (int i = 0; i < n; ++i){
		int y = tree.count(0, teams[i][2]);
		if (teams[i][1] <= y){
			counter++;
		}
		tree.add(teams[i][1], teams[i][2]);
	}
}

void main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	in >> n;
	teams.resize(n, std::vector<int>(IN_TEAM));
	getData(in);
	changeSequence();
	SegmentTree tree(n);
	solve(tree);
	out << counter << std::endl;
}