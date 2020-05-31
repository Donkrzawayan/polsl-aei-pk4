#include "receipt.hpp"
#include "XMLDoc.hpp"

void Receipt::writeSumXML(XMLDoc & doc) const
{
	doc.addElement("Suma_PLN", helpfulness::toStringPrecision2(sum).c_str());
	doc.addElement("Suma_PTU", helpfulness::toStringPrecision2(PTUSum).c_str());
}

void Receipt::pushItem(const Item & item, unsigned int quantity, float price)
{
	items.push_back(item);
	items.back().setSaleData(quantity, price);
	sum += quantity * price;
	PTUSum += (items.back().getSalesPrice() - items.back().getNettoPrice()) * quantity;
}

bool Receipt::createDocument() const {
	XMLDoc doc;
	doc.newDoc("Paragon");

	doc.addElement("Data", (helpfulness::date('.') + ' ' + helpfulness::hour(':')).c_str());
	addSellerDataToDocument(doc);
	addItemsToDocument(doc);
	addSumToDocument(doc);

	return doc.saveXML((helpfulness::date() + helpfulness::hour() + ".xml").c_str());
}

void Receipt::addSellerDataToDocument(XMLDoc & doc) const
{
	doc.addElement("Dane_sprzedawcy");
	writeSellerXML(doc);
	doc.insertChild();
}

void Receipt::addItemsToDocument(XMLDoc & doc) const
{
	doc.addElement("Pozycje");
	for_eachItem(doc,
		[](XMLDoc &doc, const Item &item) { item.writeXML(doc); });
	doc.insertChild();
}

void Receipt::addSumToDocument(XMLDoc & doc) const
{
	doc.addElement("Suma");
	writeSumXML(doc);
	doc.insertChild();
}
