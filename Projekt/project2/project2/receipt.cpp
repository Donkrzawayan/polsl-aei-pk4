#include "receipt.hpp"

//inline void Receipt::writeSellerXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pElement) const
//{
//	//using namespace tinyxml2;
//
//	//pElement = doc.NewElement("Dane_sprzedawcy"); //czemu to nie dziala????????????????????????????????????????
//	owner->writeXML(doc, pElement);
//}

inline void Receipt::writeSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement) const
{
	helpfulness::addEndElement(doc, "Suma_PLN", helpfulness::toStringPrecision2(sum).c_str(), pPrevElement);
	helpfulness::addEndElement(doc, "Suma_PTU", helpfulness::toStringPrecision2(PTUSum).c_str(), pPrevElement);
}

void Receipt::pushItem(const Item & c, unsigned int quantity, float price)
{
	items.push_back(c);
	items.back().setQuantity(quantity);
	items.back().setSalesPrice(price);
	sum += quantity * price;
	PTUSum += (items.back().getSalesPrice() - items.back().getNettoPrice()) * quantity;
}

bool Receipt::createDocument() const
{
	using namespace tinyxml2;

	XMLDocument doc;
	
	XMLNode *pRoot = doc.NewElement("Paragon");

	doc.InsertFirstChild(pRoot);
	{
		helpfulness::addEndElement(doc, "Data", (helpfulness::date('.') + ' ' + helpfulness::hour(':')).c_str(), pRoot);

		XMLElement * pElement;

		pElement = doc.NewElement("Dane_sprzedawcy");
		writeSellerXML(doc, pElement);
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Pozycje_na_paragonie");
		for_eachItem(doc, pElement,
			[](XMLDocument &doc, XMLElement * pElement, const Item &item) { item.writeXML(doc, pElement); });
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Suma");
		writeSumXML(doc, pElement);
		pRoot->InsertEndChild(pElement);
	}

	return saveXML(doc, (helpfulness::date() + helpfulness::hour() + ".xml").c_str());
}

inline bool Receipt::saveXML(tinyxml2::XMLDocument & doc, const char * docName) const
{
	doc.InsertFirstChild(doc.NewDeclaration()); //add <?xml version="1.0" encoding="UTF-8"?>

	tinyxml2::XMLError result = doc.SaveFile(docName);
	return result == tinyxml2::XML_SUCCESS;
}
