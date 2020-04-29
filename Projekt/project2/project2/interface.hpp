#ifndef INTERFACEE_HPP
#define INTERFACE_HPP
#include <string>
#include "dataBase.hpp"
#include "invoice.hpp"

class Interface
{
	std::string dbFileName;
	DataBase db;
private:
	template<typename T>
	void checkAndRepairCin(T &t)const;

	template <typename T>
	T getNumberFromCin(const char *statement)const;

	inline char getCharFromCin()const;
public:
	Interface(std::string dbFileName) : dbFileName(dbFileName), db(dbFileName) {}
	void mainMenu(); //main loop
private:
	inline void mainMenuSwitch(char choice, bool &end);

protected:
	inline void changeOwnerData();

protected:
	inline void addFromInvoice(); //add items from invoice to database

protected:
	void receiptIssueMenu();
	void invoiceIssueMenu();
private:
	inline void receiptIssueMenuSwitch(char choice, bool &end, Receipt &re);
	inline void invoiceIssueMenuSwitch(char choice, bool &end, bool &buyerCreated, Invoice &inv);
	inline void buyerNotCreated()const;
	inline bool checkStock()const; //check if there is anything in the stock
	inline void addItem(Receipt &re); //add item from user to receipt/invoice
	
	inline void ShowStock()const;
	inline void checkItemQuantity(unsigned int index, unsigned int quantity)const;
	inline void confirmDocument(Receipt &re);

protected:
	void dailyReport() { db.dailyRaport(); }
};

template<typename T>
inline void Interface::checkAndRepairCin(T &t) const
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
