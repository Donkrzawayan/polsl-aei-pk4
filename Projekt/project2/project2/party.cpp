#include "party.hpp"
#include <iostream>
#include "helpfulness.hpp"

std::istream & Party::read(std::istream & is)
{
	size_t size;

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	name.resize(size);
	is.read(reinterpret_cast<char *>(&name[0]), size * sizeof(name[0]));

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	NIP.resize(size);
	is.read(reinterpret_cast<char *>(&NIP[0]), size * sizeof(NIP[0]));

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	address.resize(size);
	is.read(reinterpret_cast<char *>(&address[0]), size * sizeof(address[0]));

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	postcode.resize(size);
	is.read(reinterpret_cast<char *>(&postcode[0]), size * sizeof(postcode[0]));

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	city.resize(size);
	is.read(reinterpret_cast<char *>(&city[0]), size * sizeof(city[0]));

	return is;
}

std::ostream & Party::write(std::ostream & os) const
{
	size_t size = name.length() + 1U;
	os.write(reinterpret_cast<const char *>(&size), sizeof(size))
		.write(name.c_str(), size * sizeof(name[0]));
	size = NIP.length() + 1U;
	os.write(reinterpret_cast<const char *>(&size), sizeof(size))
		.write(NIP.c_str(), size * sizeof(NIP[0]));
	size = address.length() + 1U;
	os.write(reinterpret_cast<const char *>(&size), sizeof(size))
		.write(address.c_str(), size * sizeof(address[0]));
	size = postcode.length() + 1U;
	os.write(reinterpret_cast<const char *>(&size), sizeof(size))
		.write(postcode.c_str(), size * sizeof(postcode[0]));
	size = city.length() + 1U;
	os.write(reinterpret_cast<const char *>(&size), sizeof(size))
		.write(city.c_str(), size * sizeof(city[0]));

	return os;
}

void Party::writeXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement * pPrevElement) const
{
	helpfulness::addEndElement(doc, "Nazwa", name.c_str(), pPrevElement);
	helpfulness::addEndElement(doc, "NIP", NIP.c_str(), pPrevElement);
	helpfulness::addEndElement(doc, "Adres", address.c_str(), pPrevElement);
	helpfulness::addEndElement(doc, "Kod_pocz.", postcode.c_str(), pPrevElement);
	helpfulness::addEndElement(doc, "Miasto", city.c_str(), pPrevElement);
}

void Party::createParty()
{
	std::cout << "Nazwa: ";
	std::getline(std::cin, name, '\n');
	std::cout << "NIP: ";
	std::getline(std::cin, NIP, '\n');
	std::cout << "Adres: ";
	std::getline(std::cin, address, '\n');
	std::cout << "Kod pocz.: ";
	std::getline(std::cin, postcode, '\n');
	std::cout << "Miasto: ";
	std::getline(std::cin, city, '\n');
}

std::ostream & operator<<(std::ostream & os, const Party & p)
{
	return os << "Nazwa: " << p.name << "\n"
		<< "NIP: " << p.NIP << "\n"
		<< "Adres: " << p.address << "\n"
		<< "Kod pocz.: " << p.postcode << "\n"
		<< "Miasto: " << p.city << "\n";
}
