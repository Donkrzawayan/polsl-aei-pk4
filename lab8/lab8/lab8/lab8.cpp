
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

void wypiszZnalezione(string str, regex reg) {
	smatch matches;

	while (regex_search(str, matches, reg)) {
		cout << "Znaleziono:  " << matches.str(0) << "\n";
		str = matches.suffix().str();
	}
}

void wypiszZnalezioneIter(string str, regex reg) {
	ofstream plik;
	plik.open("Wynik.txt", ios::trunc);
	if (!plik.is_open())
		return;
	plik << "\n\n";

	sregex_iterator current(str.begin(), str.end(), reg);
	sregex_iterator end;

	while (current != end) {
		smatch match = *current;
		plik << match.str() << "\n";
		++current;
	}

	plik << "\n";
	plik.close();
}

void wczytajTekst(vector<string>& vtekst) {
	fstream plik("Tekst.txt");
	string linia;
	while (getline(plik, linia))
		vtekst.push_back(linia);
	plik.close();
}

class Strona {
	string adres;
	string etykieta;
	string opis;
public:
	const string getAdres()const { return adres; }
	const string getOpis()const { return opis; }
	const string getEtykieta()const { return etykieta; }
	Strona(string adres = "", string etykieta = "", string opis = "") : adres(adres), etykieta(etykieta), opis(opis) {}
	void wypisz()const { cout << etykieta << "\n" << adres << " \n" << opis << "\n\n"; }
};

void oczyscTekst(vector<string>& vec) {
	regex reg("spacja");
	string replace_with = " ";
	for(auto &i: vec)
		i = regex_replace(i, reg, replace_with);

	regex reg2("^(.*?)@");
	for (auto &i: vec) {
		smatch match;
		regex_search(i, match, reg2);
		if (!match.empty())
			i = match.str(1);
	}
}

void wypisz(const vector<string>& vec) {
	for (const auto &i : vec)
		cout << i << "\n";
}

string wytnijOpis(const string& str) {
	regex reg1("(.*?)\\d{4}");
	smatch match;

	regex_search(str, match, reg1);
	if (!match.empty())
		return match.str(1);
	return " ";
}

string wytnijAdres(const string& str) {
	smatch match;
	regex regx("//(.*)");

	regex_search(str, match, regx);
	if (!match.empty())
		return match.str(1);
	return " ";
}

string wytnijEtykiete(const string& str) {
	regex reg1("<(.*?)>");
	smatch match;

	regex_search(str, match, reg1);
	if (!match.empty())
		return match.str(1);
	return (string)" ";
}

void usun_wadliwe(list<Strona*>& lista) {
	regex reg1("(https://)?www\.[a-zA-Z]([\\w-]{1,18})?[a-zA-Z]\.(com|pl)");

	for (auto i = lista.begin(); i != lista.end(); ++i) {
		smatch match;
		string str = (*i)->getAdres();

		regex_search(str, match, reg1);
		if (match.empty() || ((*i)->getEtykieta() == "")) {
			delete *i;
			i = lista.erase(i);
		}
	}
}

void usunListe(list<Strona*>& lista) {
	while (!lista.empty()) {
		delete lista.front();
		lista.pop_front();
	}
}

int main() {
	{ //Zad 1
		string nr = "111 111 111 222-222-222";
		regex reg("\\d{3}[ -]\\d{3}[ -]\\d{3}");
		wypiszZnalezioneIter(nr, reg);

		string s1 = "abaacadaeaaaafag";
		std::regex reg1("(a[^a])");
		smatch matches1;

		int i = 0;
		while (regex_search(s1, matches1, reg1)) {
			++i;
			s1 = matches1.suffix().str();
		}
		cout << "Znaleziono " << i << " dopasowan\n";
	}
	{ //Zad 2
		std::vector<string> tekst;

		wczytajTekst(tekst);
		oczyscTekst(tekst);
		wypisz(tekst);
	//Zad 3
		list<Strona*> lista;

		for (auto i : tekst)
			lista.push_front(new Strona(wytnijAdres(i), wytnijEtykiete(i), wytnijOpis(i)));

		cout << "Lista przed:\n";
		for (const auto &i : lista)
			i->wypisz();

		usun_wadliwe(lista);

		cout << "Lista po:\n";
		for (const auto &i : lista)
			i->wypisz();

		usunListe(lista);
	}

	return 0;
}