#ifndef RECEIPT_HPP
#define RECEIPT_HPP
#include "party.hpp"
#include <vector>
#include "item.hpp"
#include <utility> //move
#include "tinyxml2/tinyxml2.h"
#include <tuple>


class Receipt //dodac zagniezdzna klase ItemRef
{
	const Party *owner; //const pointer to owner data
	std::vector<Item> items;
	double sum, PTUSum;

	//std::vector<std::tuple<size_t, unsigned int, float>> commodityRefs; //idea for future?
protected:
	//writing seller to XML
	void writeSellerXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pElement)const { owner->writeXML(doc, pElement); }

	//writing items to XML in given way
	template<typename Function>
	Function for_eachItem(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pElement, Function fn)const;

	//writing sum to XML
	void writeSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement)const;
	void writeNettoSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement)const {
		helpfulness::addEndElement(doc, "Suma_netto", helpfulness::toStringPrecision2(sum - PTUSum).c_str(), pPrevElement);
	}
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

	//void pushItem(std::tuple<size_t, unsigned int, float> &&commodityRef) { commodityRefs.push_back(std::move(commodityRef)); } //idea for future? cont

	//create XML document
	virtual bool createDocument()const;
	inline bool saveXML(tinyxml2::XMLDocument & doc, const char *docName)const;
};

template<typename Function>
inline Function Receipt::for_eachItem(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pElement, Function fn) const
{
	using namespace tinyxml2;

	XMLElement * pItemsElement;

	for (auto item : items)
	{
		pItemsElement = doc.NewElement("Pozycja");
			fn(doc, pItemsElement, item);
		pElement->InsertEndChild(pItemsElement);
	}

	return std::move(fn);
}

#endif // !RECEIPT_HPP
