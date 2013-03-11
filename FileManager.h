/*
 * FileManager.h
 *
 *  Created on: Oct 18, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_
#define TAB "\t"

#include "Bond.h"
#include "BondCalculator.h"
#include "PnL.h"

using namespace std;

//make the methods and variables static to improve speed
class FileManager
{
	public:
		FileManager();
		virtual ~FileManager();
		static const void processTradingBooks(char* argv[]);
		static const vector<double>* returnStaticResults(long confidenceInterval);
		static const double* returnDynamicResults();
		static const long* returnGraphData();
		static const double* calculateBumpedMarketValue_Risk_TwoYrHedge(int bump, string bucket);
		static const void back_To_Original();

	private:
		static const void processOpeningTradingbook(const char* openingbookFile);
		static const void processClosingTradingbook(const char* closingbookFile);
		static const double calculateYieldFromSpread(double rate, long period);
		static const void readYieldCurve(const char* yieldcurveFile);
		//static const void calculatePnL_Closing(string securityIdFile, long amount);
		static const void calculatePnL_Opening(string securityIdFile, string rateType, long period, double coupon,
				                             long frequency, double bondPrice, long amount, double rate);
		static vector<double>* readHistoricalYieldcurve(string filename);
		static const void updatePnLVector(string filename, vector<PnL> *temp);
		static const void updateBookVector_Opening(string filename);
		static const void updateBookVector_Closing(string filename, long amount);
		static const bool isFileRead(string filename);
		static const void updateAmount_Risk_LGD(string tradingbook, string ticker, string quality, long amount, double risk);
		static const void updateRisk_MarketValue(double risk, double marketValue, long period);
		static const void createAndBucketizeBond(string rateType, long period, long frequency, long amount, double coupon, double rate, double bondPrice, double dvo1);
		static const void addVaRs(long confidenceInterval);
		static const void addNotional_Risk_Lgd();
		static const void addRisk_MarketValue_TwoYrHedge();

		static vector<long> yieldPeriods;
		static vector<double> yieldRates;
		static vector<PnL> book_vector;
		//static vector<PnL> book_vector_closing;
		static bool isYieldCurveRead;

};

#endif /* FILEMANAGER_H_ */
