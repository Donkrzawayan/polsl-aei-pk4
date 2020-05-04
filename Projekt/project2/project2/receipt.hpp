#ifndef RECEIPT_HPP
#define RECEIPT_HPP
#include "party.hpp"
#include <vector>
#include "item.hpp"
#include <utility> //move

class Receipt
{
	const Party *owner; //const pointer to owner data
	std::vector<Item> items;
	double sum, PTUSum;
public:
	Receipt(const Party *owner) : owner(owner), sum(0.0), PTUSum(0.0) {}
	virtual ~Receipt() = default;

	//getters for sum
	double getSum() { return sum; }
	double getPTUSum() { return PTUSum; }

	//push item to items
	void pushItem(const Item &c, unsigned int quantity, float price);

	//const_iterator for items forwarding
	auto cbegin() { return items.cbegin(); }
	auto cend() { return items.cend(); }

	//create XML document
	virtual bool createDocument()const;
protected:
	void addSellerDataToDocument(XMLDoc & doc)const;
	void addItemsToDocument(XMLDoc &doc)const;
	void addSumToDocument(XMLDoc &doc)const;

	//writing seller to XML
	void writeSellerXML(XMLDoc & doc)const { owner->writeXML(doc); }

	//writing items to XML in given way
	template<typename Function>
	Function for_eachItem(XMLDoc & doc, Function fn)const;

	//writing sum to XML
	void writeSumXML(XMLDoc & doc)const;
	void writeNettoSumXML(XMLDoc & doc)const {
		doc.addElement("Suma_netto", helpfulness::toStringPrecision2(sum - PTUSum).c_str());
	}
};

template<typename Function>
inline Function Receipt::for_eachItem(XMLDoc & doc, Function fn) const
{
	for (auto item : items)
	{
		doc.addElement("Pozycja");
		fn(doc, item);
		doc.insertChild();
	}

	return std::move(fn);
}

#endif // !RECEIPT_HPP
