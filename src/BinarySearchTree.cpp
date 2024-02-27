#include <memory>
#include <random>
#include <chrono>
#include <iostream>
#include <queue>
#include <stack>
#include "Tree.hpp"

struct Tree::Node
{
	int key;
	int height;
	Item value;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* prev;
	Node** direction = nullptr;
};

class Tree::Impl
{
public:
	Node* mRoot = nullptr;
	std::vector<Node*> mNodes;
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
		Node** direction{};

		std::stack<Node*> paths;
		auto isLess = false;

		while (next != nullptr)
		{
			isLess = key < next->key;
			paths.push(next);
			prev = next;

			if (key == next->key)
			{
				next->value = value;
				return true;
			}

			if (isLess)
			{
				direction = &next->left;
				target = &next->left;
				next = next->left;
			}
			else
			{
				direction = &next->right;
				target = &next->right;
				next = next->right;
			}
		}

		//linking node to target(next empty position)
		linkNode(target, createNode(key, value, 0, prev));
		return false;
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

	Node* search(int key, std::stack<Node*>* paths = nullptr)
	{
		Node* next = mRoot;

		while (next != nullptr)
		{
			auto isGreater = key > next->key;
			auto isLess = key < next->key;

			if (paths != nullptr)
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

	Node* linkNode(Node** target, Node* newNode)
	{
		*target = newNode;
		n++;
		return newNode;
	}

	Node* createNode(int key, Item value, int height, Node* prev)
	{
		auto node = new Node();
		node->left = nullptr;
		node->right = nullptr;
		node->key = key;
		node->value = value;
		node->height = height;
		node->prev = prev;

		this->mNodes.push_back(node);
		return node;
	}

	Node* printNode(Node* node)
	{
		if (node == nullptr)
		{
			std::cout << "X(?)";
			return nullptr;
		}

		//node
		std::cout << node->key << "(" << node->height << ")";
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
				return;
			}

			//checking direction and cleaning prev node pointer
			if (node->key < prev->key)
				prev->left = nullptr;
			else
				prev->right = nullptr;

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

			if (prev != nullptr && node->key < prev->key)
				prev->left = min;
			else if (prev != nullptr)
				prev->right = min;

			//usualy the sucessor is node of the righsubtree, leftmost node
			//so it need to be removed to prevent pointing to itself
			if (min->prev != nullptr)
				min->prev->left = min->left;

			//
			if (prev == nullptr)
				mRoot = min;

			//prev needs to be min node
			childLeft->prev = min;
			childRight->prev = min;
			min->prev = node->prev;

			//
			if (childLeft != min)
				min->left = childLeft;
			if (childRight != min)
				min->right = childRight;

			//
			node->prev = nullptr;

			delete node;
			return;
		}
	}

	void printSegment(Node* left, Node* right, int spacesLeft, int spacesInner)
	{
		for (size_t i = 0; i < spacesLeft; i++)
			std::cout << " ";

		printNode(left);

		//innerspace
		for (size_t i = 0; i < spacesInner; i++)
			std::cout << "-";

		printNode(right);
		std::cout << "!";

		for (size_t i = 0; i < 2; i++)
			std::cout << " ";
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

	if (node != nullptr)
		return std::optional<Item>(node->value);

	return std::nullopt;
}

void Tree::remove(int key)
{
	mImpl->remove(key);
}

void Tree::print()
{
	auto level = 1;
	std::queue<Node*> queue;
	queue.push(mImpl->mRoot);
	queue.push(nullptr);

	//root
	auto node = queue.front();
	auto leftSpaces = mImpl->mNodes.size();
	auto num = 1;

	for (size_t i = 0; i < leftSpaces + 3; i++)
		std::cout << " ";
	std::cout << node->key << "(" << node->height << ")" << std::endl;

	while (!queue.empty())
	{
		auto node = queue.front();
		auto isNewLine = (node == nullptr);
		queue.pop();

		if (isNewLine)
		{
			std::cout << std::endl;
			num += 1;
			if (!queue.empty())
				queue.push(nullptr);

			continue;
		}

		if (node->left != nullptr)
			queue.push(node->left);

		if (node->right != nullptr)
			queue.push(node->right);

		mImpl->printSegment(
			node->left,
			node->right,
			leftSpaces,
			num);
		leftSpaces /= 2;
	}
	std::cout << std::endl;
}

size_t Tree::size()
{
	return mImpl->n;
}