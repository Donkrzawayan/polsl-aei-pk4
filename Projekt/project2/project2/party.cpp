
#include "party.hpp"


Party::Party()
{
}

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
	town.resize(size);
	is.read(reinterpret_cast<char *>(&town[0]), size * sizeof(town[0]));

	return is;
}
