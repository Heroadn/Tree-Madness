#include <memory>
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include <optional>

using Item = std::string;

class SymbolTable
{
public:
	SymbolTable();

	~SymbolTable();

	void put(std::string key, Item val);

	std::optional<Item> get(std::string key);

	void remove(std::string key);

	bool contains(std::string key);

	bool isEmpty();

	size_t size();

private:
	class Impl;
	std::unique_ptr<Impl> mImpl;
};