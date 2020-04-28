#ifndef INVOICE_HPP
#define INVOICE_HPP
#include "receipt.hpp"

class Invoice :	virtual public Receipt
{
	Party contractor;
	unsigned int invoiceNo; //number of invoice
protected:
	//writing sum to XML
	void writeSumXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement)const;
public:
	Invoice(const Party *owner, unsigned int invoiceNo) : Receipt(owner), invoiceNo(invoiceNo) {}

	//set contractor from user
	void createBuyer();
	
	//create XML document
	virtual bool createDocument()const override;
};

#endif // !INVOICE_HPP
