#include "interface.hpp"
extern "C" {
#include "windowsFunctions.h"
}
#include "menusStringLiterals.h"

void Interface::mainMenu()
{
	bool end = false;

	while (!end) {
		ClearScreen();
		std::cout << menusStrings::logo << menusStrings::mainMenu
			<< "Wybierz: ";
		char choice = getCharFromCin();

		mainMenuSwitch(choice, end);
	}
}

inline void Interface::mainMenuSwitch(char choice, bool & end)
{
	switch (choice)
	{
	case '1':
		return changeOwnerData();
	case '2':
		return addFromInvoice();
	case '3':
		return receiptIssueMenu();
	case '4':
		return invoiceIssueMenu();
	case '5':
		return dailyReport();
	case EOF:
		std::cout << "Bledny format danych.\n";
	case '6':
		end = true;
		return db.writeBase(dbFileName);
	}
}

inline void Interface::changeOwnerData()
{
	ClearScreen();
	std::cout << menusStrings::logo << "Dane Twojej firmy\n"
		<< "Obecne dane:\n"
		<< (*db.pOwner())
		<< "Zmienic? [T/n]: ";
	char choice = getCharFromCin();

	if (choice == 'T' || choice == 't')
		db.createOwner();
}

inline void Interface::addFromInvoice()
{
	std::string path;
	std::cout << "Podaj sciezke z faktura: ";
	std::getline(std::cin, path);

	bool result = db.loadFromXMLInvoice(path);
	if (!result) {
		std::cout << "Nie da sie odczytac z pliku. [ENTER]";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void Interface::receiptIssueMenu()
{
	bool result = checkStock();
	if (!result) return;

	Receipt re(db.pOwner());

	bool end = false;
	while (!end) {
		ClearScreen();
		std::cout << menusStrings::logo << menusStrings::receiptMenu
			<< "Wybierz: ";
		char choice = getCharFromCin();

		receiptIssueMenuSwitch(choice, end, re);
	}
}

void Interface::invoiceIssueMenu()
{
	bool result = checkStock();
	if (!result) return;

	Invoice inv(db.pOwner(), db.getInvoiceNo());

	bool end = false;
	bool buyerCreated = false;
	while (!end) {
		ClearScreen();
		std::cout << menusStrings::logo << menusStrings::invoiceMenu
			<< "Wybierz: ";
		char choice = getCharFromCin();

		invoiceIssueMenuSwitch(choice, end, buyerCreated, inv);
		
	}
}

inline void Interface::receiptIssueMenuSwitch(char choice, bool & end, Receipt &re)
{
	switch (choice) {
	case '1':
		return addItem(re);
	case '2':
		confirmDocument(re);
	case '3':
		end = true;
	}
}

inline void Interface::invoiceIssueMenuSwitch(char choice, bool & end, bool &buyerCreated, Invoice &inv)
{
	switch (choice) {
	case '1':
		buyerCreated = true;
		return inv.createBuyer();
	case '2':
		return addItem(inv);
	case '3':
		if (!buyerCreated)
			return buyerNotCreated();
		confirmDocument(inv);
	case '4':
		end = true;
	}
}

inline void Interface::buyerNotCreated() const
{
	std::cout << "Wprowadz dane kupujacego przed wystawieniem faktury. [ENTER]";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline bool Interface::checkStock() const
{
	bool result = db.checkStock();
	if (!result) {
		std::cout << "Brak towaru/uslug na stanie.\n"
			<< "Umiesc towar, aby moc go sprzedac. [ENTER]";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}

	return true;
}

inline void Interface::addItem(Receipt & re)
{
	ShowStock();

	unsigned int index = getNumberFromCin<unsigned int>("Indeks pozycji do dodania [0 aby anulowac dodawanie]: ");

	if (index == 0) return; //cancel adding
	--index; //index starts from zero

	unsigned int quantity = getNumberFromCin<unsigned int>("Ilosc: ");

	bool check;
	try {
		check = db.checkItem(index, quantity);
	}
	catch (const std::out_of_range&) {
		std::cout << "Nie ma takiej pozycji! [ENTER]";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return;
	}

	if(!check)
		checkItemQuantity(index, quantity);
	
	float price = getNumberFromCin<float>("Cena sprzedazy [PLN]: ");

	re.pushItem(db[index], quantity, price);
}

inline char Interface::getCharFromCin() const
{
	char character;
	std::cin >> character;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return character;
}

inline void Interface::ShowStock() const
{
	ClearScreen();
	std::cout << "Stan:\n";
	db.ShowStock();
}

inline void Interface::checkItemQuantity(unsigned int index, unsigned int quantity) const
{
	do {
		std::cout << "Nie ma takiej ilosci na stanie\n";
		quantity = getNumberFromCin<unsigned int>("Wprowadz inna ilosc: ");
	} while (!db.checkItem(index, quantity));
}

inline void Interface::confirmDocument(Receipt & re)
{
	bool result = re.createDocument();
	db.addSum(re.getSum(), re.getPTUSum());
	if (result)
		db.remove(re.cbegin(), re.cend()); //remove from stock
}