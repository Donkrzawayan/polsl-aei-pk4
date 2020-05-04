#include "helpfulness.hpp"
#include <ctime>
#include <utility> //move

std::string helpfulness::date(const char delim)
{
	std::string date;
	const time_t t = time(NULL);

	date += std::to_string(localtime(&t)->tm_year + 1900);
	if (delim != '\0') date += delim;
	date += std::to_string(localtime(&t)->tm_mon + 1);
	if (delim != '\0') date += delim;
	date += std::to_string(localtime(&t)->tm_mday);

	return std::move(date);
}

std::string helpfulness::hour(const char delim)
{
	std::string hour;
	const time_t t = time(NULL);

	hour += std::to_string(localtime(&t)->tm_hour);
	if (delim != '\0') hour += delim;
	hour += std::to_string(localtime(&t)->tm_min);

	return std::move(hour);
}

std::istream & helpfulness::readStringBin(std::istream & is, std::string & str) {
	size_t size;

	is.read(reinterpret_cast<char *>(&size), sizeof(size));
	str.resize(size);
	is.read(reinterpret_cast<char *>(&str[0]), size * sizeof(str[0]));

	return is;
}

std::ostream & helpfulness::writeStringBin(std::ostream & os, const std::string & str)
{
	const size_t size = str.size() + 1U;

	os.write(reinterpret_cast<const char *>(&size), sizeof(size));
	return os.write(str.c_str(), size * sizeof(str[0]));
}
