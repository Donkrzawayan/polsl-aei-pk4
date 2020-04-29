#include "interface.hpp"
extern "C" {
#include "windowsFunctions.h"
}
#include "invoice.hpp"

void Interface::mainMenu()
{
	bool end = false;

	while (!end) {
		ClearScreen();
		std::cout
			<< " _____  _            _____            _                     \n"
			<< "|_   _||_| ___  _ _ | __  | _ _  ___ |_| ___  ___  ___  ___ \n"
			<< "  | |  | ||   || | || __ -|| | ||_ -|| ||   || -_||_ -||_ -|\n"
			<< "  |_|  |_||_|_||_  ||_____||___||___||_||_|_||___||___||___|\n"
			<< "               |___|                                        \n"
			<< "____________________________________________________________________________________________________________\n"
			<< "MENU GLOWNE\n"
			<< "--------------------------------------\n"
			<< "1. Dane Twojej firmy.\n"
			<< "2. Wprowadz towar/uslugi z faktury.\n"
			<< "3. Wystaw paragon.\n"
			<< "4. Wystaw fakture.\n"
			<< "5. Wygeneruj raport dobowy.\n"
			<< "6. Zakoncz prace programu.\n"
			<< "--------------------------------------\n"
			<< "Wybierz: ";
		char choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
	std::cout << "Dane Twojej firmy\n"
		<< "Obecne dane:\n"
		<< (*db.pOwner())
		<< "Zmienic? [T/n]: ";
	char choice;
	std::cin >> choice;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
		std::cout << "MENU WYSTAWIANIA PARAGONU\n"
			<< "--------------------------------------\n"
			<< "1. Dodaj pozycje do paragonu.\n"
			<< "2. Zatwierdz paragon.\n"
			<< "3. Anuluj wystawianie paragonu.\n"
			<< "--------------------------------------\n"
			<< "Wybierz: ";
		char choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		receiptIssueMenuSwitch(choice, end, re);
	}
}

void Interface::invoiceIssueMenu()
{
	bool result = checkStock();
	if (!result) return;

	Invoice inv(db.pOwner(), db.getInvoiceNo());

	bool end = false;
	while (!end) {
		ClearScreen();
		std::cout << "MENU WYSTAWIANIA FAKTURY\n"
			<< "--------------------------------------\n"
			<< "1. Wprowadz dane kupujacego.\n"
			<< "2. Dodaj pozycje do faktury.\n"
			<< "3. Zatwierdz fakture.\n"
			<< "4. Anuluj wystawianie faktury.\n"
			<< "--------------------------------------\n"
			<< "Wybierz: ";
		char choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		invoiceIssueMenuSwitch(choice, end, inv);
		
	}
}

inline void Interface::receiptIssueMenuSwitch(char choice, bool & end, Receipt &re)
{
	switch (choice) {
	case '1':
		return addItem(re);
	case '2':
		return confirmDocument(re);
	case '3':
		end = true;
	}
}

inline void Interface::invoiceIssueMenuSwitch(char choice, bool & end, Invoice &inv)
{
	bool buyerCreated = false;

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

inline bool Interface::checkStock()
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
	ClearScreen();
	std::cout << "Stan:\n";
	db.ShowStock();

	std::cout << "Indeks pozycji do dodania [0 aby anulowac dodawanie]: ";
	unsigned int index;
	std::cin >> index;
	checkCin(index);

	if (index == 0) //cancel adding
		return;

	--index; //index starts from zero

	std::cout << "Ilosc: ";
	unsigned int quantity;
	std::cin >> quantity;
	checkCin(quantity);

	bool check;
	try {
		check = db.checkItem(index, quantity);
	}
	catch (const std::out_of_range&) {
		std::cout << "Nie ma takiej pozycji!\n";
		return;
	}

	while (!check) {
		std::cout << "Nie ma takiej ilosci na stanie\n"
			<< "Wprowadz inna ilosc: ";
		std::cin >> quantity;
		check = db.checkItem(index, quantity);
	}
	std::cout << "Cena sprzedazy [PLN]: ";
	float price;
	std::cin >> price;
	checkCin(price);

	re.pushItem(db[index], quantity, price);
}

inline void Interface::confirmDocument(Receipt & re)
{
	bool result = re.createDocument();
	db.addSum(re.getSum(), re.getPTUSum());
	if (result)
		db.remove(re.cbegin(), re.cend()); //remove from stock
}

void Interface::dailyReport()
{
	db.dailyRaport();
}
