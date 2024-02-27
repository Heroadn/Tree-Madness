#include <memory>
#include <random>
#include <chrono>
#include <iostream>
#include <any>
#include <string>

using Item = std::string;

class Tree
{
public:
	Tree();

	~Tree();

	//returns true if the key already exists, and replaces the value
	bool insert(int key, Item value);

	void remove(int key);

	void print();

	size_t size();

	std::optional<Item> search(int key);

private:
	class Impl;
	struct Node;
	std::unique_ptr<Impl> mImpl;
};