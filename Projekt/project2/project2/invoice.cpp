
#include "invoice.hpp"
#include <iostream>
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
			helpfulness::addEndElement(doc, "Nr",
				("FV" + std::move(helpfulness::date()) + std::move(std::to_string(invoiceNo))).c_str(),
				pElement);
			helpfulness::addEndElement(doc, "Data_wystawienia", helpfulness::date('.').c_str(), pElement);
			helpfulness::addEndElement(doc, "Godzina", helpfulness::hour(':').c_str(), pElement);
		}
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Dane_sprzedawcy");
		writeSellerXML(doc, pElement);
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Dane_kupujacego");
			contractor.writeXML(doc, pElement);
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Pozycje_na_paragonie");
		for_eachItem(doc, pElement,
			[](XMLDocument &doc, XMLElement * pElement, const Item &item) {item.writeXML(doc, pElement); item.writeNettoXML(doc, pElement); });
		pRoot->InsertEndChild(pElement);

		pElement = doc.NewElement("Suma");
		writeSumXML(doc, pElement);
		pRoot->InsertEndChild(pElement);
	}

	return saveXML(doc, ("FV" + std::move(helpfulness::date()) + std::move(std::to_string(invoiceNo)) + ".xml").c_str());
}
