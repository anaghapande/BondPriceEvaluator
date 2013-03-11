/*
 * Bond.h
 *
 *  Created on: Oct 15, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#ifndef BOND_H_
#define BOND_H_

#include <vector>
#include <string>
using namespace std;

class Bond
{
	public:
		Bond();
		virtual ~Bond();//virtual means that you have to override this
		string rateType;
		long period;
		long frequency;
		long amount;
		double coupon;
		double rate;
		double originalBondPrice;
		double dvo1;

		static vector<Bond> allBonds_2yr;
		static vector<Bond> allBonds_5yr;
		static vector<Bond> allBonds_10yr;
		static vector<Bond> allBonds_30yr;

	//FIELDS

		static double originalTotalRisk_2yr;
		static double originalTotalRisk_5yr;
		static double originalTotalRisk_10yr;
		static double originalTotalRisk_30yr;
		static double originalTotalMarketValue_2yr;
		static double originalTotalMarketValue_5yr;
		static double originalTotalMarketValue_10yr;
		static double originalTotalMarketValue_30yr;
		static double twoYrDVO1;

	 /********* NOTIONAL (AMOUNT) **********/
		static long totalOpeningAmount_GM;
		static long totalClosingAmount_GM;
		static long totalOpeningAmount_FG;
		static long totalClosingAmount_FG;
		static long totalOpeningAmount_YU;
		static long totalClosingAmount_YU;
		static long totalOpeningAmount_XY;
		static long totalClosingAmount_XY;
		static long totalOpeningAmount_TT;
		static long totalClosingAmount_TT;

		static long totalOpeningAmount_A;
		static long totalClosingAmount_A;
		static long totalOpeningAmount_AA;
		static long totalClosingAmount_AA;
		static long totalOpeningAmount_AAA;
		static long totalClosingAmount_AAA;
		static long totalOpeningAmount_B;
		static long totalClosingAmount_B;
		static long totalOpeningAmount_BB;
		static long totalClosingAmount_BB;
		static long totalOpeningAmount_BBB;
		static long totalClosingAmount_BBB;
		static long totalOpeningAmount_C;
		static long totalClosingAmount_C;
		static long totalOpeningAmount_CC;
		static long totalClosingAmount_CC;
		static long totalOpeningAmount_CCC;
		static long totalClosingAmount_CCC;
		static long totalOpeningAmount_D;
		static long totalClosingAmount_D;

		static long totalOpeningAmount_IG;
		static long totalClosingAmount_IG;
		static long totalOpeningAmount_junk;
		static long totalClosingAmount_junk;

	/**************** (RISK) ***************/
		static double totalOpeningRisk_GM;
		static double totalClosingRisk_GM;
		static double totalOpeningRisk_FG;
		static double totalClosingRisk_FG;
		static double totalOpeningRisk_YU;
		static double totalClosingRisk_YU;
		static double totalOpeningRisk_XY;
		static double totalClosingRisk_XY;
		static double totalOpeningRisk_TT;
		static double totalClosingRisk_TT;

		static double totalOpeningRisk_A;
		static double totalClosingRisk_A;
		static double totalOpeningRisk_AA;
		static double totalClosingRisk_AA;
		static double totalOpeningRisk_AAA;
		static double totalClosingRisk_AAA;
		static double totalOpeningRisk_B;
		static double totalClosingRisk_B;
		static double totalOpeningRisk_BB;
		static double totalClosingRisk_BB;
		static double totalOpeningRisk_BBB;
		static double totalClosingRisk_BBB;
		static double totalOpeningRisk_C;
		static double totalClosingRisk_C;
		static double totalOpeningRisk_CC;
		static double totalClosingRisk_CC;
		static double totalOpeningRisk_CCC;
		static double totalClosingRisk_CCC;
		static double totalOpeningRisk_D;
		static double totalClosingRisk_D;

		static double totalOpeningRisk_IG;
		static double totalClosingRisk_IG;
		static double totalOpeningRisk_junk;
		static double totalClosingRisk_junk;

	 /*************** (LGD) ***************/
		static double totalOpeningLGD_GM;
		static double totalClosingLGD_GM;
		static double totalOpeningLGD_FG;
		static double totalClosingLGD_FG;
		static double totalOpeningLGD_YU;
		static double totalClosingLGD_YU;
		static double totalOpeningLGD_XY;
		static double totalClosingLGD_XY;
		static double totalOpeningLGD_TT;
		static double totalClosingLGD_TT;

		static double totalOpeningLGD_A;
		static double totalClosingLGD_A;
		static double totalOpeningLGD_AA;
		static double totalClosingLGD_AA;
		static double totalOpeningLGD_AAA;
		static double totalClosingLGD_AAA;
		static double totalOpeningLGD_B;
		static double totalClosingLGD_B;
		static double totalOpeningLGD_BB;
		static double totalClosingLGD_BB;
		static double totalOpeningLGD_BBB;
		static double totalClosingLGD_BBB;
		static double totalOpeningLGD_C;
		static double totalClosingLGD_C;
		static double totalOpeningLGD_CC;
		static double totalClosingLGD_CC;
		static double totalOpeningLGD_CCC;
		static double totalClosingLGD_CCC;
		static double totalOpeningLGD_D;
		static double totalClosingLGD_D;

		static double totalOpeningLGD_IG;
		static double totalClosingLGD_IG;
		static double totalOpeningLGD_junk;
		static double totalClosingLGD_junk;
	/****************************************/

		//SETTERS

		/*
		void setPeriod(long period);
		void setFrequency(long freq);
		void setAmount(long amount);
		void setCoupon(double coupon);
		void setYieldRate (double rate);
		void setOriginalondPrice(double originalBondPrice);

		long getPeriod();
		long getFrequency();
		long getAmount();
		double getCoupon();
		double getYieldRate ();
		double getOriginalondPrice();

		static void addBond_2yr(Bond bond);
		static void addBond_5yr(Bond bond);
		static void addBond_10yr(Bond bond);
		static void addBond_30yr(Bond bond);


		static void setTotalOpeningAmount_GM(long amount);
		static void setTotalClosingAmount_GM(long amount);
		static void setTotalOpeningAmount_FG(long amount);
		static void setTotalClosingAmount_FG(long amount);
		static void setTotalOpeningAmount_YU(long amount);
		static void setTotalClosingAmount_YU(long amount);
		static void setTotalOpeningAmount_XY(long amount);
		static void setTotalClosingAmount_XY(long amount);
		static void setTotalOpeningAmount_TT(long amount);
		static void setTotalClosingAmount_TT(long amount);
		static void setTotalOpeningAmount_A(long amount);
		static void setTotalClosingAmount_A(long amount);
		static void setTotalOpeningAmount_AA(long amount);
		static void setTotalClosingAmount_AA(long amount);
		static void setTotalOpeningAmount_AAA(long amount);
		static void setTotalClosingAmount_AAA(long amount);
		static void setTotalOpeningAmount_B(long amount);
		static void setTotalClosingAmount_B(long amount);
		static void setTotalOpeningAmount_BB(long amount);
		static void setTotalClosingAmount_BB(long amount);
		static void setTotalOpeningAmount_BBB(long amount);
		static void setTotalClosingAmount_BBB(long amount);
		static void setTotalOpeningAmount_C(long amount);
		static void setTotalClosingAmount_C(long amount);
		static void setTotalOpeningAmount_CC(long amount);
		static void setTotalClosingAmount_CC(long amount);
		static void setTotalOpeningAmount_CCC(long amount);
		static void setTotalClosingAmount_CCC(long amount);
		static void setTotalOpeningAmount_D(long amount);
		static void setTotalClosingAmount_D(long amount);
		static void setTotalOpeningAmount_IG(long amount);
		static void setTotalClosingAmount_IG(long amount);
		static void setTotalOpeningAmount_junk(long amount);
		static void setTotalClosingAmount_junk(long amount);

		static void setTotalOpeningRisk_GM(double risk);
		static void setTotalClosingRisk_GM(double risk);
		static void setTotalOpeningRisk_FG(double risk);
		static void setTotalClosingRisk_FG(double risk);
		static void setTotalOpeningRisk_YU(double risk);
		static void setTotalClosingRisk_YU(double risk);
		static void setTotalOpeningRisk_XY(double risk);
		static void setTotalClosingRisk_XY(double risk);
		static void setTotalOpeningRisk_TT(double risk);
		static void setTotalClosingRisk_TT(double risk);
		static void setTotalOpeningRisk_A(double risk);
		static void setTotalClosingRisk_A(double risk);
		static void setTotalOpeningRisk_AA(double risk);
		static void setTotalClosingRisk_AA(double risk);
		static void setTotalOpeningRisk_AAA(double risk);
		static void setTotalClosingRisk_AAA(double risk);
		static void setTotalOpeningRisk_B(double risk);
		static void setTotalClosingRisk_B(double risk);
		static void setTotalOpeningRisk_BB(double risk);
		static void setTotalClosingRisk_BB(double risk);
		static void setTotalOpeningRisk_BBB(double risk);
		static void setTotalClosingRisk_BBB(double risk);
		static void setTotalOpeningRisk_C(double risk);
		static void setTotalClosingRisk_C(double risk);
		static void setTotalOpeningRisk_CC(double risk);
		static void setTotalClosingRisk_CC(double risk);
		static void setTotalOpeningRisk_CCC(double risk);
		static void setTotalClosingRisk_CCC(double risk);
		static void setTotalOpeningRisk_D(double risk);
		static void setTotalClosingRisk_D(double risk);
		static void setTotalOpeningRisk_IG(double risk);
		static void setTotalClosingRisk_IG(double risk);
		static void setTotalOpeningRisk_junk(double risk);
		static void setTotalClosingRisk_junk(double risk);

		static void setTotalOpeningLGD_GM(double lgd);
		static void setTotalClosingLGD_GM(double lgd);
		static void setTotalOpeningLGD_FG(double lgd);
		static void setTotalClosingLGD_FG(double lgd);
		static void setTotalOpeningLGD_YU(double lgd);
		static void setTotalClosingLGD_YU(double lgd);
		static void setTotalOpeningLGD_XY(double lgd);
		static void setTotalClosingLGD_XY(double lgd);
		static void setTotalOpeningLGD_TT(double lgd);
		static void setTotalClosingLGD_TT(double lgd);
		static void setTotalOpeningLGD_A(double lgd);
		static void setTotalClosingLGD_A(double lgd);
		static void setTotalOpeningLGD_AA(double lgd);
		static void setTotalClosingLGD_AA(double lgd);
		static void setTotalOpeningLGD_AAA(double lgd);
		static void setTotalClosingLGD_AAA(double lgd);
		static void setTotalOpeningLGD_B(double lgd);
		static void setTotalClosingLGD_B(double lgd);
		static void setTotalOpeningLGD_BB(double lgd);
		static void setTotalClosingLGD_BB(double lgd);
		static void setTotalOpeningLGD_BBB(double lgd);
		static void setTotalClosingLGD_BBB(double lgd);
		static void setTotalOpeningLGD_C(double lgd);
		static void setTotalClosingLGD_C(double lgd);
		static void setTotalOpeningLGD_CC(double lgd);
		static void setTotalClosingLGD_CC(double lgd);
		static void setTotalOpeningLGD_CCC(double lgd);
		static void setTotalClosingLGD_CCC(double lgd);
		static void setTotalOpeningLGD_D(double lgd);
		static void setTotalClosingLGD_D(double lgd);
		static void setTotalOpeningLGD_IG(double lgd);
		static void setTotalClosingLGD_IG(double lgd);
		static void setTotalOpeningLGD_junk(double lgd);
		static void setTotalClosingLGD_junk(double lgd);
		*/

		/*
		static void setTotalRisk(double risk);
		static void setTotalOpeningMarketValue(double marketValue);
		static void setTotalClosingMarketValue(double marketValue);
		static void setTotalOpeningLGD(double lgd);
		static void setTotalClosingLGD(double lgd);
		static void setTotalOpeningAmount(long amount);
		static void setTotalClosingAmount(long amount);
		//static void setTotalOpeningAmount(long amount);
		//static void setTotalClosingAmount(long amount);
		//static void setTotalMarketValueUp(double marketValueUp);
		//static void setTotalMarketValueDown(double marketValueDown);

		static double totalRisk; //totalRisk is now the bucket risk for 30yr maturity
		static double totalOpeningMarketValue;
		static double totalClosingMarketValue;
		static double totalOpeningLGD;
		static double totalClosingLGD;
		//static long totalOpeningAmount;
		//static long totalClosingAmount;

		//static double twoYrDVO1;
		//static double totalMarketValueUp;
		//static double totalMarketValueDown;
		*/
};

#endif /* BOND_H_ */
