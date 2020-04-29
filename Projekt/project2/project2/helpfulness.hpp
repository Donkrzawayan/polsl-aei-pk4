#ifndef HELPFULNESS_HPP
#define HELPFULNESS_HPP
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "tinyxml2/tinyxml2.h"

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

	template <typename T, typename XMLType>
	void addEndElement(tinyxml2::XMLDocument &doc, const char *TextNewElement, const T SetText, XMLType * pPrevElement) {
		using namespace tinyxml2;

		XMLElement * pElement = doc.NewElement(TextNewElement);
		pElement->SetText(SetText);
		pPrevElement->InsertEndChild(pElement);
	}
}

#endif // !HELPFULNESS_HPP
