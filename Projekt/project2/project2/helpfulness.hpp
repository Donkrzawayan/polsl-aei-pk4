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
}

#endif // !HELPFULNESS_HPP
