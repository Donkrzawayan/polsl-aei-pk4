#include <iostream>
#include <memory>
#include <vector>
#include <deque>

using namespace std;

/*********************** Zadanie 1 **************************/

weak_ptr<int> zad1()
{
	auto ptr1 = make_shared<int>(10);
	cout << "ptr1.use_count(): " << ptr1.use_count() << endl; //1bo jeden wskaznik
	cout << "ptr1.unique(): " << ptr1.unique() << endl << endl; //true

	auto ptr2 = ptr1, ptr3 = ptr1;

	cout << "ptr1.use_count(): " << ptr1.use_count() << endl; //3 bo ptr2 i ptr3 się pojawily
	cout << "ptr1.unique(): " << ptr1.unique() << endl << endl; //false, wiecej niz jeden ptr istnieje

	auto weak = weak_ptr<int>(ptr1);

	cout << "weak.use_count(): " << weak.use_count() << endl; //3 weak nic nie zmienia
	cout << "weak.expired(): " << weak.expired() << endl; //false, ptry istnieja
	cout << "weak.lock(): " << weak.lock() << endl << endl;

	return weak;
}

/*********************** Zadanie 2 **************************/

struct Element
{
	friend class Stack;

private:
	int value;
	shared_ptr<Element> next;

public:
	// odpowiedni konstruktor
	Element(int val, shared_ptr<Element> next) : value(val), next(move(next)) {}

	// odpowiedni destruktor
	~Element()
	{
		cout << "destruktor";
	}
};

class Stack
{
private:
	shared_ptr<Element> head;
public:
	void push(int val)
	{
		head = make_shared<Element>(val, head);
	}
	int pop()
	{
		auto temp = head->value;
		head = move(head->next);
		return temp;
	}
	void print() const
	{
		weak_ptr<Element> temp = head;
		while (temp.lock()) {
			std::cout << temp.lock()->value << std::endl;
			temp = temp.lock()->next;
		}
	}
};

/*********************** Zadanie 3 **************************/

class Mieszkanie;
class Lokator;

class Mieszkanie
{
	vector<shared_ptr<Lokator>> lokatorzy;
public:
	Mieszkanie() { cout << "stworzenie mieszkania"; }
	~Mieszkanie() { cout << "zniszczenie mieszkania"; }
	void dodaj_lokatora(const shared_ptr<Lokator>&l) {
		lokatorzy.push_back(l);
	}
};

class Lokator
{
	weak_ptr<Mieszkanie> dom;
public:
	Lokator() { cout << "stworzenie lokatora"; }
	~Lokator() { cout << "zniszczenie lokatora"; }
	void ustaw_dom(const weak_ptr<Mieszkanie> &spm) {
		dom = spm;
	}
};

void zad3()
{
	auto mieszkanie = make_shared<Mieszkanie>();

	for (int i = 0; i < 10; ++i)
	{
		auto lokator = make_shared<Lokator>();
		lokator->ustaw_dom(mieszkanie);
		mieszkanie->dodaj_lokatora(lokator);
	}
}

/************************************************************/

int main()
{
	cout << boolalpha << "Zadanie 1" << endl << endl;

	auto weak = zad1();

	cout << "weak.use_count(): " << weak.use_count() << endl; //0 bo nic nie wskazuje
	cout << "weak.expired(): " << weak.expired() << endl; //true bo nic nie wskazuje
	cout << "weak.lock(): " << weak.lock() << endl << endl; //nullptr bo nic nie wskazuje

	cout << "Zadanie 2" << endl << endl;

	Stack test;
	test.push(2137);
	test.push(1);
	test.push(2);
	test.push(69);

	cout << test.pop() << " " << test.pop() << "\n";

	cout << "Zadanie 3" << endl << endl;

	zad3();

	return 0;
}