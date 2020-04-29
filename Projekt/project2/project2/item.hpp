#ifndef ITEM_HPP
#define ITEM_HPP
#include <string>
#include <iostream> //streamsize
#include "tinyxml2/tinyxml2.h"
#include "helpfulness.hpp"

class Item
{
	std::string description;
	int quantity;
	int vat;
	float salesPrice, purchasePrice;
public:
	Item() = default;
	Item(std::string &&description, unsigned int quantity, int vat, float salesPrice, float purchasePrice = 0.0f)
		: description(description), quantity(quantity), vat(vat), salesPrice(salesPrice), purchasePrice(purchasePrice) {}
	
	//getters
	int getQuantity()const { return quantity; }
	float getPurchasePrice()const { return purchasePrice; }
	float getSalesPrice()const { return salesPrice; }
	double getNettoPrice()const { return 100.0 * salesPrice / (100.0 + vat); }
	int getVAT()const { return vat; }

	//setters
	void setQuantity(unsigned int qty) { quantity = qty; }
	void setPurchasePrice(float pprice) { purchasePrice = pprice; }
	void setSalesPrice(float sprice) { salesPrice = sprice; }

	//relational operators
	friend bool operator==(const Item &i1, const Item &i2) { return (i1.description == i2.description && i1.vat == i2.vat); }
	friend bool operator<(const Item &i1, const Item &i2) { return i1.description < i2.description; }

	//adding and substructing quantity
	Item &operator+=(unsigned int ui) { quantity += ui; return *this; }
	Item &operator-=(unsigned int ui) { quantity -= ui; return *this; }

	//reading, showing, writing
	void Show(const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth)const;
	std::istream &read(std::istream & is);
	std::ostream &write(std::ostream & os)const;
	void writeXML(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement * pPrevElement)const;
	void writeNettoXML(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement * pPrevElement)const {
		helpfulness::addEndElement(doc, "Cena_netto", helpfulness::toStringPrecision2(getNettoPrice()).c_str(), pPrevElement);
	}
	bool readXML(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement * pPrevElement);
};

#endif // !ITEM_HPP
