#ifndef INCLUDED_MQF_CASHFLOW
#define INCLUDED_MQF_CASHFLOW
#include "interest.h"
#include "../gregorian.h"

namespace mqf {

	template<typename Date = Gregorian::Date>
	struct Cashflow {
		double amount;
		Date date;

		Cashflow() = default;
		Cashflow( double amount, const Date& date ) : amount(amount), date(date) {}
	};

	template<typename Date,typename Interest>
	double PresentValue( const Cashflow<Date>& cashflow,
	                     const Interest& interest,
						 const Date& presentDate ) {
		return cashflow.amount * interest.discountFactor( Duration( presentDate, cashflow.date ) );
	}

	template<typename Container,typename Date,typename Interest>
	double NetPresentValue( const Container& cashflows, const Interest& interest, const Date& presentDate ) {
		double sum = 0.0;
		for( auto&& cf : cashflows ) {
			sum += PresentValue( cf, interest, presentDate );
		}
		return sum;
	}




}

#endif
