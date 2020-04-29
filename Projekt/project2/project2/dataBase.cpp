#include "dataBase.hpp"
#include <fstream>
#include <iomanip> //std::setw
#include <ctime>
#include "helpfulness.hpp"

void DataBase::readBase(std::ifstream &ifs)
{
	owner.read(ifs);
	readMonthAndInvoiceNo(ifs);
	readStock(ifs);
}

inline void DataBase::readMonthAndInvoiceNo(std::ifstream & ifs)
{
	int month;
	ifs.read(reinterpret_cast<char *>(&month), sizeof(month));
	ifs.read(reinterpret_cast<char *>(&invoiceNo), sizeof(invoiceNo));

	//if month has changed reset invoice invoiceNo
	const time_t t = time(NULL);
	if (month != localtime(&t)->tm_mon) invoiceNo = 1U;
}

inline void DataBase::readStock(std::ifstream & ifs)
{
	size_t size;
	ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
	stock.resize(size);
	for (auto &item : stock)
		item.read(ifs);
}

DataBase &DataBase::operator+=(Item &&c) {
	auto it = std::find(stock.begin(), stock.end(), c);
	if (it != stock.end()) {
		(*it) += c.getQuantity();
		it->setPurchasePrice(c.getPurchasePrice()); //update purchase price
	}
	else //not found in the base
		stock.push_back(std::move(c));
	return *this;
}

DataBase::DataBase(std::string dbFileName): totalPayment(0.0L), totalPTUAmount(0.0L)
{
	std::ifstream ifs(dbFileName.c_str(), std::ios::binary | std::ios::in);
	if (!ifs.is_open()){ //first use
		std::cout << "Witam w programie TinyBusiness!\n"
			<< "Wprowadz dane wlasciciela\n"
			<< "Dane Twojej firmy\n";
		createOwner();
	}
	else {
		readBase(ifs);
		ifs.close();
	}
}

void DataBase::createOwner()
{
	std::cout << "Enter konczy wczytywanie danej pozycji. \n";
	owner.createParty();
}

void DataBase::writeBase(std::string dbFileName)
{
	std::sort(stock.begin(), stock.end()); //sort items in stock

	std::ofstream ofs(dbFileName.c_str(), std::ios::binary | std::ios::out);
	if (!ofs.is_open())
		std::cout << "Nie mozna otworzyc pliku.";
	else {
		owner.write(ofs);
		writeMonthAndInvoiceNo(ofs);
		writeStock(ofs);
		
		ofs.close();
	}
}

inline void DataBase::writeMonthAndInvoiceNo(std::ofstream & ofs) const
{
	const time_t t = time(NULL);
	int month = localtime(&t)->tm_mon;
	ofs.write(reinterpret_cast<const char *>(&month), sizeof(month));
	ofs.write(reinterpret_cast<const char *>(&invoiceNo), sizeof(invoiceNo));
}

inline void DataBase::writeStock(std::ofstream & ofs) const
{
	const size_t size = stock.size();

	ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
	for (const auto &item : stock)
		item.write(ofs);
}

bool DataBase::loadFromXMLInvoice(const std::string & docName)
{
	using namespace tinyxml2;

	XMLDocument doc;

	XMLError result = doc.LoadFile(docName.c_str());
	if (result != XML_SUCCESS) return false;

	XMLNode * pRoot = doc.LastChild(); //first is <?xml version="1.0" encoding="UTF-8"?>
	if (!pRoot) return false;

	XMLElement * pElement = pRoot->FirstChildElement("Pozycje_na_paragonie");
	if (!pElement) return false; //XML_ERROR_PARSING_ELEMENT

		XMLElement * pItemsElement = pElement->FirstChildElement("Pozycja");
		while (pItemsElement) {
			Item temp;
			bool result = temp.readXML(doc, pItemsElement);
			if (!result) return false;
			(*this) += std::move(temp);
			pItemsElement = pItemsElement->NextSiblingElement("Pozycja");
		}

	return true;
}

bool DataBase::dailyRaport()
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLNode *pRoot = doc.NewElement("Raport_dobowy");

	doc.InsertFirstChild(pRoot);
		writeDocumentInfoXML(doc, pRoot);
		writePaymentXML(doc, pRoot);

	return saveXML(doc);
}

inline void DataBase::writeDocumentInfoXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLNode * pRoot) const
{
	tinyxml2::XMLElement * pElement = doc.NewElement("Informacje_o_dokumencie");
	helpfulness::addEndElement(doc, "Data_wykonania_raportu_dobowego", helpfulness::date('.').c_str(), pElement);
	helpfulness::addEndElement(doc, "Godzina_wykonania_wydruku", helpfulness::hour(':').c_str(), pElement);
	pRoot->InsertEndChild(pElement);
}

inline void DataBase::writePaymentXML(tinyxml2::XMLDocument & doc, tinyxml2::XMLNode * pRoot) const
{
	tinyxml2::XMLElement *pElement = doc.NewElement("Naleznosci");
	helpfulness::addEndElement(doc, "Laczna_kwota_sprzedazy_brutto", helpfulness::toStringPrecision2(totalPayment).c_str(), pElement);
	helpfulness::addEndElement(doc, "Laczna_kwota_PTU", helpfulness::toStringPrecision2(totalPTUAmount).c_str(), pElement);
	helpfulness::addEndElement(doc, "Waluta", "PLN", pElement);
	pRoot->InsertEndChild(pElement);
}

inline bool DataBase::saveXML(tinyxml2::XMLDocument & doc)
{
	using namespace tinyxml2;

	doc.InsertFirstChild(doc.NewDeclaration()); //add <?xml version="1.0" encoding="UTF-8"?>

	XMLError result = doc.SaveFile(("Raport" + std::move(helpfulness::date()) + ".xml").c_str());
	if (result == XML_SUCCESS) {
		totalPayment = totalPTUAmount = 0.0L; //reset values
		return true;
	}
	else
		return false;
}

void DataBase::ShowStock() const
{
	constexpr std::streamsize NO_WIDTH = 5;
	constexpr std::streamsize DESCRIPTION_WIDTH = 56;
	constexpr std::streamsize QTY_WIDTH = 5;
	constexpr std::streamsize PRICE_WIDTH = 12;
	constexpr std::streamsize VAT_WIDTH = 4;

	auto width = std::cout.width(); //remember old value

	std::cout << std::right << std::setw(NO_WIDTH) << "lp. ";
	std::cout << std::left << std::setw(DESCRIPTION_WIDTH) << "Nazwa";
	std::cout << std::right << std::setw(QTY_WIDTH) << "Ilosc";
	std::cout << std::right << std::setw(PRICE_WIDTH) << "Cena sprz.";
	std::cout << std::right << std::setw(VAT_WIDTH) << "VAT";
	std::cout << "\n";

	for (size_t i = 0; i < stock.size(); ++i) {
		std::cout << std::right << std::setw(NO_WIDTH - 2) << (i + 1) << ". "; //index for user
		stock[i].Show(DESCRIPTION_WIDTH, QTY_WIDTH, PRICE_WIDTH, VAT_WIDTH);
		std::cout << "\n";
	}

	std::cout.unsetf(std::ios_base::adjustfield); //set default
	std::cout.width(width);
}
