#ifndef ITEM_HPP
#define ITEM_HPP
#include <string>
#include <iostream> //streamsize
#include "helpfulness.hpp"
#include "XMLDoc.hpp"

/// Selling item data
class Item
{
	std::string description; ///< Description of Item
	int quantity;
	int vat;
	float salesPrice, purchasePrice; ///< Brutto prices
public:
	Item() = default;
	
	//getters
	int getQuantity()const { return quantity; }
	float getPurchasePrice()const { return purchasePrice; }
	float getSalesPrice()const { return salesPrice; }
	double getNettoPrice()const { return 100.0 * salesPrice / (100.0 + vat); }
	int getVAT()const { return vat; }

	//setters
	void setSaleData(unsigned int qty, float sPrice) { quantity = qty; salesPrice = sPrice; }
	void setPurchasePrice(float pPrice) { purchasePrice = pPrice; }
	void setSalesPrice(float sPrice) { salesPrice = sPrice; }

	//relational operators
	friend bool operator==(const Item &i1, const Item &i2) { return (i1.description == i2.description && i1.vat == i2.vat); }
	friend bool operator<(const Item &i1, const Item &i2) { return i1.description < i2.description; }

	//adding and substructing quantity
	Item &operator+=(unsigned int ui) { quantity += ui; return *this; }
	Item &operator-=(unsigned int ui) { quantity -= ui; return *this; }

	//reading, showing, writing

	/** @param descriptionWidth,quantityWidth,spriceWidth,vatWidth Set width of specified column */
	void Show(const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth)const;
	std::istream &read(std::istream & is);
	std::ostream &write(std::ostream & os)const;
	void writeXML(XMLDoc &doc)const;
	void writeNettoXML(XMLDoc &doc)const {
		doc.addElement("Cena_netto", helpfulness::toStringPrecision2(getNettoPrice()).c_str());
	}
	void readXML(XMLDoc &doc);
};

#endif // !ITEM_HPP
