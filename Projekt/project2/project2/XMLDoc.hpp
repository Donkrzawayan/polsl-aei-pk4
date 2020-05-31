#ifndef XMLDOC_HPP
#define XMLDOC_HPP
#include "tinyxml2/tinyxml2.h"
#include <stack>
#include <string>
#include <exception>

/// Tinyxml library managment
class XMLDoc
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode *pRoot;
	std::stack<tinyxml2::XMLElement *> pElements; ///< Stack of pointers to element
public:
	XMLDoc() : doc(), pRoot(nullptr) {}


	void newDoc(const char *name);

	template <typename T>
	void addElement(const char *TextNewElement, const T SetText);
	/** @attention need insertChild after and void addElement(const char *TextNewElement, const T SetText) between */
	void addElement(const char *TextNewElement) { if (pRoot) pElements.push(doc.NewElement(TextNewElement)); }
	void insertChild();

	bool saveXML(const char * docName);


	/// Exceptions thrown from read documents.
	class XMLException : std::exception
	{
#include <cstring>
		char msg[80]; ///< Buffer for warning text
	public:
		XMLException(const char *message) { strncpy(msg, message, sizeof(msg)); }
		char const* what() const override { return msg; }
	};

	/**
	 @exception XMLException("XML_ERROR_PARSING_TEXT") invalid file content
	 @retval false file doesnt exist or is empty
	*/
	bool loadFile(const char *docName);

	bool childElement(const char * text);
	bool nextElement(const char * text);
	/** @exception XMLException("XML_ERROR_PARSING_ELEMENT") invalid file content */
	std::string getText(const char * text)const;
	/** @exception XMLException("XML_ERROR_PARSING_ELEMENT") invalid file content */
	int getInt(const char * text)const;
	/** @exception XMLException("XML_ERROR_PARSING_ELEMENT") invalid file content */
	float getFloat(const char * text)const;
};

template<typename T>
inline void XMLDoc::addElement(const char * TextNewElement, const T SetText)
{
	tinyxml2::XMLElement * pNewElement = doc.NewElement(TextNewElement);
	pNewElement->SetText(SetText);

	if (pElements.empty())
		pRoot->InsertEndChild(pNewElement);
	else
		pElements.top()->InsertEndChild(pNewElement);
}

#endif // !XMLDOC_HPP
