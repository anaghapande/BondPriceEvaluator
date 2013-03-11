/*
 * Calculator.h
 *
 *  Created on: Oct 15, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#ifndef BONDCALCULATOR_H_
#define BONDCALCULATOR_H_

#define MATURITY 100
#define ONE_BP 0.0001

#include "Bond.h"
#include "BondRatings.h"

class BondCalculator
{
	public:
		BondCalculator();
		virtual ~BondCalculator();

		const double calculateBondPrice(int bp, int bump, long period, double coupon, long frequency, double yieldRate);
		const double calculateDVO1(long period, double coupon, long frequency, double rate, double bondPrice);
		const double calculateRisk(double dvo1, long amount);
		const double calculateAmountAdjustedLGD(string quality, long amount);
		//const double* calculateBumpedMarketValue_Risk_TwoYrHedge(int bump, string bucket);
};

#endif /* CALCULATOR_H_ */
