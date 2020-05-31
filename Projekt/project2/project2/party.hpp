#ifndef PARTY_HPP
#define PARTY_HPP
#include <string>
#include "XMLDoc.hpp"

/// Class with party data
class Party
{
	std::string name, NIP, address, postcode, city;
public:
	Party() = default;

	/// Set all members from user
	void createParty();

	//reading, showing, writing
	std::istream &read(std::istream & is);
	std::ostream &write(std::ostream & os)const;
	friend std::ostream & operator<<(std::ostream & os, const Party & p);
	void writeXML(XMLDoc &doc)const;
};

#endif // !PARTY_HPP
