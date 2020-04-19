#ifndef PARTY_HPP
#define PARTY_HPP
#include <string>
#include "tinyxml2/tinyxml2.h"

class Party
{
	std::string name, NIP, address, postcode, town;
public:
	Party() = default;

	//set all members from user
	void createParty();

	//reading, showing, writing
	std::istream &read(std::istream & is);
	std::ostream &write(std::ostream & os)const;
	friend std::ostream & operator<<(std::ostream & os, const Party & p);
	void writeXML(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement * pPrevElement)const;
};

#endif // !PARTY_HPP
