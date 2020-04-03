
#include <iostream>
#include <vector>
#include <algorithm> // std::max_element
#include <random> //std::mt19937
#include <chrono> //seed
#include <stack>
#include <fstream> //std::fstream
#include <string>
#include <queue>

class Student {
	std::string imie, nazwisko;
	int nr_zestawu;
public:
	Student(const std::string& imie, const std::string &nazwisko)
		: imie(imie), nazwisko(nazwisko), nr_zestawu(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())() % 101) {}
	int pokaz_zestaw() { return nr_zestawu; }
	friend std::ostream& operator<<(std::ostream& stream, const Student& st) {
		return stream << st.imie << " " << st.nazwisko << " zestaw: " << st.nr_zestawu;
	}
};

void f(std::queue<Student> &oczekujacy, std::queue<Student> &zdali, std::queue<Student> &nie_zd) {
	while (!oczekujacy.empty()) {
		if (oczekujacy.front().pokaz_zestaw() % 2)
			nie_zd.push(oczekujacy.front());
		else
			zdali.push(oczekujacy.front());
		oczekujacy.pop();
	}
}

void f2(std::queue<Student> &zdali) {
	std::cout << "Zdali:\n";
	while (!zdali.empty()){
		std::cout << zdali.front() << " pkt. \n";
		zdali.pop();
	}
}

void f3(std::queue<Student> &nie_zd) {
	std::cout << "Nie zdali:\n";
	while (!nie_zd.empty()) {
		std::cout << nie_zd.front() << " pkt. \n";
		nie_zd.pop();
	}
}

int main()
{
	{//zad 1.
		std::vector<int> obiad;

		obiad.resize(10);
		for (size_t i = 0; i < obiad.size(); ++i) {
			std::cout << "Podaj  ilosc zjedzonych nalesnikow przez " << i + 1 << ". osobe: ";
			std::cin >> obiad[i];
		}
		std::cout << "Najwiecej zjadla " << std::max_element(obiad.begin(), obiad.end()) + 1 - obiad.begin() << ". osoba."; //max_element(obiad) - poczatek obiad
	}
	std::cout << '\n';
	{//zad 2.
		std::stack<std::string> s;
		std::ifstream ifs("zdania.txt");

		if (ifs) {
			std::string tmp;

			while (ifs >> tmp)
				s.push(tmp);
			ifs.close();

			std::ofstream ofs("wynik.txt");
			if (ofs) {
				while (!s.empty()) {
					std::cout << s.top() << ' ';
					ofs << s.top() << ' ';
					s.pop();
				}
				ofs.close();
			}
		}
	}
	std::cout << '\n';
	{//zad 3.
		std::queue<Student> oczekujacy_na_egzamin, zdali, nie_zdali;
		oczekujacy_na_egzamin.push(Student{ "Krzysztof", "D." });
		oczekujacy_na_egzamin.push(Student{ "Jan", "Pawel" });
		oczekujacy_na_egzamin.push(Student{ "Jan", "K." });
		oczekujacy_na_egzamin.push(Student{ "Ktos", "" });

		f(oczekujacy_na_egzamin, zdali, nie_zdali);
		f2(zdali);
		f3(nie_zdali);
	}

	return 0;
}
