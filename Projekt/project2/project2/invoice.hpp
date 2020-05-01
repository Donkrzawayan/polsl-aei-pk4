#ifndef INVOICE_HPP
#define INVOICE_HPP
#include "receipt.hpp"

class Invoice :	virtual public Receipt
{
	Party contractor;
	unsigned int invoiceNo; //number of invoice
protected:
	void writeDocInfoXML(XMLDoc & doc)const;
public:
	Invoice(const Party *owner, unsigned int invoiceNo) : Receipt(owner), invoiceNo(invoiceNo) {}

	//set contractor from user
	void createBuyer();
	
	//create XML document
	virtual bool createDocument()const override;
protected:
	void addDocInfoToDocument(XMLDoc & doc)const;
	void addBuyerDataToDocument(XMLDoc & doc)const;
	void addItemsToDocument(XMLDoc &doc)const;
	void addSumToDocument(XMLDoc &doc)const;
};

#endif // !INVOICE_HPP
