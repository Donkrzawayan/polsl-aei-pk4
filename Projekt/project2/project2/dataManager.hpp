#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP
#include <iostream>
#include "party.hpp"
#include <vector>
#include "item.hpp"
#include <algorithm> //sort find

/// Class managing all data saved in binary file
class DataManager
{
private:
	Party owner;
	std::vector<Item> stock;
	unsigned int invoiceNo; ///< Number of next invoice

	long double totalPayment; ///< Daily raport info
	long double totalPTUAmount; ///< Daily raport info
private:
	/// Adding items to base; called from loadFromInvoice
	DataManager &operator+=(Item &&c);
public:
	/** @param dFileName name of file with user data */
	DataManager(std::string dbFileName = "db.bin");


	//owner operations
	void createOwner();
	const Party *pOwner()const { return &owner; } ///< Return pointer to owner data


	//receipt and invoice operations

	/// Getter number of invoice
	unsigned int getInvoiceNo() { return invoiceNo++; }

	/// Check if there is anything in the stock
	bool checkStock()const { return stock.size(); }

	/** Check whenever item exists in sufficient quantity
	@retval false if not enough quantity
	@exception std::out_of_range for out-of-range index
	*/
	bool checkItem(const unsigned int index, const int quantity)const { return stock.at(index).getQuantity() >= quantity; }

	/// Access to elements in stock
	const Item &operator[](size_t n)const { return stock[n]; }

	/** Remove range from stock
	 @param first,last Forward iterators to the initial and final positions in a sequence with items to remove
	*/
	template <typename InputIterator>
	void remove(InputIterator first, InputIterator last);


	//showing, wrighting, reading

	/// Show stock to console
	void ShowStock()const;
	void ShowStockHeader(const std::streamsize noWidth, const std::streamsize descriptionWidth,	const std::streamsize quantityWidth,
		const std::streamsize spriceWidth, const std::streamsize vatWidth)const;
	void ShowStockContent(const std::streamsize noWidth, const std::streamsize descriptionWidth, const std::streamsize quantityWidth,
		const std::streamsize spriceWidth, const std::streamsize vatWidth)const;

	/// Sort and write base to bin
	void sortAndWriteBase(std::string dbFileName = "db.bin");
private:
	/// Read base from bin; called from constructor
	void readBase(std::ifstream &ifs);
	inline void readMonthAndInvoiceNo(std::ifstream &ifs);
	inline void readStock(std::ifstream &ifs);

	/// Write base elements to bin; called from sortAndWriteBase()
	inline void writeMonthAndInvoiceNo(std::ofstream &ofs)const;
	inline void writeStock(std::ofstream &ofs)const;

public:
	/** Load Items from file given by parametr
	 @param docName document name with Items to add
	 */
	bool loadFromXMLInvoice(const std::string &docName);



	//osobna klasa na raport?

	/// Add payments to sum payments
	void addSum(double sum, double PTUSum) { totalPayment += sum, totalPTUAmount += PTUSum; }
	/// Generate daily report file
	bool dailyRaport();
private:
	inline void writeDocumentInfoXML(XMLDoc & doc)const;
	inline void writePaymentXML(XMLDoc & doc)const;
};

template<typename InputIterator>
inline void DataManager::remove(InputIterator first, InputIterator last)
{
	while (first != last) {
		auto it = std::find(stock.begin(), stock.end(), *first);
		if (it != stock.end()) {
			(*it) -= first->getQuantity();
			it->setSalesPrice(first->getSalesPrice()); //update sales price
			if (!it->getQuantity()) //if there is 0 left
				stock.erase(it);
		}
		++first;
	}
}

#endif // !DATABASE_HPP
