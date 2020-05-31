#ifndef INVOICE_HPP
#define INVOICE_HPP
#include "receipt.hpp"

/// Class with invoice data
class Invoice :	virtual public Receipt
{
	Party contractor;
	unsigned int invoiceNo; ///< Number of invoice
protected:
	void writeDocInfoXML(XMLDoc & doc)const;
public:
	/**
	 @param owner pointer to owner data
	 @param invoiceNo number of invoice
	 */
	Invoice(const Party *owner, unsigned int invoiceNo) : Receipt(owner), invoiceNo(invoiceNo) {}

	/// Set contractor from user
	void createBuyer();
	
	/// Create XML document
	virtual bool createDocument()const override;
protected:
	void addDocInfoToDocument(XMLDoc & doc)const;
	void addBuyerDataToDocument(XMLDoc & doc)const;
	void addItemsToDocument(XMLDoc &doc)const;
	void addSumToDocument(XMLDoc &doc)const;
};

#endif // !INVOICE_HPP
