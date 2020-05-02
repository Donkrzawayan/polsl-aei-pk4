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

void DataBase::sortAndWriteBase(std::string dbFileName)
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

bool DataBase::loadFromXMLInvoice(const std::string & docName) {
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

bool DataBase::dailyRaport() {
	XMLDoc doc;
	doc.newDoc("Raport_dobowy");

	writeDocumentInfoXML(doc);
	writePaymentXML(doc);

	bool result = doc.saveXML(("Raport" + std::move(helpfulness::date()) + ".xml").c_str());
	if (result) {
		totalPayment = totalPTUAmount = 0.0L; //reset values
		return true;
	}
	else
		return false;
}

inline void DataBase::writeDocumentInfoXML(XMLDoc & doc) const
{
	doc.addElement("Informacje_o_dokumencie");
	doc.addElement("Data_wykonania_raportu_dobowego", helpfulness::date('.').c_str());
	doc.addElement("Godzina_wykonania_wydruku", helpfulness::hour(':').c_str());
	doc.insertChild();
}

inline void DataBase::writePaymentXML(XMLDoc & doc) const
{
	doc.addElement("Naleznosci");
	doc.addElement("Laczna_kwota_sprzedazy_brutto", helpfulness::toStringPrecision2(totalPayment).c_str());
	doc.addElement("Laczna_kwota_PTU", helpfulness::toStringPrecision2(totalPTUAmount).c_str());
	doc.addElement("Waluta", "PLN");
	doc.insertChild();
}

void DataBase::ShowStock() const
{
	constexpr std::streamsize NO_WIDTH = 5;
	constexpr std::streamsize DESCRIPTION_WIDTH = 56;
	constexpr std::streamsize QTY_WIDTH = 5;
	constexpr std::streamsize PRICE_WIDTH = 12;
	constexpr std::streamsize VAT_WIDTH = 4;

	auto width = std::cout.width(); //remember old value

	ShowStockHeader(NO_WIDTH, DESCRIPTION_WIDTH, QTY_WIDTH, PRICE_WIDTH, VAT_WIDTH);

	ShowStockContent(NO_WIDTH, DESCRIPTION_WIDTH, QTY_WIDTH, PRICE_WIDTH, VAT_WIDTH);

	std::cout.unsetf(std::ios_base::adjustfield); //set default
	std::cout.width(width);
}

void DataBase::ShowStockHeader(const std::streamsize noWidth, const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth) const
{
	std::cout << std::right << std::setw(noWidth) << "lp. ";
	std::cout << std::left << std::setw(descriptionWidth) << "Nazwa";
	std::cout << std::right << std::setw(quantityWidth) << "Ilosc";
	std::cout << std::right << std::setw(spriceWidth) << "Cena sprz.";
	std::cout << std::right << std::setw(vatWidth) << "VAT";
	std::cout << "\n";
}

void DataBase::ShowStockContent(const std::streamsize noWidth, const std::streamsize descriptionWidth, const std::streamsize quantityWidth, const std::streamsize spriceWidth, const std::streamsize vatWidth) const
{
	for (size_t i = 0; i < stock.size(); ++i) {
		std::cout << std::right << std::setw(noWidth - 2) << (i + 1) << ". "; //index for user
		stock[i].Show(descriptionWidth, quantityWidth, spriceWidth, vatWidth);
		std::cout << "\n";
	}
}
