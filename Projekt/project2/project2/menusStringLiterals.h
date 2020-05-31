#ifndef MENUSSTRINGLITERALS_H
#define MENUSSTRINGLITERALS_H

/**
 @namespace menusStrings
 @brief String literals for program menus
 */
namespace menusStrings {
	/// TinyBusiness logo
	constexpr const char *logo =
		" _____  _            _____            _                     \n"
		"|_   _||_| ___  _ _ | __  | _ _  ___ |_| ___  ___  ___  ___ \n"
		"  | |  | ||   || | || __ -|| | ||_ -|| ||   || -_||_ -||_ -|\n"
		"  |_|  |_||_|_||_  ||_____||___||___||_||_|_||___||___||___|\n"
		"               |___|                                        \n"
		"____________________________________________________________________________________________________________\n";

	/// Main menu options
	constexpr const char *mainMenu =
		"MENU GLOWNE\n"
		"--------------------------------------\n"
		"1. Dane Twojej firmy.\n"
		"2. Wprowadz towar/uslugi z faktury.\n"
		"3. Wystaw paragon.\n"
		"4. Wystaw fakture.\n"
		"5. Wygeneruj raport dobowy.\n"
		"6. Zakoncz prace programu.\n"
		"--------------------------------------\n";

	/// Receipt menu options
	constexpr const char *receiptMenu =
		"MENU WYSTAWIANIA PARAGONU\n"
		"--------------------------------------\n"
		"1. Dodaj pozycje do paragonu.\n"
		"2. Zatwierdz paragon.\n"
		"3. Anuluj wystawianie paragonu.\n"
		"--------------------------------------\n";

	/// Invoice menu options
	constexpr const char *invoiceMenu =
		"MENU WYSTAWIANIA FAKTURY\n"
		"--------------------------------------\n"
		"1. Wprowadz dane kupujacego.\n"
		"2. Dodaj pozycje do faktury.\n"
		"3. Zatwierdz fakture.\n"
		"4. Anuluj wystawianie faktury.\n"
		"--------------------------------------\n";
}

#endif // !MENUSTRINGLITERALS_H
