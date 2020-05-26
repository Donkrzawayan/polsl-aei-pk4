#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>
#include <vector>
#include <mutex>
#include <random> //std::mt19937
#include <chrono> //seed
using namespace std;


class Punkt
{
	int x, y;
public:
	Punkt(int x, int y) : x(x), y(y) {}
	//konstruktor
	void operator() (int mnoznik)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		x *= mnoznik;
		y *= mnoznik;
	}
	void wyswietl_polozenie()const { cout << "x: " << x << "\ty: " << y << "\n"; }
};


void srednia(double n)
{
	double a = n - 1;
	double b = n;
	double c = n + 1;
	thread t([](double d, double e, double f) {std::cout << (d*d + e*e + f*f) / 3 << "\n"; }, a, b, c);
	t.join();
}

class Skarbonka
{
	int suma = 0;
	mutable mutex mut;
public:
	void operator() (int ilosc, bool dodaj)
	{
		for (int i = 0; i < ilosc; i++)
		{
			mut.lock();
			if (dodaj)
			{
				std::this_thread::sleep_for(chrono::microseconds(100));
				suma += 1;
				//...
			}
			else
			{
				std::this_thread::sleep_for(chrono::microseconds(100));
				suma -= 1;
				//...
			}
			mut.unlock();
		}
	}
	void Wyswietl() { cout << "Suma: " << suma << "\n";	}
	//...
};

void losowanie(vector<int> &v) {
	for (int i = 0; i < 100; ++i);
		v.push_back(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())() % 100 + 1);
}

void parzyste(vector<int> v) {
	int count = 0;
	for (const auto &i : v)
		if (!(i % 2))
			++count;
	std::cout << "Liczba parzystych: " << count << "\n";
}

void nieparzyste(vector<int> v) {
	int count = 0;
	for (const auto &i : v)
		if (i % 2)
			++count;
	std::cout << "Liczba nieparzystych: " << count << "\n";
}

int main()
{
	//Zad 1
	cout << "Zadanie 1 " << endl;
	vector<int> losowe;
	//wątek 1
	thread thr1(losowanie, ref(losowe));
	thr1.join();
	//wątek 2
	thread thr2(parzyste, ref(losowe));
	//wątek 3
	thread thr3(nieparzyste, ref(losowe));

	thr2.join();
	thr3.join();

	//Zad 2
	cout << "Zadanie 2 " << endl;
	Punkt NowyPunkt(1, 1);

	thread thrPkt(ref(NowyPunkt), 2);
	thrPkt.join();
	NowyPunkt.wyswietl_polozenie();

	thread thrPkt2(ref(NowyPunkt), 2);
	thrPkt2.detach();
	NowyPunkt.wyswietl_polozenie();
	//nie odczekano 2 sekund - wynik sie nie zmienil
	// nowy wątek

	//Zad 3
	cout << "Zadanie 3 " << endl;
	thread t(srednia, 2);
	t.join();
	//nowy watek

	//Zad 4
	cout << "Zadanie 4 " << endl;
	Skarbonka NowaSkarbonka;
	std::thread t1(std::ref(NowaSkarbonka), 500, true);
	std::thread t2(std::ref(NowaSkarbonka), 500, true);
	std::thread t3(std::ref(NowaSkarbonka), 500, true);
	std::thread t4(std::ref(NowaSkarbonka), 500, true);
	// Nowy watek dodający 500
	std::thread t5(std::ref(NowaSkarbonka), 500, false);
	std::thread t6(std::ref(NowaSkarbonka), 500, false);
	std::thread t7(std::ref(NowaSkarbonka), 500, false);
	std::thread t8(std::ref(NowaSkarbonka), 500, false);
	// Nowy wątek odejmujący 500
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	//...
	NowaSkarbonka.Wyswietl();

	// nowywątek.join();
	//...
}


