#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <iostream>
#include "party.hpp"
#include <vector>
#include "commodity.hpp"

class DataBase
{
	Party owner;
	std::vector<Commodity> stock;
public:
	DataBase();
	~DataBase() = default;

	std::istream &read(std::istream & ifs);
	std::ostream &write(std::ostream &of)const;

	void push(const Commodity &t);
	void pop_back();
};

#endif // !DATABASE_HPP
