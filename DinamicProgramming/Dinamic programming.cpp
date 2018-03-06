#include<fstream>
#include<algorithm>
#include<iostream>

/*Task: Game modeling. There is a row of numbers and two plyers.
Each player choose one number: either from the left side of th row or from the right.
Assuming that our oppenet alwayse choose the best option, you should try to win. Count the sum you can win.*/


void read(std::istream& in, long long** s, long long** f, int n)
{
	for (int i = 0; i < n; ++i){
		in >> s[i][i];
		f[i][i] = s[i][i];
	}
}

void countSums(long long** sum, int n){
	for (int i = 0; i < n - 1; ++i){
		sum[i][i + 1] = sum[i][i] + sum[i + 1][i + 1];
	}
	for (int i = 0; i < n - 2; ++i){
		for (int j = i + 2; j < n; ++j){
			sum[i][j] = sum[i][j - 1] + sum[j][j];
		}
	}
}

long long solve(long long** sum, long long** F, int n){
	countSums(sum,n);
	for (int i = 1; i < n; ++i){
		for (int j = 0; j < n - i; ++j){
			F[j][j + i] = sum[j][j + i] - std::min(F[j + 1][j + i], F[j][j + i - 1]);
		}
	}
	return F[0][n - 1];

}

void main(){
	long long** F = 0;
	long long** s = 0;
	int n = 0;
	std::fstream in("input.txt");
	std::ofstream out("output.txt");
	if (!in.eof()){
		in >> n;
	}
	s = new long long*[n];
	F = new long long*[n];
	for (int i = 0; i < n; ++i){
		s[i] = new long long[n];
		F[i] = new long long[n];
	}
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; j++){
			s[i][j] = F[i][j] = 0;
		}
	}
	read(in, s, F, n);
	out << solve(s, F, n);
	in.close();
	out.close();
}