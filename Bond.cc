/*
 * Bond.cc
 *
 *  Created on: Oct 15, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#include "Bond.h"

const long maturity = 100;

Bond::Bond()
{

};

Bond::~Bond()
{
	// TODO Auto-generated destructor stub
}

vector<Bond> Bond::allBonds_2yr;
vector<Bond> Bond::allBonds_5yr;
vector<Bond> Bond::allBonds_10yr;
vector<Bond> Bond::allBonds_30yr;

double Bond::originalTotalRisk_2yr = 0;
double Bond::originalTotalRisk_5yr = 0;
double Bond::originalTotalRisk_10yr = 0;
double Bond::originalTotalRisk_30yr = 0;
double Bond::originalTotalMarketValue_2yr = 0;
double Bond::originalTotalMarketValue_5yr = 0;
double Bond::originalTotalMarketValue_10yr = 0;
double Bond::originalTotalMarketValue_30yr = 0;
double Bond::twoYrDVO1 = 0;

long Bond::totalOpeningAmount_GM = 0;
long Bond::totalClosingAmount_GM = 0;
long Bond::totalOpeningAmount_FG = 0;
long Bond::totalClosingAmount_FG = 0;
long Bond::totalOpeningAmount_YU = 0;
long Bond::totalClosingAmount_YU = 0;
long Bond::totalOpeningAmount_XY = 0;
long Bond::totalClosingAmount_XY = 0;
long Bond::totalOpeningAmount_TT = 0;
long Bond::totalClosingAmount_TT = 0;

long Bond::totalOpeningAmount_A = 0;
long Bond::totalClosingAmount_A = 0;
long Bond::totalOpeningAmount_AA = 0;
long Bond::totalClosingAmount_AA = 0;
long Bond::totalOpeningAmount_AAA = 0;
long Bond::totalClosingAmount_AAA = 0;
long Bond::totalOpeningAmount_B = 0;
long Bond::totalClosingAmount_B = 0;
long Bond::totalOpeningAmount_BB = 0;
long Bond::totalClosingAmount_BB = 0;
long Bond::totalOpeningAmount_BBB = 0;
long Bond::totalClosingAmount_BBB = 0;
long Bond::totalOpeningAmount_C = 0;
long Bond::totalClosingAmount_C = 0;
long Bond::totalOpeningAmount_CC = 0;
long Bond::totalClosingAmount_CC = 0;
long Bond::totalOpeningAmount_CCC = 0;
long Bond::totalClosingAmount_CCC = 0;
long Bond::totalOpeningAmount_D = 0;
long Bond::totalClosingAmount_D = 0;

long Bond::totalOpeningAmount_IG = 0;
long Bond::totalClosingAmount_IG = 0;
long Bond::totalOpeningAmount_junk = 0;
long Bond::totalClosingAmount_junk = 0;

/** RISK **/
double Bond::totalOpeningRisk_GM = 0;
double Bond::totalClosingRisk_GM = 0;
double Bond::totalOpeningRisk_FG = 0;
double Bond::totalClosingRisk_FG = 0;
double Bond::totalOpeningRisk_YU = 0;
double Bond::totalClosingRisk_YU = 0;
double Bond::totalOpeningRisk_XY = 0;
double Bond::totalClosingRisk_XY = 0;
double Bond::totalOpeningRisk_TT = 0;
double Bond::totalClosingRisk_TT = 0;

double Bond::totalOpeningRisk_A = 0;
double Bond::totalClosingRisk_A = 0;
double Bond::totalOpeningRisk_AA = 0;
double Bond::totalClosingRisk_AA = 0;
double Bond::totalOpeningRisk_AAA = 0;
double Bond::totalClosingRisk_AAA = 0;
double Bond::totalOpeningRisk_B = 0;
double Bond::totalClosingRisk_B = 0;
double Bond::totalOpeningRisk_BB = 0;
double Bond::totalClosingRisk_BB = 0;
double Bond::totalOpeningRisk_BBB = 0;
double Bond::totalClosingRisk_BBB = 0;
double Bond::totalOpeningRisk_C = 0;
double Bond::totalClosingRisk_C = 0;
double Bond::totalOpeningRisk_CC = 0;
double Bond::totalClosingRisk_CC = 0;
double Bond::totalOpeningRisk_CCC = 0;
double Bond::totalClosingRisk_CCC = 0;
double Bond::totalOpeningRisk_D = 0;
double Bond::totalClosingRisk_D = 0;

double Bond::totalOpeningRisk_IG = 0;
double Bond::totalClosingRisk_IG = 0;
double Bond::totalOpeningRisk_junk = 0;
double Bond::totalClosingRisk_junk = 0;

/** LGD **/
double Bond::totalOpeningLGD_GM = 0;
double Bond::totalClosingLGD_GM = 0;
double Bond::totalOpeningLGD_FG = 0;
double Bond::totalClosingLGD_FG = 0;
double Bond::totalOpeningLGD_YU = 0;
double Bond::totalClosingLGD_YU = 0;
double Bond::totalOpeningLGD_XY = 0;
double Bond::totalClosingLGD_XY = 0;
double Bond::totalOpeningLGD_TT = 0;
double Bond::totalClosingLGD_TT = 0;

double Bond::totalOpeningLGD_A = 0;
double Bond::totalClosingLGD_A = 0;
double Bond::totalOpeningLGD_AA = 0;
double Bond::totalClosingLGD_AA = 0;
double Bond::totalOpeningLGD_AAA = 0;
double Bond::totalClosingLGD_AAA = 0;
double Bond::totalOpeningLGD_B = 0;
double Bond::totalClosingLGD_B = 0;
double Bond::totalOpeningLGD_BB = 0;
double Bond::totalClosingLGD_BB = 0;
double Bond::totalOpeningLGD_BBB = 0;
double Bond::totalClosingLGD_BBB = 0;
double Bond::totalOpeningLGD_C = 0;
double Bond::totalClosingLGD_C = 0;
double Bond::totalOpeningLGD_CC = 0;
double Bond::totalClosingLGD_CC = 0;
double Bond::totalOpeningLGD_CCC = 0;
double Bond::totalClosingLGD_CCC = 0;
double Bond::totalOpeningLGD_D = 0;
double Bond::totalClosingLGD_D = 0;

double Bond::totalOpeningLGD_IG = 0;
double Bond::totalClosingLGD_IG = 0;
double Bond::totalOpeningLGD_junk = 0;
double Bond::totalClosingLGD_junk = 0;


//SETTERS

/*
void Bond::setAmount(long amount)
{
	this->amount = amount;
}
void Bond::setPeriod(long period)
{
	this->period = period;
}
void Bond::setFrequency(long freq)
{
	frequency = freq;
}
void Bond::setCoupon(double coupon)
{
	this->coupon = coupon;
}
void Bond::setYieldRate(double rate)
{
	this->rate = rate;
}
void Bond::setFrequency(double bondPrice)
{
	this->originalBondPrice = bondPrice;
}


void Bond::addBond_2yr(Bond bond)
{
	allBonds_2yr.push_back(bond);
}
void Bond::addBond_5yr(Bond bond)
{
	allBonds_5yr.push_back(bond);
}
void Bond::addBond_10yr(Bond bond)
{
	allBonds_10yr.push_back(bond);
}
void Bond::addBond_30yr(Bond bond)
{
	allBonds_30yr.push_back(bond);
}


void Bond::setTotalOpeningAmount_GM(long amount)
{
	totalOpeningAmount_GM += amount;
}
void Bond::setTotalClosingAmount_GM(long amount)
{
	totalClosingAmount_GM += amount;
}
void Bond::setTotalOpeningAmount_FG(long amount)
{
	totalOpeningAmount_FG += amount;
}
void Bond::setTotalClosingAmount_FG(long amount)
{
	totalClosingAmount_FG += amount;
}
void Bond::setTotalOpeningAmount_YU(long amount)
{
	totalOpeningAmount_YU += amount;
}
void Bond::setTotalClosingAmount_YU(long amount)
{
	totalClosingAmount_YU += amount;
}
void Bond::setTotalOpeningAmount_XY(long amount)
{
	totalOpeningAmount_XY += amount;
}
void Bond::setTotalClosingAmount_XY(long amount)
{
	totalClosingAmount_XY += amount;
}
void Bond::setTotalOpeningAmount_TT(long amount)
{
	totalOpeningAmount_TT += amount;
}
void Bond::setTotalClosingAmount_TT(long amount)
{
	totalClosingAmount_TT += amount;
}
void Bond::setTotalOpeningAmount_A(long amount)
{
	totalOpeningAmount_A += amount;
}
void Bond::setTotalClosingAmount_A(long amount)
{
	totalClosingAmount_A += amount;
}
void Bond::setTotalOpeningAmount_AA(long amount)
{
	totalOpeningAmount_AA += amount;
}
void Bond::setTotalClosingAmount_AA(long amount)
{
	totalClosingAmount_AA += amount;
}
void Bond::setTotalOpeningAmount_AAA(long amount)
{
	totalOpeningAmount_AAA += amount;
}
void Bond::setTotalClosingAmount_AAA(long amount)
{
	totalClosingAmount_AAA += amount;
}
void Bond::setTotalOpeningAmount_B(long amount)
{
	totalOpeningAmount_B += amount;
}
void Bond::setTotalClosingAmount_B(long amount)
{
	totalClosingAmount_B += amount;
}
void Bond::setTotalOpeningAmount_BB(long amount)
{
	totalOpeningAmount_BB += amount;
}
void Bond::setTotalClosingAmount_BB(long amount)
{
	totalClosingAmount_BB += amount;
}
void Bond::setTotalOpeningAmount_BBB(long amount)
{
	totalOpeningAmount_BBB += amount;
}
void Bond::setTotalClosingAmount_BBB(long amount)
{
	totalClosingAmount_BBB += amount;
}
void Bond::setTotalOpeningAmount_C(long amount)
{
	totalOpeningAmount_C += amount;
}
void Bond::setTotalClosingAmount_C(long amount)
{
	totalClosingAmount_C += amount;
}
void Bond::setTotalOpeningAmount_CC(long amount)
{
	totalOpeningAmount_CC += amount;
}
void Bond::setTotalClosingAmount_CC(long amount)
{
	totalClosingAmount_CC += amount;
}
void Bond::setTotalOpeningAmount_CCC(long amount)
{
	totalOpeningAmount_CCC += amount;
}
void Bond::setTotalClosingAmount_CCC(long amount)
{
	totalClosingAmount_CCC += amount;
}
void Bond::setTotalOpeningAmount_D(long amount)
{
	totalOpeningAmount_D += amount;
}
void Bond::setTotalClosingAmount_D(long amount)
{
	totalClosingAmount_D += amount;
}
void Bond::setTotalOpeningAmount_IG(long amount)
{
	totalOpeningAmount_IG += amount;
}
void Bond::setTotalClosingAmount_IG(long amount)
{
	totalClosingAmount_IG += amount;
}
void Bond::setTotalOpeningAmount_junk(long amount)
{
	totalOpeningAmount_junk += amount;
}
void Bond::setTotalClosingAmount_junk(long amount)
{
	totalClosingAmount_junk += amount;
}

/** RISK **

void Bond::setTotalOpeningRisk_GM(double risk)
{
	totalOpeningRisk_GM += risk;
}
void Bond::setTotalClosingRisk_GM(double risk)
{
	totalClosingRisk_GM += risk;
}
void Bond::setTotalOpeningRisk_FG(double risk)
{
	totalOpeningRisk_FG += risk;
}
void Bond::setTotalClosingRisk_FG(double risk)
{
	totalClosingRisk_FG += risk;
}
void Bond::setTotalOpeningRisk_YU(double risk)
{
	totalOpeningRisk_YU += risk;
}
void Bond::setTotalClosingRisk_YU(double risk)
{
	totalClosingRisk_YU += risk;
}
void Bond::setTotalOpeningRisk_XY(double risk)
{
	totalOpeningRisk_XY += risk;
}
void Bond::setTotalClosingRisk_XY(double risk)
{
	totalClosingRisk_XY += risk;
}
void Bond::setTotalOpeningRisk_TT(double risk)
{
	totalOpeningRisk_TT += risk;
}
void Bond::setTotalClosingRisk_TT(double risk)
{
	totalClosingRisk_TT += risk;
}
void Bond::setTotalOpeningRisk_A(double risk)
{
	totalOpeningRisk_A += risk;
}
void Bond::setTotalClosingRisk_A(double risk)
{
	totalClosingRisk_A += risk;
}
void Bond::setTotalOpeningRisk_AA(double risk)
{
	totalOpeningRisk_AA += risk;
}
void Bond::setTotalClosingRisk_AA(double risk)
{
	totalClosingRisk_AA += risk;
}
void Bond::setTotalOpeningRisk_AAA(double risk)
{
	totalOpeningRisk_AAA += risk;
}
void Bond::setTotalClosingRisk_AAA(double risk)
{
	totalClosingRisk_AAA += risk;
}
void Bond::setTotalOpeningRisk_B(double risk)
{
	totalOpeningRisk_B += risk;
}
void Bond::setTotalClosingRisk_B(double risk)
{
	totalClosingRisk_B += risk;
}
void Bond::setTotalOpeningRisk_BB(double risk)
{
	totalOpeningRisk_BB += risk;
}
void Bond::setTotalClosingRisk_BB(double risk)
{
	totalClosingRisk_BB += risk;
}
void Bond::setTotalOpeningRisk_BBB(double risk)
{
	totalOpeningRisk_BBB += risk;
}
void Bond::setTotalClosingRisk_BBB(double risk)
{
	totalClosingRisk_BBB += risk;
}
void Bond::setTotalOpeningRisk_C(double risk)
{
	totalOpeningRisk_C += risk;
}
void Bond::setTotalClosingRisk_C(double risk)
{
	totalClosingRisk_C += risk;
}
void Bond::setTotalOpeningRisk_CC(double risk)
{
	totalOpeningRisk_CC += risk;
}
void Bond::setTotalClosingRisk_CC(double risk)
{
	totalClosingRisk_CC += risk;
}
void Bond::setTotalOpeningRisk_CCC(double risk)
{
	totalOpeningRisk_CCC += risk;
}
void Bond::setTotalClosingRisk_CCC(double risk)
{
	totalClosingRisk_CCC += risk;
}

void Bond::setTotalOpeningRisk_D(double risk)
{
	totalOpeningRisk_D += risk;
}
void Bond::setTotalClosingRisk_D(double risk)
{
	totalClosingRisk_D += risk;
}
void Bond::setTotalOpeningRisk_IG(double risk)
{
	totalOpeningRisk_IG += risk;
}
void Bond::setTotalClosingRisk_IG(double risk)
{
	totalClosingRisk_IG += risk;
}
void Bond::setTotalOpeningRisk_junk(double risk)
{
	totalOpeningRisk_junk += risk;
}
void Bond::setTotalClosingRisk_junk(double risk)
{
	totalClosingRisk_junk += risk;
}

/** LGD **
void Bond::setTotalOpeningLGD_GM(double lgd)
{
	totalOpeningLGD_GM += lgd;
}
void Bond::setTotalClosingLGD_GM(double lgd)
{
	totalClosingLGD_GM += lgd;
}
void Bond::setTotalOpeningLGD_FG(double lgd)
{
	totalOpeningLGD_FG += lgd;
}
void Bond::setTotalClosingLGD_FG(double lgd)
{
	totalClosingLGD_FG += lgd;
}
void Bond::setTotalOpeningLGD_YU(double lgd)
{
	totalOpeningLGD_YU += lgd;
}
void Bond::setTotalClosingLGD_YU(double lgd)
{
	totalClosingLGD_YU += lgd;
}
void Bond::setTotalOpeningLGD_XY(double lgd)
{
	totalOpeningLGD_XY += lgd;
}
void Bond::setTotalClosingLGD_XY(double lgd)
{
	totalClosingLGD_XY += lgd;
}
void Bond::setTotalOpeningLGD_TT(double lgd)
{
	totalOpeningLGD_TT += lgd;
}
void Bond::setTotalClosingLGD_TT(double lgd)
{
	totalClosingLGD_TT += lgd;
}
void Bond::setTotalOpeningLGD_A(double lgd)
{
	totalOpeningLGD_A += lgd;
}
void Bond::setTotalClosingLGD_A(double lgd)
{
	totalClosingLGD_A += lgd;
}
void Bond::setTotalOpeningLGD_AA(double lgd)
{
	totalOpeningLGD_AA += lgd;
}
void Bond::setTotalClosingLGD_AA(double lgd)
{
	totalClosingLGD_AA += lgd;
}
void Bond::setTotalOpeningLGD_AAA(double lgd)
{
	totalOpeningLGD_AAA += lgd;
}
void Bond::setTotalClosingLGD_AAA(double lgd)
{
	totalClosingLGD_AAA += lgd;
}
void Bond::setTotalOpeningLGD_B(double lgd)
{
	totalOpeningLGD_B += lgd;
}
void Bond::setTotalClosingLGD_B(double lgd)
{
	totalClosingLGD_B += lgd;
}
void Bond::setTotalOpeningLGD_BB(double lgd)
{
	totalOpeningLGD_BB += lgd;
}
void Bond::setTotalClosingLGD_BB(double lgd)
{
	totalClosingLGD_BB += lgd;
}
void Bond::setTotalOpeningLGD_BBB(double lgd)
{
	totalOpeningLGD_BBB += lgd;
}
void Bond::setTotalClosingLGD_BBB(double lgd)
{
	totalClosingLGD_BBB += lgd;
}
void Bond::setTotalOpeningLGD_C(double lgd)
{
	totalOpeningLGD_C += lgd;
}
void Bond::setTotalClosingLGD_C(double lgd)
{
	totalClosingLGD_C += lgd;
}
void Bond::setTotalOpeningLGD_CC(double lgd)
{
	totalOpeningLGD_CC += lgd;
}
void Bond::setTotalClosingLGD_CC(double lgd)
{
	totalClosingLGD_CC += lgd;
}
void Bond::setTotalOpeningLGD_CCC(double lgd)
{
	totalOpeningLGD_CCC += lgd;
}
void Bond::setTotalClosingLGD_CCC(double lgd)
{
	totalClosingLGD_CCC += lgd;
}void Bond::setTotalOpeningLGD_D(double lgd)
{
	totalOpeningLGD_D += lgd;
}
void Bond::setTotalClosingLGD_D(double lgd)
{
	totalClosingLGD_D += lgd;
}
void Bond::setTotalOpeningLGD_IG(double lgd)
{
	totalOpeningLGD_IG += lgd;
}
void Bond::setTotalClosingLGD_IG(double lgd)
{
	totalClosingLGD_IG += lgd;
}
void Bond::setTotalOpeningLGD_junk(double lgd)
{
	totalOpeningLGD_junk += lgd;
}
void Bond::setTotalClosingLGD_junk(double lgd)
{
	totalClosingLGD_junk += lgd;
}
*/

/*double Bond::totalRisk = 0;
double Bond::totalOpeningMarketValue = 0;
double Bond::totalClosingMarketValue = 0;
double Bond::totalOpeningLGD = 0;
double Bond::totalClosingLGD = 0;
long Bond::totalOpeningAmount = 0;
long Bond::totalClosingAmount = 0;
//double Bond::twoYrDVO1 = 0;
//double Bond::totalMarketValueUp = 0;
//double Bond::totalMarketValueDown = 0;


void Bond::setTotalRisk(double risk)
{
	totalRisk += risk;
}

void Bond::setTotalOpeningMarketValue(double marketValue)
{
	totalOpeningMarketValue += marketValue;
}

void Bond::setTotalClosingMarketValue(double marketValue)
{
	totalClosingMarketValue += marketValue;
}

void Bond::setTotalOpeningLGD(double lgd)
{
	totalOpeningLGD += lgd;
}

void Bond::setTotalClosingLGD(double lgd)
{
	totalClosingLGD += lgd;
}

void Bond::setTotalOpeningAmount(long amount)
{
	totalOpeningAmount += amount;
}

void Bond::setTotalClosingAmount(long amount)
{
	totalClosingAmount += amount;
}


void Bond::setTotalMarketValueUp(double marketValueUp)
{
	totalMarketValueUp += marketValueUp;
}

void Bond::setTotalMarketValueDown(double marketValueDown)
{
	totalMarketValueDown += marketValueDown;
}
*/
