//

#include <iostream>

template <typename T>
class Wektor2D {
	T _x, _y;
public:
	Wektor2D(T x, T y) :_x(x), _y(y) {}
	Wektor2D(const Wektor2D& wektor) = default;

	void normalise() {
		T dlugosc = std::sqrt(_x*_x + _y * _y);
		_x = _x / dlugosc;
		_y = _y / dlugosc;
	}

	Wektor2D<T> &operator+=(const Wektor2D<T> &w) { _x += w._x; _y += w._y; return *this; }

	Wektor2D operator*(T s) { return Wektor2D(_x*s, _y*s); }
	void wypisz() { std::cout << "x: " << _x << ", y: " << _y << ".\n"; }
};

template <typename T>
class Object {
	Wektor2D<T> w1, w2;
	T predkosc;
public:
	Object(T a, T b, T c, T d, T e) : w1(a, b), w2(c, d), predkosc(e) { w2.normalise(); }
	~Object() = default;
	void move(){ w1 += w2 * predkosc; }

	void wypisz() {
		std::cout << "Polozenie: ";
		w1.wypisz();
		std::cout << "Kierunek: ";
		w2.wypisz();
		std::cout << "Predkosc: " << predkosc << ".\n";
	}
};

int main()
{
	{
		Wektor2D<double> W1(2.1, 3.7);
		W1.normalise();
		W1.wypisz();

		auto W2 = W1;
		W2.wypisz();

		W1.operator+=(W2);
		W1.wypisz();

		auto W3{ W1 * 88 };
		W3.wypisz();
	}

	{
		auto O1 = new Object<double>(3.0, 3.0, 5.0, 5.0, 4.0);

		O1->move();
		O1->wypisz();

		delete O1;
	}
}
