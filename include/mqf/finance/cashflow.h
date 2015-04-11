#ifndef INCLUDED_MQF_CASHFLOW
#define INCLUDED_MQF_CASHFLOW
#include "interest.h"

namespace mqf {

	template<typename Calendar = Gregorian>
	struct Cashflow {
		double amount;
		Date<Calendar> date;

		Cashflow() = default;
		Cashflow( double amount, const Date<Calendar>& date ) : amount(amount), date(date) {}
	};

	template<typename Calendar,Compounding C>
	double PresentValue( const Cashflow<Calendar>& cashflow,
	                     const InterestRate<C>& interest,
						 const Date<Calendar>& presentDate ) {
		return cashflow.amount * interest.discountFactor( Duration( presentDate, cashflow.date ) );
	}

	template<typename Container,typename Calendar,Compounding C>
	double NetPresentValue( const Container& cashflows, const InterestRate<C>& interest, const Date<Calendar>& presentDate ) {
		double sum = 0.0;
		for( auto&& cf : cashflows ) {
			sum += PresentValue( cf, interest, presentDate );
		}
		return sum;
	}




}

#endif
