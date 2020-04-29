#ifndef HELPFULNESS_HPP
#define HELPFULNESS_HPP
#include <iomanip> // setprecision
#include <sstream> // stringstream

namespace helpfulness
{
	template<typename T>
	std::string toStringPrecision2(T input) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << input;
		return ss.str();
	}

	//return date and time as a string; delim deliminate numbers
	std::string date(const char delim = '\0');
	std::string hour(const char delim = '\0');
}

#endif // !HELPFULNESS_HPP
