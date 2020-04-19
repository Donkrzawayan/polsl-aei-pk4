#include "item.hpp"
#include <iomanip> //std::setw
#include "helpfulness.hpp"


void Item::Show(const std::streamsize nameWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth) const
{
	
	//setting floating point values
	const auto prec = std::cout.precision(2);
	const auto fmtfl = std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield); //fixed-point notation

	std::cout << std::left << std::setw(nameWidth) << name;
	std::cout << std::right << std::setw(quantityWidth) << quantity;
	std::cout << std::right << std::setw(spriceWidth) << salesPrice;
	std::cout << std::right << std::setw(vatWidth) << vat << "%";

	std::cout.setf(fmtfl, std::ios_base::floatfield);
	std::cout.precision(prec);
	
}

std::istream & Item::read(std::istream & is)
{
	size_t size;

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	name.resize(size);
	is.read(reinterpret_cast<char *>(&name[0]), size * sizeof(name[0]));
	is.read(reinterpret_cast<char *>(&quantity), sizeof(quantity));
	is.read(reinterpret_cast<char *>(&vat), sizeof(vat));
	is.read(reinterpret_cast<char *>(&salesPrice), sizeof(salesPrice));
	is.read(reinterpret_cast<char *>(&purchasePrice), sizeof(purchasePrice));

	return is;
}

std::ostream & Item::write(std::ostream & os) const
{
	const size_t size = name.size();

	os.write(reinterpret_cast<const char *>(&size), sizeof(size))
		.write(name.c_str(), size * sizeof(name[0]));
	os.write(reinterpret_cast<const char *>(&quantity), sizeof(quantity));
	os.write(reinterpret_cast<const char *>(&vat), sizeof(vat));
	os.write(reinterpret_cast<const char *>(&salesPrice), sizeof(salesPrice));
	os.write(reinterpret_cast<const char *>(&purchasePrice), sizeof(purchasePrice));

	return os;
}

void Item::writeXML(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement * pPrevElement) const
{
	using namespace tinyxml2;

	XMLElement * pElement;

	pElement = doc.NewElement("Nazwa");
	pElement->SetText(name.c_str());
	pPrevElement->InsertEndChild(pElement);

	pElement = doc.NewElement("Ilosc");
	pElement->SetText(quantity);
	pPrevElement->InsertEndChild(pElement);

	pElement = doc.NewElement("VAT_proc.");
	pElement->SetText(vat);
	pPrevElement->InsertEndChild(pElement);

	pElement = doc.NewElement("Cena_brutto");
	pElement->SetText(helpfulness::toStringPrecision2(salesPrice).c_str());
	pPrevElement->InsertEndChild(pElement);
}

void Item::writeNettoXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement) const
{
	using namespace tinyxml2;

	XMLElement * pElement = doc.NewElement("Cena_netto");
	pElement->SetText(helpfulness::toStringPrecision2(getNettoPrice()).c_str());
	pPrevElement->InsertEndChild(pElement);
}

bool Item::readXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement)
{
	using namespace tinyxml2;

	XMLElement * pElement;

	pElement = pPrevElement->FirstChildElement("Nazwa");
	if (!pElement) return false; // XML_ERROR_PARSING_ELEMENT;
	name = pElement->GetText();

	pElement = pPrevElement->FirstChildElement("Ilosc");
	if (!pElement) return false; // XML_ERROR_PARSING_ELEMENT;
	pElement->QueryUnsignedText(&quantity);

	pElement = pPrevElement->FirstChildElement("VAT_proc.");
	if (!pElement) return false; // XML_ERROR_PARSING_ELEMENT;
	pElement->QueryIntText(&vat);

	pElement = pPrevElement->FirstChildElement("Cena_brutto");
	if (!pElement) return false; // XML_ERROR_PARSING_ELEMENT;
	pElement->QueryFloatText(&purchasePrice);

	return true;
}
