#ifndef COMMODITY_HPP
#define COMMODITY_HPP
#include <string>

class Commodity
{
	std::string name;
	int quantity, VAT;
	float price;
public:
	Commodity();
	~Commodity() = default;
};

#endif // !COMMODITY_HPP
