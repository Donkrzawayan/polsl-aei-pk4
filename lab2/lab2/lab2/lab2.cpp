// lab2.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <typeinfo>

//ZAD 1

class TBladMatematyczny : public std::exception
{
public:
	void what(){ std::cout << "TBladMatematyczny\n"; }
};

//******************************************************************************

//ZAD 2
class TDzielPrzezZero :public TBladMatematyczny
{
public:
	void what() { std::cout << "TDzielPrzezZero\n"; }
};


//******************************************************************************

//ZAD 3
class TSsak
{
	int waga;
public:
	TSsak(int w=0) : waga(w) {}
	virtual ~TSsak(){}
	int get_waga() { return waga; }
};


//******************************************************************************

//ZAD 4
class TZwierze: virtual public TSsak
{
	int dlugosc;
public:
	TZwierze(int w=1, int dl=2) : TSsak(w), dlugosc(dl) {}
	virtual ~TZwierze() {}
};


//******************************************************************************

//ZAD 5
double dziel(int x, int y) {
	if (y != 0)
		return (double)(x / y);
	else
		throw TDzielPrzezZero();
}

//******************************************************************************

int main()
{
	//ZAD 6

	std::cout << "Zad 6: \n";
	try {
		std::cout << dziel(0, 0) << std::endl;
	}
	catch (TBladMatematyczny &e) { //zlapany
		e.what();
	}
	catch (TDzielPrzezZero &e) {
		e.what();
	}
	catch (...) {

	}

	std::cout << "\n\n";

	//ZAD 7

	TSsak * tab_pSsak[8]{ new TSsak, new TSsak, new TSsak, new TSsak, new TZwierze, new TZwierze, new TZwierze, nullptr };

	//ZAD 8

	std::cout << "Zad 8: \n";

	for (auto ssak : tab_pSsak)
	{
		try {
			std::cout << typeid(*ssak).name() << std::endl;
		}
		catch (std::bad_typeid &e) {}
	}

	std::cout << "\n\n";

	//ZAD 9

	std::cout << "Zad 9: \n";

	for (auto ssak : tab_pSsak)
	{
		if (ssak) {
			try {
				TZwierze & ss = dynamic_cast<TZwierze &>(*ssak);

				std::cout << "Waga: " << ss.get_waga() << std::endl;
			}
			catch (std::bad_cast &e) {}
		}
	}

	std::cout << "\n\n";

}