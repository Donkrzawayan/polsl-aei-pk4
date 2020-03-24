#ifndef PARTY_HPP
#define PARTY_HPP
#include <string>

class Party
{
	std::string name, NIP, address, postcode, town;
public:
	Party();
	~Party() = default;

	std::istream &read(std::istream & ifs);
};

#endif // !PARTY_HPP
