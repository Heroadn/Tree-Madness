#include <memory>
#include <random>
#include <chrono>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric> 
#include <vector> 
#include "Tree.hpp"

#ifdef WIN32
#include <windows.h>

void screenSize(int* columns, int* rows)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	*columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#else
#include <sys/ioctl.h>
#include <unistd.h>

void screenSize(int* columns, int* rows)
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	*rows = w.ws_row;
	*cols = w.ws_col;
}
#endif 
#undef max

struct Tree::Node
{
	int key;   
	int height;
	Item value;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* prev;
};

class Tree::Impl
{
public:
	Node* mRoot = nullptr;
	int n;

	Impl()
		: n(0)
	{
	}

	~Impl()
	{
	}

	bool insert(int key, Item value, struct Node* root)
	{
		Node* next = root;
		Node* prev = nullptr;
		Node** target{};
		auto isLess = false;

		while (next != nullptr)
		{
			isLess = key < next->key;
			prev = next;

			if (key == next->key)
			{
				next->value = value;
				return true;
			}

			if (isLess)
			{
				target = &next->left;
				next = next->left;
			}else
			{
				target = &next->right;
				next = next->right;
			}
		}

		//linking node to target(next empty position)
		Node *node = linkNode(target, createNode(key, value, 0, prev));
		balance(node);

		return false;
	}

	void balance(Node* node)
	{
		Node* prev = node->prev;

		while (prev != nullptr)
		{
			Node* item = prev;
			item->height = calculateHeight(item);
			auto balance = calculateBalance(item);

			//
			rotate(balance, item);

			prev = prev->prev;
		}
	}

	void balance(std::stack<Node*> paths)
	{
		while (!paths.empty())
		{
			Node* item = paths.top();
			paths.pop();

			//
			if (item == nullptr)
				continue;

			item->height = calculateHeight(item);
			auto balance = calculateBalance(item);

			//
			rotate(balance, item);
		}
	}

	void rotate(int balance, Tree::Node* item)
	{
		//positive = left unbalance
		if (balance > 1 && calculateBalance(item->left) < 0)
			shortRotateLeft(item);
		if (balance > 1)
			rightRotation(item);

		//negative = right unbalance
		if (balance < -1 && calculateBalance(item->right) > 0)
			shortRotateRight(item);
		if (balance < -1)
			leftRotation(item);
	}

	void shortRotateRight(Tree::Node* item)
	{
		Node* a = item;
		Node* b = item->right;
		Node* c = b->left;
		Node* childC = c->right;

		a->right = c;
		c->right = b;

		b->prev = c;
		c->prev = a;
		b->left = childC;

		if (childC != nullptr)
			c->prev = b;

		item->height = calculateHeight(item);
		b->height = calculateHeight(b);
		c->height = calculateHeight(c);
	}

	void shortRotateLeft(Tree::Node* item)
	{
		Node* a = item;
		Node* b = item->left;
		Node* c = b->right;
		Node* childC = c->left;

		a->left = c;
		c->left = b;

		b->prev = c;
		c->prev = a;
		b->right = childC;

		if (childC != nullptr)
			childC->prev = b;

		item->height = calculateHeight(item);
		b->height = calculateHeight(b);
		c->height = calculateHeight(c);
	}

	Node* findInorderSucessor(const Node const* tree)
	{
		Node* next = tree->right;
		Node* prev = nullptr;

		if (next == nullptr)
		{
			next = tree->prev;
			prev = tree->prev->prev; 
			while (prev != nullptr && prev->right == next)
			{
				next = prev;
				prev = prev->prev;
			}

			next = (prev == nullptr) ? next : prev;
			
			//if next is less than the tree, but the right node is greater than tree
			if (next->key < tree->key && next->right != nullptr && next->right->key > tree->key)
				return next->right;
			if (next->key < tree->key)
				return nullptr;

			return next;
		}

		//inorder sucessor is the leftmost element of the subtree
		while (next != nullptr && next->left != nullptr)
			next = next->left;

		return next;
	}

	Node* search(int key, std::stack<Node*> *paths = nullptr)
	{
		Node* next = mRoot;

		while (next != nullptr)
		{
			auto isGreater = key > next->key;
			auto isLess = key < next->key;

			if(paths != nullptr)
				paths->push(next);

			if (next->key == key)
				return next;

			if (isLess)
				next = next->left;
			else if (isGreater)
				next = next->right;
		}

		return nullptr;
	}

	Node* leftRotation(Node *node)
	{
		Node* b = node->right;
		Node* a = node;
		Node* prev = a->prev;

		//b is now the root of the subtree
		if (prev != nullptr)
		{
			auto testB = b;
			auto testA = a;
			auto testC = node;

			//direction of the subtree
			if (b->key > prev->key)
				prev->right = b;
			else
				prev->left = b;
		}
		else
			//b is now the root of the tree
			mRoot = b;

		//making node pointing to a point to b
		b->prev = prev; 

		//a takes b left child as right
		a->right = b->left;

		//a previous needs to point to b
		a->prev = b;

		//b takes a as left child
		b->left = a;

		if (b->left != nullptr)
			b->left->height = calculateHeight(b->left);

		if (b->right != nullptr)
			b->right->height = calculateHeight(b->right);
		return b;
	}

	Node* rightRotation(Node* node)
	{
		Node* b = node->left;
		Node* c = node;
		Node* prev = c->prev;

		//b is now the root of the subtree	
		if (prev != nullptr)
		{
			//direction of the subtree
			//direction of the subtree
			if (b->key > prev->key)
				prev->right = b;
			else
				prev->left = b;
		}
		else
			//b is now the root of the tree
			mRoot = b;

		//making node pointing to a point to b
		b->prev = prev;

		//c takes b left child as right
		c->left = b->right;

		//a previous needs to point to b
		c->prev = b;

		//b takes c as left child
		b->right = c;

		if (b->left != nullptr)
			b->left->height = calculateHeight(b->left);

		if (b->right != nullptr)
			b->right->height = calculateHeight(b->right);
		return b;
	}

	Node* linkNode(Node** target, Node *newNode)
	{
		*target = newNode;
		n++;
		return newNode;
	}

	Node* createNode(int key, Item value, int height, Node *prev)
	{
		auto node = new Node();
		node->left = nullptr;
		node->right = nullptr;
		node->key = key;
		node->value = value;
		node->height = height;
		node->prev = prev;
		return node;
	}

	Node* printNode(Node* node)
	{
		if (node == nullptr)
		{
			std::cout << "X";
			return nullptr;
		}

		//node
		//std::cout << node->key << "(" << node->height << ")";
		std::cout << node->key;
		return node;
	}

	void remove(int key)
	{
		std::stack<Node*> paths;
		Node* node = search(key, &paths);
		Node* prev;

		//node has been found
		if (node == nullptr)
			return;

		//
		prev = node->prev;

		//checking if is a leaf
		if (node->left == nullptr && node->right == nullptr)
		{
			//if is root and dont have left and right
			if (prev == nullptr)
			{
				delete node;
				mRoot = nullptr;
				return;
			}

			//adding previous to balance
			paths.push(prev);

			//checking direction and cleaning prev node pointer
			if (node->key < prev->key)
				prev->left = nullptr;
			else
				prev->right = nullptr;

			//balancing
			balance(paths);

			//
			delete node;
			return;
		}

		//checking if has a single child, not both null
		if (node->left == nullptr || node->right == nullptr)
		{
			Node* child = (node->left == nullptr) ? node->right : node->left;

			//checking direction and cleaning prev node pointer
			if (node->key < prev->key)
				prev->left = child;
			else
				prev->right = child;

			//adding to balance
			paths.push(child);
			paths.push(prev);

			//balancing
			balance(paths);

			delete node;
			return;
		}

		//if was more than one child
		if (node->left != nullptr && node->right != nullptr)
		{
			//
			Node* min = findInorderSucessor(node);
			Node* childLeft = node->left;
			Node* childRight = node->right;
			prev = node->prev;

			//finding the direction of node(left or right based on key) 
			//and replacing it with sucessor
			if (prev != nullptr && node->key < prev->key)
				prev->left = min;
			else if(prev != nullptr)
				prev->right = min;

			//usualy the sucessor is node of the righsubtree, leftmost node
			//so it need to be removed to prevent pointing to itself

			//but prev can be the node to be deleted(since it will be deleted no reason to add a node to it)
			if (min->prev != nullptr && min->prev != node)
				min->prev->left  = min->left;

			//
			if(prev == nullptr)
				mRoot = min;

			//prev needs to be min node
			childLeft->prev  = min;
			childRight->prev = min;
			min->prev = node->prev;

			//checks to prevent the node to pointing to itself
			if(childLeft != min)
				min->left  = childLeft;
			if (childRight != min)
				min->right = childRight;

			//
			node->prev = nullptr;

			//adding to balance
			paths.push(mRoot);
			paths.push(min);
			paths.push(min->left);
			paths.push(min->right);

			//balancing
			balance(paths);

			//node 12(left) is still points to node 10
			delete node;
			return;
		}
	}

	int calculateBalance(Node* node)
	{
		if (node == nullptr)
			return 0;

		return  maxHeight(node->left) - maxHeight(node->right);
	}

	int calculateHeight(Node* node)
	{
		auto left  = maxHeight(node->left);
		auto right = maxHeight(node->right);
		return 1 + std::max(left, right);
	}

	int maxHeight(Node* node)
	{
		if (node == nullptr)
			return -1;

		return node->height;
	}
};

Tree::Tree()
	: mImpl(std::make_unique<Tree::Impl>())
{
}

Tree::~Tree()
{
}

bool Tree::insert(int key, Item value)
{
	auto* root = mImpl->mRoot;

	if (mImpl->mRoot == nullptr)
	{
		mImpl->mRoot = mImpl->createNode(key, value, 0, nullptr);
		mImpl->n++;
		return false;
	}

	return mImpl->insert(key, value, root);
}

std::optional<Item> Tree::search(int key)
{
	Node* node = mImpl->search(key);

	if(node != nullptr)
		return std::optional<Item>(node->value);

	return std::nullopt;
}

void Tree::remove(int key)
{
	mImpl->remove(key);
}

void Tree::print()
{
	if (mImpl->mRoot == nullptr)
		return;

	std::queue<Node*> queue;
	queue.push(mImpl->mRoot);
	queue.push(nullptr);

	//
	auto width = 0, height = 0;
	screenSize(&width, &height);
	width /= 4;

	//root
	auto node = queue.front();
	std::vector<std::queue<Node*>> levels;
	std::queue<Node*> level;
	
	//
	level.push(mImpl->mRoot);
	levels.push_back(level);
	level = std::queue<Node*>();


	while (!queue.empty())
	{
		auto node = queue.front();
		auto isNewLine = (node == nullptr);
		queue.pop();

		if (isNewLine)
		{
			levels.push_back(level);
			level = std::queue<Node*>();

			//
			if (!queue.empty())
				queue.push(nullptr);

			continue;
		}

		if (node->left != nullptr)
			queue.push(node->left);

		if (node->right != nullptr)
			queue.push(node->right);
		
		level.push(node->left);
		level.push(node->right);
	}

	for (auto line : levels)
	{
		for (size_t i = 0; i < width; i++)
			std::cout << " ";
		width -= 2;

		//root
		if (line.size() == 1)
		{
			auto root = line.front();
			line.pop();

			mImpl->printNode(root);
		}

		while(!line.empty())
		{
			auto left = line.front();
			line.pop();

			auto right = line.front();
			line.pop();

			mImpl->printNode(left);
			for (size_t i = 0; i < 2; i++)
				std::cout << "-";
			mImpl->printNode(right);

			std::cout << " ";
		}

		std::cout << std::endl;
	}
	
}

size_t Tree::size()
{
	return mImpl->n;
}