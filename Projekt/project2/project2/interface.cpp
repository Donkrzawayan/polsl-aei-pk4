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
			<< "5. Zakoncz prace programu.\n"
			<< "--------------------------------------\n"
			<< "Wybierz: ";
		char choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		switch (choice)
		{
		case '1': {
			changeOwnerData();
			break;
		}
		case '2': {
			addFromInvoice();
			break;
		}
		case '3': {
			receiptIssueMenu();
			break;
		}
		case '4': {
			invoiceIssueMenu();
			break;
		}
		case EOF:
			std::cout << "Bledny format danych.\n";
		case '5':
			db.writeBase(dbFileName);
			end = true;
			break;
		}
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

		switch (choice) {
		case '1': {
			addItem(re);
			break;
		}
		case '2':
			confirmDocument(re);
		case '3':
			end = true;
			break;
		}
	}
}

void Interface::invoiceIssueMenu()
{
	bool result = checkStock();
	if (!result) return;

	Invoice inv(db.pOwner(), db.getNo());

	bool end = false;
	bool buyerCreated = false;
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

		switch (choice) {
		case '1': {
			inv.createBuyer();
			buyerCreated = true;
			break;
		}
		case '2': {
			addItem(inv);
			break;
		}
		case '3':
			if (!buyerCreated) {
				std::cout << "Wprowadz dane kupujacego przed wystawieniem faktury. [ENTER]";
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			confirmDocument(inv);
		case '4':
			end = true;
			break;
		}
	}
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

	std::cout << "Indeks pozycji do dodania: ";
	unsigned int index;
	std::cin >> index;
	checkCin(index);
	--index; //index starts from zero

	std::cout << "Ilosc: ";
	unsigned int amount;
	std::cin >> amount;
	checkCin(amount);

	bool check;
	try {
		check = db.checkItem(index, amount);
	}
	catch (const std::out_of_range&) {
		std::cout << "Nie ma takiej pozycji!\n";
		return;
	}

	while (!check) {
		std::cout << "Nie ma takiej ilosci na stanie\n"
			<< "Wprowadz inna ilosc: ";
		std::cin >> amount;
		check = db.checkItem(index, amount);
	}
	std::cout << "Cena sprzedazy [PLN]: ";
	float price;
	std::cin >> price;
	checkCin(price);

	re.pushItem(db[index], amount, price);
}

inline void Interface::confirmDocument(Receipt & re)
{
	bool result = re.createDocument();
	if (result)
		db.remove(re.cbegin(), re.cend()); //remove from stock
}
