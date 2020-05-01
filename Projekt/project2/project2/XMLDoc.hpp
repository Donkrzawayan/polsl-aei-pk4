#ifndef XMLDOC_HPP
#define XMLDOC_HPP
#include "tinyxml2/tinyxml2.h"
#include <stack>
#include <string>

class XMLDoc
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode *pRoot;
	std::stack<tinyxml2::XMLElement *> pElements;
public:
	XMLDoc() : doc(), pRoot(nullptr), pElements() {}


	void newDoc(const char *name);

	template <typename T>
	void addElement(const char *TextNewElement, const T SetText);
	void addElement(const char *TextNewElement) { pElements.push(doc.NewElement(TextNewElement)); } //need insertChild after
	void insertChild();

	bool saveXML(const char * docName);


	bool loadFile(const char *docName);

	bool childElement(const char * text);
	bool nextElement(const char * text);
	std::string getText(const char * text);
	int getInt(const char * text);
	float getFloat(const char * text);
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
