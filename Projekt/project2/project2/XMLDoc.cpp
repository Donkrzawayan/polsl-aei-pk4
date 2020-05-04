#include "XMLDoc.hpp"
#include <utility> //move

void XMLDoc::newDoc(const char * name)
{
	pRoot = doc.NewElement(name);
	doc.InsertFirstChild(pRoot);
}

bool XMLDoc::loadFile(const char * docName)
{
	using namespace tinyxml2;

	XMLError result = doc.LoadFile(docName);
	if (result != XML_SUCCESS)
		if (result == XML_ERROR_PARSING_TEXT)
			throw XMLException("XML_ERROR_PARSING_TEXT");
		else
			return false;

	pRoot = doc.LastChild(); //first is <?xml version="1.0" encoding="UTF-8"?>
	return pRoot ? true : false;
}

bool XMLDoc::childElement(const char * text)
{
	if (pElements.empty())
		pElements.push(pRoot->FirstChildElement(text));
	else
		pElements.push(pElements.top()->FirstChildElement(text));
	return pElements.top() ? true : false; //XML_ERROR_PARSING_ELEMENT
}

bool XMLDoc::nextElement(const char * text)
{
	pElements.top() = pElements.top()->NextSiblingElement(text);
	return pElements.top() ? true : false;
}

std::string XMLDoc::getText(const char * text) const
{
	tinyxml2::XMLElement *pElement = pElements.top()->FirstChildElement(text);
	if (!pElement)
		throw XMLException("XML_ERROR_PARSING_ELEMENT");
	return std::string(pElement->GetText());
}

int XMLDoc::getInt(const char * text) const
{
	tinyxml2::XMLElement *pElement = pElements.top()->FirstChildElement(text);
	if (!pElement)
		throw XMLException("XML_ERROR_PARSING_ELEMENT");
	int temp;
	pElement->QueryIntText(&temp);
	return temp;
}

float XMLDoc::getFloat(const char * text) const
{
	tinyxml2::XMLElement *pElement = pElements.top()->FirstChildElement("Cena_brutto");
	if (!pElement)
		throw XMLException("XML_ERROR_PARSING_ELEMENT");
	float temp;
	pElement->QueryFloatText(&temp);
	return temp;
}

void XMLDoc::insertChild()
{
	if (pElements.size() == 1U) {
		pRoot->InsertEndChild(pElements.top());
		pElements.pop();
	}
	else {
		tinyxml2::XMLElement *temp = std::move(pElements.top());
		pElements.pop();
		pElements.top()->InsertEndChild(temp);
	}
}

bool XMLDoc::saveXML(const char * docName)
{
	doc.InsertFirstChild(doc.NewDeclaration()); //add <?xml version="1.0" encoding="UTF-8"?>

	tinyxml2::XMLError result = doc.SaveFile(docName);
	return result == tinyxml2::XML_SUCCESS;
}
