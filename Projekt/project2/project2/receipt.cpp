#include "receipt.hpp"
#include "tinyxml2/tinyxml2.h"
#include <ctime>
#include "helpfulness.hpp"

inline std::string Receipt::date(const char delim) const
{
	std::string date;
	const time_t t = time(NULL);

	date += std::to_string(localtime(&t)->tm_year + 1900);
	if (delim != '\0') date += delim;
	date += std::to_string(localtime(&t)->tm_mon + 1);
	if (delim != '\0') date += delim;
	date += std::to_string(localtime(&t)->tm_mday);
	
	return date;
}

inline std::string Receipt::hour(const char delim) const
{
	std::string hour;
	const time_t t = time(NULL);

	hour += std::to_string(localtime(&t)->tm_hour);
	if (delim != '\0') hour += delim;
	hour += std::to_string(localtime(&t)->tm_min);

	return hour;
}

//inline void Receipt::writeSellerXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pElement) const
//{
//	//using namespace tinyxml2;
//
//	//pElement = doc.NewElement("Dane_sprzedawcy"); //czemu to nie dziala????????????????????????????????????????
//	owner->writeXML(doc, pElement);
//}

inline void Receipt::writeSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement) const
{
	using namespace tinyxml2;

	XMLElement * pElement;

	pElement = doc.NewElement("Suma_PLN");
	pElement->SetText(helpfulness::toStringPrecision2(sum).c_str());
	pPrevElement->InsertEndChild(pElement);

	pElement = doc.NewElement("Suma_PTU");
	pElement->SetText(helpfulness::toStringPrecision2(PTUSum).c_str());
	pPrevElement->InsertEndChild(pElement);
}

void Receipt::writeNettoSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement) const
{
	using namespace tinyxml2;

	XMLElement * pElement = doc.NewElement("Suma_netto");
	pElement->SetText(helpfulness::toStringPrecision2(sum - PTUSum).c_str());
	pPrevElement->InsertEndChild(pElement);
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
		XMLElement * pElement;

		pElement = doc.NewElement("Data");
		pElement->SetText((date('.') + ' ' + hour(':')).c_str());
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Dane_sprzedawcy");
		writeSellerXML(doc, pElement);
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Pozycje_na_paragonie");
		for_eachItem(doc, pElement,
			[](XMLDocument &doc, XMLElement * pElement, const Item &i) { i.writeXML(doc, pElement); });
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Suma");
		writeSumXML(doc, pElement);
		pRoot->InsertEndChild(pElement);
	}

	doc.InsertFirstChild(doc.NewDeclaration()); //add <?xml version="1.0" encoding="UTF-8"?>

	XMLError result = doc.SaveFile((date() + hour() + ".xml").c_str());
	return result == XML_SUCCESS;
}
