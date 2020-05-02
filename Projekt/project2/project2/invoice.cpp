
#include "invoice.hpp"
#include <iostream>
#include <string>
#include <utility> //move

void Invoice::writeDocInfoXML(XMLDoc & doc) const
{
	doc.addElement("Nr",
		("FV" + std::move(helpfulness::date()) + std::move(std::to_string(invoiceNo))).c_str());
	doc.addElement("Data_wystawienia", helpfulness::date('.').c_str());
	doc.addElement("Godzina", helpfulness::hour(':').c_str());
}

void Invoice::createBuyer()
{
	std::cout << "Dane kupujacego.\n";
	contractor.createParty();
}

bool Invoice::createDocument() const {
	XMLDoc doc;
	doc.newDoc("Faktura");

	addDocInfoToDocument(doc);
	addSellerDataToDocument(doc);
	addBuyerDataToDocument(doc);
	addItemsToDocument(doc);
	addSumToDocument(doc);

	return doc.saveXML(("FV" + std::move(helpfulness::date()) + std::move(std::to_string(invoiceNo)) + ".xml").c_str());
}

void Invoice::addDocInfoToDocument(XMLDoc & doc) const
{
	doc.addElement("Informacje_o_dokumencie");
	writeDocInfoXML(doc);
	doc.insertChild();
}

void Invoice::addBuyerDataToDocument(XMLDoc & doc) const
{
	doc.addElement("Dane_kupujacego");
	contractor.writeXML(doc);
	doc.insertChild();
}

void Invoice::addItemsToDocument(XMLDoc & doc) const
{
	doc.addElement("Pozycje");
	for_eachItem(doc,
		[](XMLDoc &doc, const Item &item) { item.writeXML(doc); item.writeNettoXML(doc); });
	doc.insertChild();
}

void Invoice::addSumToDocument(XMLDoc & doc) const
{
	doc.addElement("Suma");
	Receipt::writeSumXML(doc);
	writeNettoSumXML(doc);
	doc.insertChild();
}
