#include "dataManager.hpp"
#include <fstream>
#include <iomanip> //std::setw
#include <ctime>
#include "helpfulness.hpp"

void DataManager::readBase(std::ifstream &ifs)
{
	owner.read(ifs);
	readMonthAndInvoiceNo(ifs);
	readStock(ifs);
}

inline void DataManager::readMonthAndInvoiceNo(std::ifstream & ifs)
{
	int month;
	ifs.read(reinterpret_cast<char *>(&month), sizeof(month));
	ifs.read(reinterpret_cast<char *>(&invoiceNo), sizeof(invoiceNo));

	//if month has changed reset invoice invoiceNo
	const time_t t = time(NULL);
	if (month != localtime(&t)->tm_mon) invoiceNo = 1U;
}

inline void DataManager::readStock(std::ifstream & ifs)
{
	size_t size;
	ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
	stock.resize(size);
	for (auto &item : stock)
		item.read(ifs);
}

DataManager &DataManager::operator+=(Item &&c) {
	auto it = std::find(stock.begin(), stock.end(), c);
	if (it != stock.end()) {
		(*it) += c.getQuantity();
		it->setPurchasePrice(c.getPurchasePrice()); //update purchase price
	}
	else //not found in the base
		stock.push_back(std::move(c));
	return *this;
}

DataManager::DataManager(std::string dbFileName)
{
	std::ifstream ifs(dbFileName.c_str(), std::ios::binary | std::ios::in);
	if (!ifs.is_open()){ //first use
		std::cout << "Witam w programie TinyBusiness!\n"
			<< "Wprowadz dane wlasciciela\n"
			<< "Dane Twojej firmy\n";
		createOwner();
		invoiceNo = 1U;
	}
	else {
		readBase(ifs);
		ifs.close();
	}
}

void DataManager::createOwner()
{
	std::cout << "Enter konczy wczytywanie danej pozycji. \n";
	owner.createParty();
}

void DataManager::sortAndWriteBase(std::string dbFileName)
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

inline void DataManager::writeMonthAndInvoiceNo(std::ofstream & ofs) const
{
	const time_t t = time(NULL);
	int month = localtime(&t)->tm_mon;
	ofs.write(reinterpret_cast<const char *>(&month), sizeof(month));
	ofs.write(reinterpret_cast<const char *>(&invoiceNo), sizeof(invoiceNo));
}

inline void DataManager::writeStock(std::ofstream & ofs) const
{
	const size_t size = stock.size();

	ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
	for (const auto &item : stock)
		item.write(ofs);
}

bool DataManager::loadFromXMLInvoice(const std::string & docName) {
	XMLDoc doc;
	bool result = doc.loadFile(docName.c_str());
	if (!result) return false;

	doc.childElement("Pozycje");
		if (doc.childElement("Pozycja"))
			do {
				Item temp;
				temp.readXML(doc);
				(*this) += std::move(temp);
			} while (doc.nextElement("Pozycja"));

	return true;
}

void DataManager::showStock() const
{
	constexpr std::streamsize NO_WIDTH = 5;
	constexpr std::streamsize DESCRIPTION_WIDTH = 56;
	constexpr std::streamsize QTY_WIDTH = 5;
	constexpr std::streamsize PRICE_WIDTH = 12;
	constexpr std::streamsize VAT_WIDTH = 4;

	auto width = std::cout.width(); //remember old value

	showStockHeader(NO_WIDTH, DESCRIPTION_WIDTH, QTY_WIDTH, PRICE_WIDTH, VAT_WIDTH);

	showStockContent(NO_WIDTH, DESCRIPTION_WIDTH, QTY_WIDTH, PRICE_WIDTH, VAT_WIDTH);

	std::cout.unsetf(std::ios_base::adjustfield); //set default
	std::cout.width(width);
}

void DataManager::showStockHeader(const std::streamsize noWidth, const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth) const
{
	std::cout << std::right << std::setw(noWidth) << "lp. ";
	std::cout << std::left << std::setw(descriptionWidth) << "Nazwa";
	std::cout << std::right << std::setw(quantityWidth) << "Ilosc";
	std::cout << std::right << std::setw(spriceWidth) << "Cena sprz.";
	std::cout << std::right << std::setw(vatWidth) << "VAT";
	std::cout << "\n";
}

void DataManager::showStockContent(const std::streamsize noWidth, const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth) const
{
	for (size_t i = 0; i < stock.size(); ++i) {
		std::cout << std::right << std::setw(noWidth - 2) << (i + 1) << ". "; //index for user
		stock[i].show(descriptionWidth, quantityWidth, spriceWidth, vatWidth);
		std::cout << "\n";
	}
}
