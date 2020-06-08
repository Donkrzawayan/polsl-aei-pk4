#ifndef INTERFACEE_HPP
#define INTERFACE_HPP
#include <string>
#include "dataManager.hpp"
#include "invoice.hpp"
#include "dailyRaport.hpp"

/// Creates interface for user
class Interface
{
	std::string dFileName;
	DataManager dmngr;
	DailyRaport dRaport;


public:
	/** @param dFileName name of file with user data */
	Interface(std::string dFileName) : dFileName(dFileName), dmngr(dFileName) {}
	void mainMenu(); ///< main loop
private:
	inline char getCharFromCin()const;
	inline void mainMenuSwitch(char choice, bool &end);

	inline void changeOwnerData();

	inline void addFromInvoice(); ///< Add items from invoice to dataManager
	inline void addItemsFromInvoice();

	void receiptIssueMenu(); ///< Receipt menu
	void invoiceIssueMenu(); ///< Invoice menu

	inline void receiptIssueMenuSwitch(char choice, bool &end, Receipt &re);
	inline void invoiceIssueMenuSwitch(char choice, bool &end, bool &buyerCreated, Invoice &inv);
	inline void buyerNotCreated()const;
	inline bool checkStock()const; ///< Check if there is anything in the stock
	inline void addItem(Receipt &re); ///< Add chosen by user item from dataManager to receipt/invoice
	inline void addItemFromUser(Receipt &re);
	
	inline void ShowStock()const;
	inline void checkAndRepairItemQuantity(unsigned int index, unsigned int quantity)const;
	inline void confirmDocument(Receipt &re);

	template <typename T>
	T getNumberFromCin(const char *statement)const;

	template<typename T>
	void checkAndRepairCin(T &t)const;

	void dailyRaport() { dRaport.generate(); }
};

template<typename T>
void Interface::checkAndRepairCin(T &t) const
{
	using std::cin;

	while (cin.fail() && !cin.eof()) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Wprowadzono nieprawidlowy format. Wprowadz jeszcze raz: ";
		cin >> t;
	}
}

template<typename T>
T Interface::getNumberFromCin(const char *statement) const
{
	std::cout << statement;
	T number;
	std::cin >> number;
	checkAndRepairCin(number);
	return number;
}

#endif // !INTERFACEE_HPP
