#include <memory>
#include <random>
#include <chrono>
#include <iostream>
#include "SymbolTable.hpp"
#include "Tree.hpp"
/*hashing has been base on https://cp-algorithms.com/string/string-hashing.html resouces*/


class SymbolTable::Impl
{
public:
	Tree tree;
	int p; //prime number
	int m;

	Impl()
		: p(53), m(1000000000 + 9)
	{
	}

	~Impl()
	{
	}

	uint32_t hash(std::string key)
	{
		auto hash = 0;
		auto pow = 1;

		for (int i = 0; i < key.size(); i++)
		{
			pow = pow * p;
			hash = (key[i] + (key[i] - 'a' + 1)) * pow;
		}

		return hash % m;
	}
};

SymbolTable::SymbolTable()
	: mImpl(std::make_unique<SymbolTable::Impl>())
{
}

SymbolTable::~SymbolTable()
{
}


void SymbolTable::put(std::string key, Item val)
{
	auto hash = mImpl->hash(key);
	mImpl->tree.insert(hash, val);
}

std::optional<Item> SymbolTable::get(std::string key)
{
	auto hash = mImpl->hash(key);
	return mImpl->tree.search(hash);
}

void SymbolTable::remove(std::string key)
{

}

bool SymbolTable::contains(std::string key)
{
	return false;
}

bool SymbolTable::isEmpty()
{
	return false;
}

size_t SymbolTable::size()
{
	return 0;
}