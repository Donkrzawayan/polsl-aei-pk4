#ifndef HELPFULNESS_HPP
#define HELPFULNESS_HPP
#include <iomanip> // setprecision
#include <sstream> // stringstream

/**
 @namespace helpfulness
 @brief Useful functions
 */
namespace helpfulness
{
	/// Convert numerical value to string with setprecision(2)
	template<typename T>
	std::string toStringPrecision2(T input) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << input;
		return ss.str();
	}

	/**
	 @return date as a string
	 @param delim deliminate numbers
	 */
	std::string date(const char delim = '\0');
	/**
	 @return hour as a string
	 @param delim deliminate numbers
	 */
	std::string hour(const char delim = '\0');

	std::istream &readStringBin(std::istream & is, std::string &str);
	std::ostream &writeStringBin(std::ostream & os, const std::string &str);
}

#endif // !HELPFULNESS_HPP
