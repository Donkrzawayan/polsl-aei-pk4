#include "item.hpp"
#include <iomanip> //std::setw
#include <utility> //move

void Item::show(const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth) const
{
	
	//setting floating point values
	const auto prec = std::cout.precision(2);
	const auto fmtfl = std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield); //fixed-point notation

	std::cout << std::left << std::setw(descriptionWidth) << description;
	std::cout << std::right << std::setw(quantityWidth) << quantity;
	std::cout << std::right << std::setw(spriceWidth) << salesPrice;
	std::cout << std::right << std::setw(vatWidth) << vat << "%";

	std::cout.setf(fmtfl, std::ios_base::floatfield);
	std::cout.precision(prec);
	
}

std::istream & Item::read(std::istream & is)
{
	helpfulness::readStringBin(is, description);
	is.read(reinterpret_cast<char *>(&quantity), sizeof(quantity));
	is.read(reinterpret_cast<char *>(&vat), sizeof(vat));
	is.read(reinterpret_cast<char *>(&salesPrice), sizeof(salesPrice));
	is.read(reinterpret_cast<char *>(&purchasePrice), sizeof(purchasePrice));

	return is;
}

std::ostream & Item::write(std::ostream & os) const
{
	helpfulness::writeStringBin(os, description);
	os.write(reinterpret_cast<const char *>(&quantity), sizeof(quantity));
	os.write(reinterpret_cast<const char *>(&vat), sizeof(vat));
	os.write(reinterpret_cast<const char *>(&salesPrice), sizeof(salesPrice));
	os.write(reinterpret_cast<const char *>(&purchasePrice), sizeof(purchasePrice));

	return os;
}

void Item::writeXML(XMLDoc & doc) const
{
	doc.addElement("Nazwa", description.c_str());
	doc.addElement("Ilosc", quantity);
	doc.addElement("VAT_proc.", vat);
	doc.addElement("Cena_brutto", helpfulness::toStringPrecision2(salesPrice).c_str());
}

void Item::readXML(XMLDoc & doc)
{
	description = std::move(doc.getText("Nazwa"));
	quantity = doc.getInt("Ilosc");
	vat = doc.getInt("VAT_proc.");
	purchasePrice = doc.getFloat("Cena_brutto");
}
