#ifndef DAILYRAPORT_HPP
#define DAILYRAPORT_HPP
#include "XMLDoc.hpp"

/// Class to generate daily raport
class DailyRaport
{
	long double totalPayment; ///< Total daily payment
	long double totalPTUAmount; ///< Total daily PTU payment
public:
	DailyRaport() : totalPayment(0.0L), totalPTUAmount(0.0L) {}
	void addSum(double sum, double PTUSum) { totalPayment += sum, totalPTUAmount += PTUSum; }
	bool generate();
private:
	inline void writeDocumentInfoXML(XMLDoc &doc)const;
	inline void writePaymentXML(XMLDoc &doc)const;
};

#endif // !DAILYRAPORT_HPP
