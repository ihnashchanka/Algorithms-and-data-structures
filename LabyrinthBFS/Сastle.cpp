#include <vector>
#include <fstream>
#include <queue>
#include <iostream>
#include <algorithm>

/*À castle is divided into m x n squares, that are represented as a sequence of numbers.
The number is a sum of the following numbers:
1 - if the square has a western wall
2 - the northern wall
3 - eastern
4 - southern
Determine :
-the number of rooms in the castle
- the square of the largest room
- which wall needs to be torn down to get the largest possible room

/*The castle is considered to be a graph. The problem is solving using BFS*/
struct room{
	bool visited = false;
	bool left =false;
	bool top = false;
	bool right = false;
	bool bottom = false;
	int i;
	int j;
	room(){};
	room(int n, int _i, int _j){
		visited = false;
		if (n - 8 >= 0){
			bottom = true;
			n -= 8;
		}
		if (n - 4 >= 0){
			right = true;
			n -= 4;
		}
		if (n - 2 >= 0){
			top = true;
			n -= 2;
		}
		if (n - 1 >= 0){
			left = true;
			n -= 1;
		}
		i = _i;
		j = _j;
	}
};


int lines, col;
std::vector<std::vector<room>> matrix;
std::vector<std::vector<int>> sets;
std::queue<room> q;
std::vector<int> sizes;

int counter = 0;
int biggest = 0;
int del = 0;

void getData(std::ifstream& in){
	for (int i = 0; i < lines; ++i){
		for (int j = 0; j < col; ++j){
			int cur = 0;
			in >> cur;
			matrix[i][j] = room(cur, i, j);
		}
	}
}

void solve(){
	for (int i = 0; i < lines; ++i){
		for (int j = 0; j < col; ++j){
			if (!matrix[i][j].visited){
				matrix[i][j].visited = true;
				q.push(matrix[i][j]);

				sizes.push_back(0);
				while (!q.empty()){
					room tmp = q.front();
					q.pop();
					sizes[counter]++;
					if (tmp.i && !tmp.top){
						if (!matrix[tmp.i - 1][tmp.j].visited){
							matrix[tmp.i - 1][tmp.j].visited = true;
							q.push(matrix[tmp.i - 1][tmp.j]);
						}
					}
					if (tmp.i < lines - 1 && !tmp.bottom){
						if (!matrix[tmp.i + 1][tmp.j].visited){
							matrix[tmp.i + 1][tmp.j].visited = true;
							q.push(matrix[tmp.i + 1][tmp.j]);
						}
					}
					if (tmp.j&&!tmp.left){
						if (!matrix[tmp.i][tmp.j - 1].visited){
							matrix[tmp.i][tmp.j - 1].visited = true;
							q.push(matrix[tmp.i][tmp.j - 1]);
						}
					}
					if (tmp.j < col - 1 && !tmp.right){
						if (!matrix[tmp.i][tmp.j + 1].visited){
							matrix[tmp.i][tmp.j + 1].visited = true;
							q.push(matrix[tmp.i][tmp.j + 1]);
						}
					}
					sets[tmp.i][tmp.j] = counter;
				}
				counter++;
			}
		}
	}
}

void ruin(){
	for (int i = 0; i < lines; ++i){
		for (int j = 0; j < col; ++j){
			if (i + 1 < lines && sets[i][j] != sets[i + 1][j]){
				if (sizes[sets[i][j]] + sizes[sets[i + 1][j]] > del)
					del = sizes[sets[i][j]] + sizes[sets[i + 1][j]];
			}
			if (j + 1 < col && sets[i][j] != sets[i][j + 1]){
				if (sizes[sets[i][j]] + sizes[sets[i][j + 1]] > del)
					del = sizes[sets[i][j]] + sizes[sets[i][j + 1]];
			}
		}
	}
}


void main(){
	
	std::ofstream out("output.txt");
	std::ifstream in("input.txt");
	
	in >> lines >> col;
	matrix.resize(lines, std::vector<room>(col));
	sets.resize(lines, std::vector<int>(col));
	getData(in);
	solve();
	out << counter << std::endl;
	biggest = *max_element(sizes.begin(), sizes.end());
	out << biggest << std::endl;
	del = biggest;
	ruin();
	out << del << std::endl;
	out.close();
	in.close();
}