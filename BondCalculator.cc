/*
 * Calculator.cpp
 *
 *  Created on: Oct 15, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#include <math.h>
#include "BondCalculator.h"

#include <stdio.h>
#include <iostream>

static BondRatings ratings;

BondCalculator::BondCalculator()
{
	// TODO Auto-generated constructor stub
}

BondCalculator::~BondCalculator()
{
	// TODO Auto-generated destructor stub
}

//static double results[4];

const double BondCalculator::calculateBondPrice(int bp, int bump, long period, double coupon, long frequency, double yieldRate)
{
	double basisPoint = ONE_BP * bump;
	double rate = (bp == 0 ? yieldRate
						   : bp > 0 ? yieldRate + basisPoint
									: yieldRate - basisPoint);
	double pvFactor = 1 / pow((1 + rate/frequency), (period * frequency));
	double bondPrice = MATURITY * pvFactor;

	bondPrice += coupon * (1 - pvFactor) / rate;

	return bondPrice;
}

const double BondCalculator::calculateDVO1(long period, double coupon, long frequency, double rate, double bondPrice)
{
	double priceUp = calculateBondPrice(1, 1, period, coupon, frequency, rate);
	double priceDown = calculateBondPrice(-1, 1, period, coupon, frequency, rate);

	return ((fabs(priceUp - bondPrice) + fabs (priceDown - bondPrice)) / 2);

}

const double BondCalculator::calculateRisk(double dvo1, long amount)
{
	return (dvo1 * amount / 100);
}

const double BondCalculator::calculateAmountAdjustedLGD(string quality, long amount)
{
	BondRatings ratings;
	return (ratings.getLGD(quality) * amount);
}

/*const double* BondCalculator::calculateBumpedMarketValue_Risk_TwoYrHedge(int bump, string bucket)
{
	//find out if the bump should be multiplied by ONE_BP or not

	double totalBumpedMarketValue = 0.0;
	double totalBumpedRisk = 0.0;
	double bumpedBondPrice, dvo1;
	Bond bond; //change this to Bond* bond for speed

	if(bucket == "2_yr")
	{
		for(unsigned i = 0; i < Bond::allBonds_2yr.size(); i++)
		{
			bond = Bond::allBonds_2yr.at(i);
			bumpedBondPrice = calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP));
			dvo1 = calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP), bumpedBondPrice);
			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}
	else if(bucket == "5_yr")
	{
		for(unsigned i = 0; i < Bond::allBonds_5yr.size(); i++)
		{
			bond = Bond::allBonds_5yr.at(i);
			bumpedBondPrice = calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP));
			dvo1 = calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP), bumpedBondPrice);
			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}
	else if(bucket == "10_yr")
	{
		for(unsigned i = 0; i < Bond::allBonds_10yr.size(); i++)
		{
			bond = Bond::allBonds_10yr.at(i);
			bumpedBondPrice = calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP));
			dvo1 = calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP), bumpedBondPrice);
			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}
	else if(bucket == "30_yr")
	{
		for(unsigned i = 0; i < Bond::allBonds_30yr.size(); i++)
		{
			bond = Bond::allBonds_30yr.at(i);
			bumpedBondPrice = calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP));
			dvo1 = calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump * ONE_BP), bumpedBondPrice);
			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}

	results[0] = totalBumpedRisk;
	results[1] = totalBumpedMarketValue;
	results[2] = totalBumpedRisk / Bond::twoYrDVO1;

	return results;
}*/
