#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <iostream>
#include "party.hpp"
#include <vector>
#include "item.hpp"
#include <algorithm> //sort find

class DataBase
{
private:
	using Items = std::vector<Item>;

	Party owner;
	Items stock;
	unsigned int invoiceNo; //number next invoice
private:
	//read base from bin; called from constructor
	void readBase(std::ifstream &ifs);

	//adding items to base; called from loadFromInvoice
	DataBase &operator+=(Item &&c);
public:
	DataBase(std::string dbFileName = "db.bin");

	//owner operations
	void createOwner();
	const Party *pOwner()const { return &owner; }

	//getter number of invoice
	unsigned int getInvoiceNo() { return invoiceNo++; }

	//check if there is anything in the stock
	bool checkStock()const { return stock.size(); }

	/** Check whenever item exists in sufficient quantity
	@return false if not enough quantity
	@exception std::out_of_range for out-of-range index
	*/
	bool checkItem(const unsigned int index, const int quantity)const { return stock.at(index).getQuantity() >= quantity; }

	//accest to base elements
	const Item &operator[](size_t n)const { return stock[n]; }

	//substracting from base
	template <typename InputIterator>
	void remove(InputIterator first, InputIterator last);

	//sort and write base to bin
	void writeBase(std::string dbFileName = "db.bin");

	//show to console
	void ShowStock()const;

	//load from file given by parametr
	bool loadFromXMLInvoice(const std::string &docName);
};

template<typename InputIterator>
inline void DataBase::remove(InputIterator first, InputIterator last)
{
	while (first != last) {
		auto it = std::find(stock.begin(), stock.end(), *first);
		if (it != stock.end()) {
			(*it) -= first->getQuantity();
			it->setSalesPrice(first->getSalesPrice()); //update sales price
			if (!it->getQuantity()) //if there is 0 left
				stock.erase(it);
		}
		++first;
	}
}

#endif // !DATABASE_HPP
