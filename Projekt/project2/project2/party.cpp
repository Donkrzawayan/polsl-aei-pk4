#include "party.hpp"
#include <iostream>
#include "helpfulness.hpp"

std::istream & Party::read(std::istream & is)
{
	helpfulness::readStringBin(is, name);
	helpfulness::readStringBin(is, NIP);
	helpfulness::readStringBin(is, address);
	helpfulness::readStringBin(is, postcode);
	helpfulness::readStringBin(is, city);

	return is;
}

std::ostream & Party::write(std::ostream & os) const
{
	helpfulness::writeStringBin(os, name);
	helpfulness::writeStringBin(os, NIP);
	helpfulness::writeStringBin(os, address);
	helpfulness::writeStringBin(os, postcode);
	helpfulness::writeStringBin(os, city);

	return os;
}

void Party::writeXML(XMLDoc & doc) const
{
	doc.addElement("Nazwa", name.c_str());
	doc.addElement("NIP", NIP.c_str());
	doc.addElement("Adres", address.c_str());
	doc.addElement("Kod_pocz.", postcode.c_str());
	doc.addElement("Miasto", city.c_str());
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
