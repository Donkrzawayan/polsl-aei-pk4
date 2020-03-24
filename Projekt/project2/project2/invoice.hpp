#ifndef INVOICE_HPP
#define INVOICE_HPP
#include "receipt.hpp"

class Invoice :	public Receipt
{
	Party billTo;
public:
	Invoice();
	virtual ~Invoice() = default;
};

#endif // !INVOICE_HPP
