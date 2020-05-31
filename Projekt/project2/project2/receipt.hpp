#ifndef RECEIPT_HPP
#define RECEIPT_HPP
#include "party.hpp"
#include <vector>
#include "item.hpp"
#include <utility> //move

/// Class with receipt data
class Receipt
{
	const Party *owner; ///< Const pointer to owner data
	std::vector<Item> items;
	double sum, PTUSum;
public:
	/** @param owner pointer to owner data */
	Receipt(const Party *owner) : owner(owner), sum(0.0), PTUSum(0.0) {}
	virtual ~Receipt() = default;

	//getters for sum
	double getSum() { return sum; }
	double getPTUSum() { return PTUSum; }

	/// Push item to items
	void pushItem(const Item &item, unsigned int quantity, float price);

	/// const_iterator for items forwarding
	auto cbegin() { return items.cbegin(); }
	/// const_iterator for items forwarding
	auto cend() { return items.cend(); }

	/// Create XML document
	virtual bool createDocument()const;
protected:
	void addSellerDataToDocument(XMLDoc & doc)const;
	void addItemsToDocument(XMLDoc &doc)const;
	void addSumToDocument(XMLDoc &doc)const;

	/// Writing seller to XML
	void writeSellerXML(XMLDoc & doc)const { owner->writeXML(doc); }

	/** @brief Apply writing type Items to XML
	 @return fn
	*/
	template<typename Function>
	Function for_eachItem(XMLDoc & doc, Function fn)const;

	/// Writing sum to XML
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
