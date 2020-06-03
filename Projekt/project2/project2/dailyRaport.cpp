#include "dailyRaport.hpp"
#include "helpfulness.hpp"

bool DailyRaport::generate() {
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

inline void DailyRaport::writeDocumentInfoXML(XMLDoc &doc) const
{
	doc.addElement("Informacje_o_dokumencie");
	doc.addElement("Data_wykonania_raportu_dobowego", helpfulness::date('.').c_str());
	doc.addElement("Godzina_wykonania_wydruku", helpfulness::hour(':').c_str());
	doc.insertChild();
}

inline void DailyRaport::writePaymentXML(XMLDoc &doc) const
{
	doc.addElement("Naleznosci");
	doc.addElement("Laczna_kwota_sprzedazy_brutto", helpfulness::toStringPrecision2(totalPayment).c_str());
	doc.addElement("Laczna_kwota_PTU", helpfulness::toStringPrecision2(totalPTUAmount).c_str());
	doc.addElement("Waluta", "PLN");
	doc.insertChild();
}
