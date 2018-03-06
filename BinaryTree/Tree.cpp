#include <fstream>
#include <algorithm>

/*To find and delete max node, path's length from which to any other node <k
and there is no halfpaths that contains this node.*/

template <class T>
class Node {
public:
	T key;
	Node* left;
	Node* right;
	Node* parent;
	int leftHight;
	int rightHight;
	int eccentricity;
	int parentLenght;
	int maxWay;
	Node(T _key) : key(_key), left(0), right(0), parent(0) {}
	bool hasLeft(){
		return left != 0;
	}
	bool hasRight(){
		return right != 0;
	}
};

template <class T>
class TTree {
public:
	int k;
	Node<T>* chosen;
	Node<T>* root;
	TTree() {
		root = 0;
		chosen = 0;
	}
	void insert(T value){
		Node<T>* curNode = root;
		Node<T>* _parent = 0;
		while (curNode != 0){
			_parent = curNode;
			if (value < curNode->key)
				curNode = curNode->left;
			else if (value >curNode->key)
				curNode = curNode->right;
			else break;
		}
		curNode = new Node<T>(value);
		if (_parent != 0){
			if (value < _parent->key){
				_parent->left = curNode;
			}
			else if (value > _parent->key){
				_parent->right = curNode;
			}
		}
		else{
			root = curNode;
		}
		curNode->parent = _parent;
	}
	void straightLeftPrint(Node<T> start, std::ofstream& out){
		out << start.key << std::endl;
		if (start.hasLeft()){
			straightLeftPrint(*(start.left), out);
		}
		if (start.hasRight()){
			straightLeftPrint(*(start.right), out);
		}
	}
	void read(std::ifstream& in){
		int cur = 0;
		if(!in.eof()) in >> k;
		while (in >> cur){
			insert(cur);
		}
	}

	void deleteRight(Node<T>* fnode){
		if (fnode == 0){
			return;
		}
		else{
			Node<T> node = *fnode;
			if (!deleteSimple(node)){
				Node<T>* curNode = node.right;
				Node<T>* parent = fnode;
				while (curNode->hasLeft()){
					parent = curNode;
					curNode = curNode->left;
				}
				int key = curNode->key;
				curNode->parent = parent; 
				deleteSimple(*curNode);
				fnode->key = key;
			}
		}
	}

	void solve(Node<T>* v){
		if (v->parent == 0) {
			v->parentLenght = 0;
		} else {
			if (v == v->parent->right){
				v->parentLenght = std::max(v->parent->leftHight, v->parent->parentLenght) + 1;
			}
			if (v == v->parent->left){
				v->parentLenght = std::max(v->parent->rightHight, v->parent->parentLenght) + 1;
			}
		}
		v->eccentricity = std::max(std::max(v->parentLenght, v->rightHight), v->rightHight);
		v->maxWay = std::max(std::max(v->rightHight + v->leftHight, v->rightHight + v->parentLenght), v->parentLenght + v->leftHight);
		if ((v->maxWay < k) && (v->eccentricity < k)){
			if (chosen == 0) {
				chosen = v;
			} 
			else if (chosen->key < v->key){
				chosen = v;
			}
		}
		if (v->hasLeft()){
			solve(v->left);
		}
		if (v->hasRight()){
			solve(v->right);
		}
	}
	void calculateHights(Node<T>* v){
		if (v->hasLeft()){
			calculateHights(v->left);
		}
		if (v->hasRight()){
			calculateHights(v->right);
		}
		if (!v->hasLeft()){
			v->leftHight = 0;
		}
		else {
			v->leftHight = std::max(v->left->leftHight, v->left->rightHight) + 1;
		}
		if (!v->hasRight()){
			v->rightHight = 0;
		}
		else {
			v->rightHight = std::max(v->right->leftHight, v->right->rightHight) + 1;
		}
	}
private:
	int deleteSimple(Node<T> node){
		if (node.left == 0 && node.right == 0){
			if (node.key == root->key){
				root = 0;
			}
			else if (node.key < node.parent->key){
				node.parent->left = 0;
			}
			else {
				node.parent->right = 0;
			}
			return 1;
		}
		if (node.left != 0 && node.right == 0){
			if (node.key == root->key){
				root = node.left;
			}
			else if (node.key < node.parent->key){
				node.parent->left = node.left;
			}
			else {
				node.parent->right = node.left;
			}
			return 1;
		}
		if (node.left == 0 && node.right != 0){
			if (node.key == root->key){
				root = node.right;
			}
			else if (node.key < node.parent->key){
				node.parent->left = node.right;
			}
			else{
				node.parent->right = node.right;
			}
			return 1;
		}
		return 0;
	}
};

void main(){
	TTree <int> tree;
	std::ifstream in("tst.in", std::ios::in);
	std::ofstream out("tst.out");
	tree.read(in);
	if (tree.root != 0)
	{
		tree.calculateHights(tree.root);
		tree.solve(tree.root);
		if (tree.chosen !=0)
		tree.deleteRight(tree.chosen);
	}
	if (tree.root != 0)
		tree.straightLeftPrint(*tree.root, out);
	else
		out << "Empty";
}
