
#include "invoice.hpp"
#include <iostream>
#include "tinyxml2/tinyxml2.h"
#include <string>
#include <utility> //move
#include "helpfulness.hpp"

void Invoice::writeSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement) const
{
	Receipt::writeSumXML(doc, pPrevElement);
	writeNettoSumXML(doc, pPrevElement);
}

void Invoice::createBuyer()
{
	std::cout << "Dane kupujacego.\n";
	contractor.createParty();
}

bool Invoice::createDocument() const
{
	using namespace tinyxml2;

	XMLDocument doc;

	XMLNode *pRoot = doc.NewElement("Faktura");

	doc.InsertFirstChild(pRoot);
	{
		XMLElement * pElement;

		pElement = doc.NewElement("Informacje_o_dokumencie");
		{
			XMLElement * pInfoElement;

			pInfoElement = doc.NewElement("Nr");
			pInfoElement->SetText(("FV" + std::move(helpfulness::date()) + std::move(std::to_string(invoiceNo))).c_str()); //performance FTW
			pElement->InsertEndChild(pInfoElement);

			pInfoElement = doc.NewElement("Data_wystawienia");
			pInfoElement->SetText(helpfulness::date('.').c_str());
			pElement->InsertEndChild(pInfoElement);

			pInfoElement = doc.NewElement("Godzina");
			pInfoElement->SetText(helpfulness::hour(':').c_str());
			pElement->InsertEndChild(pInfoElement);
		}
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Dane_sprzedawcy");
		writeSellerXML(doc, pElement);
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Dane_kupujacego");
		{
			contractor.writeXML(doc, pElement);
		}
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Pozycje_na_paragonie");
		for_eachItem(doc, pElement,
			[](XMLDocument &doc, XMLElement * pElement, const Item &i) {i.writeXML(doc, pElement); i.writeNettoXML(doc, pElement); });
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Suma");
		writeSumXML(doc, pElement);
		pRoot->InsertEndChild(pElement);
	}

	doc.InsertFirstChild(doc.NewDeclaration()); //add <?xml version="1.0" encoding="UTF-8"?>

	XMLError result = doc.SaveFile(("FV" + std::move(helpfulness::date()) + std::move(std::to_string(invoiceNo)) + ".xml").c_str());
	return result == XML_SUCCESS;
}
