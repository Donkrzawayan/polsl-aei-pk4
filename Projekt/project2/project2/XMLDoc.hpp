#ifndef XMLDOC_HPP
#define XMLDOC_HPP
#include "tinyxml2/tinyxml2.h"
#include <stack>
#include <string>
#include <exception>

class XMLDoc
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode *pRoot;
	std::stack<tinyxml2::XMLElement *> pElements;
public:
	XMLDoc() : doc(), pRoot(nullptr) {}


	void newDoc(const char *name);

	template <typename T>
	void addElement(const char *TextNewElement, const T SetText);
	void addElement(const char *TextNewElement) { if (pRoot) pElements.push(doc.NewElement(TextNewElement)); } //need insertChild after
	void insertChild();

	bool saveXML(const char * docName);


	class XMLException : std::exception
	{
#include <cstring>
		char msg[80];
	public:
		XMLException(const char *message) { strncpy(msg, message, sizeof(msg)); }
		char const* what() const override { return msg; }
	};

	bool loadFile(const char *docName);

	bool childElement(const char * text);
	bool nextElement(const char * text);
	std::string getText(const char * text)const;
	int getInt(const char * text)const;
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
