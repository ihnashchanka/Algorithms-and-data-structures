#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

/*Task: Encoding a phone number with words.
Given is a rendition table containing alphanumerical pairs as well as word combinations that can be used.*/

const int INF = 1e9;

class Node{
public:
	char ch;
	std::vector<Node*> children;
	bool isWord;
	std::string letters;
	Node(char c) : ch(c), isWord(false), children(0) {}
};

/*To find the most suitable word the prefix tree is used.*/
class BOR{
public:
	Node* root;
	BOR() :root(new Node('-')){}
	void add(std::string str, std::string letters){
		Node* node = root;
		for (std::size_t i = 0; i < str.length(); ++i){
			bool flag = false;
			char cur = str[i];
			for (std::size_t j = 0; j < node->children.size() && !flag; ++j){
				if (node->children[j]->ch == cur){
					node = node->children[j];
					flag = true;
					break;
				}
			}
			if (!flag){
				Node* tmp = new Node(cur);
				node->children.push_back(tmp);
				node = node->children.back();
			}
			if (i == str.length() - 1){
				node->isWord = true;
				node->letters = letters;
			}
		}
	}

	Node* find(Node* start, std::string str, bool* result, std::string* letters)	{
		*letters = "";
		*result = false;
		Node* node = start;
		for (std::size_t i = 0; i < str.length(); ++i){
			char cur = str[i];
			for (std::size_t j = 0; j != node->children.size(); ++j){
				if (node->children[j]->ch == cur){
					node = node->children[j];
					break;
				}
			}
		}
		start = node;
		if (node->isWord){
			*result = true;
			*letters = node->letters;
		}
		return start;
	}
	
};

std::string number = "";
int dictionaryLength;
std::vector<int> counter;
std::vector<std::string> words;
BOR b;

std::string transcript(std::string letters)
{
	std::string digits = "";
	for (std::size_t i = 0; i < letters.length(); ++i){
		char temp = letters[i];
		if (temp == 'I' || temp == 'J' || temp == '1')
			digits += '1';
		if (temp == 'A' || temp == 'B' || temp == 'C' || temp == '2')
			digits += '2';
		if (temp == 'D' || temp == 'E' || temp == 'F' || temp == '3')
			digits += '3';
		if (temp == 'G' || temp == 'H' || temp == '4')
			digits += '4';
		if (temp == 'K' || temp == 'L' || temp == '5')
			digits += '5';
		if (temp == 'M' || temp == 'N' || temp == '6')
			digits += '6';
		if (temp == 'P' || temp == 'R' || temp == 'S' || temp == '7')
			digits += '7';
		if (temp == 'T' || temp == 'U' || temp == 'V' || temp == '8')
			digits += '8';
		if (temp == 'W' || temp == 'X' || temp == 'Y' || temp == '9')
			digits += '9';
		if (temp == 'O' || temp == 'Q' || temp == 'Z' || temp == '0')
			digits += '0';
	}
	return digits;
}

void inizialize(std::ifstream& in)
{
	in >> number;
	in >> dictionaryLength;

	for (int i = 0; i < dictionaryLength && !in.eof(); ++i){
		std::string str;
		in >> str;
		b.add(transcript(str), str);
	}

	counter.resize(number.length());
	fill(counter.begin(), counter.end(), INF);
	words.resize(number.length());
	fill(words.begin(), words.end(), "");
}


std::vector<std::string> solve()
{
	Node* start;
	std::string letters = "";
	bool find = false;
	Node* searchStop;
	std::string str;
	bool toContinue = true;
	for (int i = number.size()-1; i >=0; --i){
		toContinue = true;
		start = b.root;
		for (std::size_t j = 0; j <number.size() - i - 1; ++j){
			str = std::string(1, number[j + i]);
			searchStop = b.find(start, str, &find, &letters);
			if (searchStop == start){
				toContinue = false;
				break;
			}
			else{
				start = searchStop;
			}
			if (words[i+j+1] != ""){
				if (find){
					if (counter[i + j + 1] + 1 < counter[i]){
						words[i] = letters;
						counter[i] = counter[i + j + 1] + 1;
					}
				}
			}
		}
		if (toContinue){
			find = false;
			str = std::string(1, number[number.size() - 1]);
			b.find(start, str, &find, &letters);
			if (find){
				words[i] = letters;
				counter[i] = 1;
			}
		}
	}
	return words;
}


std::string createAnswer(const std::vector<std::string>& r, int* lenght){
	std::size_t c = 0;
	std::string result = r[c];
	c += r[c].length();
	while (c < r.size()){
		if (r[c] != ""){
			result += " " + r[c];
			c += r[c].length();
			(*lenght)++;
		}
		else{
			c--;
		}
	}
	return result;
}


void main(){
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");
	inizialize(in);

	std::vector<std::string> r = solve();
	std::string result = "";
	int c = 0;
	int lenght = 1;

	if (r[c] == "")
		out << "No solution" << std::endl;
	else{
		result = createAnswer(r, &lenght);
	}
	out << lenght << std::endl;
	out << result;
	in.close();
	out.close();
}