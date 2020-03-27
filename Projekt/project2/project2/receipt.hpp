#ifndef RECEIPT_HPP
#define RECEIPT_HPP
#include "party.hpp"
#include <vector>
#include "commodity.hpp"

class Receipt
{
	std::vector<Commodity> goods;
	Party seller;
public:
	Receipt();
	virtual ~Receipt() = default;
};

#endif // !RECEIPT_HPP
