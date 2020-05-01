//
#include <iostream>
#include <cstring>
#include "interface.hpp"

void help() {
	std::cout << "POMOC PROGRAMU TINYBUSINESS\n"
		<< "----------------------------------\n"
		<< "Przelaczniki:\n"
		<< "\"-h\", \"-help\" - pomoc\n"
		<< "\"-d\" - nazwa pliku z binarna baza danych.\n"
		<< "\n"
		<< "Aby zakonczyc wpisywac dane pole wystarczy kliknac ENTER.\n"
		<< "Nacisnij ENTER, aby kontynuowac\n";
}

int main(int argc, char *argv[])
{
	std::string dbName{ "bd.bin" };

	for (int i = 1; i < argc; ++i) {
		if (std::strcmp(argv[1], "-h") || (std::strcmp(argv[1], "--help")))
			help();
		else if (std::strcmp(argv[1], "-d"))
			dbName = argv[++i];
	}
	Interface interface(dbName);
	interface.mainMenu();

	return 0;
}
