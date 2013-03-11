/*
 * FileManager.cpp
 *
 *  Created on: Oct 18, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>
#include <algorithm>
//#include <vector>

#include "FileManager.h"
#define TAB "\t"

static FILE* openingTradingbook;
static FILE* closingTradingbook;//now we need two tradingbook file readers
static FILE* yieldcurve;
static FILE* historyBondFile;
static FILE* historyYieldcurve;
static FILE* errorLog = fopen("error_logfile", "w");

static BondCalculator calculator;
//static double results[200];
static vector<double>* results = new vector<double>;
static long graphData[12];
static double dynamicResults[4];
static double dynamicValues[13];
static int bump_2yr = 0;
static int bump_5yr = 0;
static int bump_10yr = 0;
static int bump_30yr = 0;


/*****************************************************/

static vector<PnL> PnL_SBB01;
static vector<PnL> PnL_SBB02;
static vector<PnL> PnL_SBB03;
static vector<PnL> PnL_SBB04;
static vector<PnL> PnL_SBB05;
static vector<PnL> PnL_SBB06;
static vector<PnL> PnL_SBB07;
static vector<PnL> PnL_SBB08;
static vector<PnL> PnL_SBB09;
static vector<PnL> PnL_SBB10;
static vector<PnL> PnL_SBB11;
static vector<PnL> PnL_SBB12;

static vector<double>* T2 = new vector<double>;
static vector<double>* T5 = new vector<double>;
static vector<double>* T10 = new vector<double>;
static vector<double>* T30 = new vector<double>;

static vector<string> filesRead;
static string filename;

/***************************************************/

vector<PnL> FileManager::book_vector;
//vector<PnL> FileManager::book_vector_closing;

vector<long> FileManager::yieldPeriods;
vector<double> FileManager::yieldRates;
bool FileManager::isYieldCurveRead = false;

static bool isBookVectorAccessed = false;
//static bool isClosingBookVectorAccessed = false;
static bool isHistoricalYieldcurveRead = false;
static bool isTradingbookProcessed = false;


FileManager::FileManager(){ }

FileManager::~FileManager(){ }


const void FileManager::processTradingBooks(char* argv[])
{
	if(isTradingbookProcessed)
	{
		return;
	}
	readYieldCurve(argv[3]);

	processOpeningTradingbook(argv[1]);

	processClosingTradingbook(argv[2]);

	isTradingbookProcessed = true;
}

const void FileManager::processOpeningTradingbook(const char* openingbookFile)
{
	string securityIdFile, ticker, rateType, quality;
	long settlementDate, maturityDate, frequency, amount, period;
	double rate, coupon, bondPrice, dvo1;

	char bondLine[256];

	openingTradingbook = fopen (openingbookFile, "r");

	if(openingTradingbook == NULL)
	{
		fprintf(errorLog,"File 'tradingbook_opening.txt' cannot be opened!");
		exit(1);
	}

	while(fgets(bondLine,256,openingTradingbook))
	{
		if('#' == bondLine[0])
		{
			continue;
		}

		securityIdFile = strtok(bondLine, " \t");

		ticker = strtok(NULL, " \t");

		settlementDate = atol(strtok(NULL, " \t"));

		coupon = atof(strtok(NULL, " \t"));

		maturityDate = atol(strtok(NULL, " \t"));

		frequency = atol(strtok(NULL, " \t"));

		rateType = strtok(NULL, " \t");

		rate = atof(strtok(NULL, " \t"));

		quality = strtok(NULL, " \t");

		amount = atol(strtok(NULL, " \n"));

		period = ((maturityDate - settlementDate)/10000);

		if (rateType == "SPREAD")
		{
			rate = calculateYieldFromSpread(rate/100.0, period);
		}

		rate /= 100;

		/***/
		bondPrice = calculator.calculateBondPrice(0, 1, period, coupon, frequency, rate);
		dvo1 = calculator.calculateDVO1(period, coupon, frequency, rate, bondPrice);
		//risk = dvo1 * amount / 100;

		updateAmount_Risk_LGD("opening", ticker, quality, amount, dvo1 * amount / 100);

		//calculate PnL
		calculatePnL_Opening(securityIdFile, rateType, period, coupon, frequency, bondPrice, amount, rate);
	}

	fclose(openingTradingbook);
}

const void FileManager::processClosingTradingbook(const char* closingbookFile)
{
	string securityIdFile, ticker, rateType, quality;
	long settlementDate, maturityDate, frequency, amount, period;
	double rate, coupon, bondPrice, dvo1, risk;

	char bondLine[256];

	closingTradingbook = fopen(closingbookFile , "r");

	if(closingTradingbook == NULL)
	{
		fprintf(errorLog,"File 'tradingbook_closing.txt' cannot be opened!");
		exit(1);
	}

	while(fgets(bondLine,256,closingTradingbook))
	{
		if('#' == bondLine[0])
		{
			continue;
		}

		securityIdFile = strtok(bondLine, " \t");

		ticker = strtok(NULL, " \t");

		settlementDate = atol(strtok(NULL, " \t"));

		coupon = atof(strtok(NULL, " \t"));

		maturityDate = atol(strtok(NULL, " \t"));

		frequency = atol(strtok(NULL, " \t"));

		rateType = strtok(NULL, " \t");

		rate = atof(strtok(NULL, " \t"));

		quality = strtok(NULL, " \t");

		amount = atol(strtok(NULL, " \n"));

		period = ((maturityDate - settlementDate)/10000);

		if (rateType == "SPREAD")
		{
			rate = calculateYieldFromSpread(rate/100.0, period);
		}

		rate /= 100;

		/***/
		bondPrice = calculator.calculateBondPrice(0, 1, period, coupon, frequency, rate);
		dvo1 = calculator.calculateDVO1(period, coupon, frequency, rate, bondPrice);
		risk = dvo1 * amount / 100;
		//marketValue = bondPrice/100 * amount;

		updateAmount_Risk_LGD("closing", ticker, quality, amount, risk);
		updateRisk_MarketValue(risk, bondPrice/100 * amount, period);
		createAndBucketizeBond(rateType, period, frequency, amount, coupon, rate, bondPrice, dvo1);

		//calculate PnL
		updateBookVector_Closing(securityIdFile, amount);

	}
	fclose(closingTradingbook);
}

const void FileManager::createAndBucketizeBond(string rateType, long period, long frequency, long amount, double coupon, double rate, double bondPrice, double dvo1)
{
	//check whether Bond bond is faster than Bond* bond = new Bond()
	Bond bond;
	bond.coupon = coupon;
	bond.frequency = frequency;
	bond.period = period;
	bond.rateType = rateType;
	bond.rate = rate;
	bond.amount = amount;
	bond.dvo1 = dvo1;
	bond.originalBondPrice = bondPrice;

	if(period > 5 && period <= 10)
	{
		Bond::allBonds_10yr.push_back(bond);
	}
	else if(period > 0 && period <= 2)
	{
		Bond::allBonds_2yr.push_back(bond);
	}
	else if(period > 2 && period <= 5)
	{
		Bond::allBonds_5yr.push_back(bond);
	}
	else if(period > 10 && period <= 30)
	{
		Bond::allBonds_30yr.push_back(bond);
	}
}
//for opening tradingbook
const void FileManager::calculatePnL_Opening(string securityIdFile, string rateType, long period, double coupon, long frequency, double bondPrice, long amount, double rate)
{
	//check if this is optimal and actually saves time on the million bond, else comment it out
	if(isFileRead(securityIdFile))
	{
		updateBookVector_Opening(securityIdFile);
		return;
	}

	filename = "var/" + securityIdFile + ".txt";

	historyBondFile = fopen(filename.c_str(), "r");

	if(historyBondFile == NULL)
	{
		string file = "File '" +  filename + "' cannot be opened!";
		fprintf(errorLog, file.c_str());
		exit(1);
	}

	char bondLine[256];

	unsigned int historyBondCounter = 0;

	double bp_change;
	double new_yield;
	double new_price;
	double pnl, bondPriceChange_rate, bondPriceChange_spread;

	double previousYield = -1;
	double currentYield;

	//vector<double> * temp = new vector<double>;
	vector<PnL> * temp = new vector<PnL>;
	vector<double>* yieldcurve_vector;


	PnL Pnl;

	//do I really need to maintain two yields?
	while(fgets(bondLine,256,historyBondFile))
	{

		if('#' == bondLine[0])
		{
			continue;
		}

		strtok(bondLine, " \t");
		strtok(NULL, " \t");

		currentYield = atof(strtok(NULL, " \t"));

		if (previousYield != -1)
		{
			if (rateType == "YIELD")
			{
				bp_change = (currentYield - previousYield);
				bondPriceChange_rate = (calculator.calculateBondPrice(0, 0, period, coupon, frequency, rate + (bp_change / 100)) - bondPrice) / 100.0;
				Pnl.bondPriceChange_rate = bondPriceChange_rate;
				Pnl.bondPriceChange_spread = 0.0; //may need to comment this out
				Pnl.ratePnL_opening = amount * bondPriceChange_rate;
				Pnl.spreadPnL_opening = 0.0;
			}
			else if (rateType == "SPREAD")
			{
				if(!isHistoricalYieldcurveRead)
				{
					string yieldcurveFilename = strtok(NULL, " \n");
					yieldcurve_vector = readHistoricalYieldcurve(yieldcurveFilename);
				}

				bp_change = (currentYield - previousYield)/100 + yieldcurve_vector->at(historyBondCounter);

				bondPriceChange_rate = (calculator.calculateBondPrice(0, 0, period, coupon, frequency, rate + (yieldcurve_vector->at(historyBondCounter)/100)) - bondPrice) / 100.0;
				bondPriceChange_spread = (calculator.calculateBondPrice(0, 0, period, coupon, frequency, rate + ((currentYield - previousYield)/10000)) - bondPrice) / 100.0;

				Pnl.bondPriceChange_rate = bondPriceChange_rate;
				Pnl.bondPriceChange_spread = bondPriceChange_spread;
				Pnl.ratePnL_opening = amount * bondPriceChange_rate;
				Pnl.spreadPnL_opening = amount * bondPriceChange_spread;
			}


			//new_yield = closing_yield + bp_change
			new_yield = rate + (bp_change / 100);

			//new_price = calculate bond price using new_yield
			new_price = calculator.calculateBondPrice(0, 0, period, coupon, frequency, new_yield);

			//price_change = new_price - closing_price; PnL = amount * price_change / 100
			pnl = amount  * (new_price - bondPrice) / 100.0;

			Pnl.bondPriceChange_combined = (new_price - bondPrice) / 100.0;
			Pnl.combinedPnL_opening = pnl;

			   /*
				cout<<"bp"<<bp_change*100<< endl;
				cout<<"new yield"<<new_yield<< endl;
				cout<<"old price: "<<bondPrice<<endl;
				cout<<"new price: "<<new_price<<endl;
				cout<<"pnl:"<<PnL*1000<<endl;
			   */

			//update history vector with PnL value
			temp->push_back(Pnl);

			if(isBookVectorAccessed)
			{
				//book_vector.at(historyBondCounter) += pnl;
				book_vector.at(historyBondCounter).combinedPnL_opening += pnl;
				book_vector.at(historyBondCounter).ratePnL_opening += Pnl.ratePnL_opening;
				book_vector.at(historyBondCounter).spreadPnL_opening += Pnl.spreadPnL_opening;
			}
			else
			{
				//book_vector.push_back(pnl);
				book_vector.push_back(Pnl);
			}

			 historyBondCounter++;
		}

		previousYield = currentYield;
	}

	isBookVectorAccessed = true; //do these 3 things too for calculatePnL_SPREAD
	isHistoricalYieldcurveRead = false;
	updatePnLVector(securityIdFile, temp);
	filesRead.push_back(securityIdFile);

	fclose(historyBondFile);
}

const void FileManager::updateRisk_MarketValue(double risk, double marketValue, long period)
{
	if(period > 5 && period <= 10)
	{
		Bond::originalTotalRisk_10yr += risk;
		Bond::originalTotalMarketValue_10yr += marketValue;
	}
	else if(period > 0 && period <= 2)
	{
		Bond::originalTotalRisk_2yr += risk;
		Bond::originalTotalMarketValue_2yr += marketValue;
	}
	else if(period > 2 && period <= 5)
	{
		Bond::originalTotalRisk_5yr += risk;
		Bond::originalTotalMarketValue_5yr += marketValue;
	}
	else if(period > 10 && period <= 30)
	{
		Bond::originalTotalRisk_30yr += risk;
		Bond::originalTotalMarketValue_30yr += marketValue;
	}
}
vector<double>* FileManager::readHistoricalYieldcurve(string filename)
{
	string full_filename = "var/" + filename + ".txt";

	historyYieldcurve = fopen(full_filename.c_str(), "r");

	if(historyYieldcurve == NULL)
	{
		string file = "File '" +  full_filename + "' cannot be opened!";
		fprintf(errorLog, file.c_str());
		exit(1);
	}

	char bondLine[256];

	double previousYield = -1;
	double currentYield;

	vector<double>* yieldcurve_vector;

	while(fgets(bondLine,256,historyYieldcurve))
	{

		if('#' == bondLine[0])
		{
			continue;
		}

		strtok(bondLine, " \t");
		strtok(NULL, " \t");

		currentYield = atof(strtok(NULL, " \t"));

		if (previousYield != -1)
		{
			if(filename == "T2")
			{
				T2->push_back((currentYield - previousYield));
			}
			else if(filename == "T5")
			{
				T5->push_back((currentYield - previousYield));
			}
			else if(filename == "T10")
			{
				T10->push_back((currentYield - previousYield));
			}
			else if(filename == "T30")
			{
				T30->push_back((currentYield - previousYield));
			}
		 }
		else
		{
		     yieldcurve_vector = (filename == "T2" ? T2
					                       : filename == "T5" ? T5
					                                    : filename == "T10" ? T10
					                            				    : T30);
		}

		previousYield = currentYield;
	}
	isHistoricalYieldcurveRead = true;

	fclose(historyYieldcurve);

	return yieldcurve_vector;
}

const void FileManager::updateBookVector_Opening(string filename)
{
	if(filename == "SBB_0001")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			//book_vector.at(i) += PnL_SBB01.at(i);
			book_vector.at(i).combinedPnL_opening += PnL_SBB01.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB01.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB01.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0002")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB02.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB02.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB02.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0003")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB03.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB03.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB03.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0004")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB04.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB04.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB04.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0005")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB05.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB05.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB05.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0006")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB06.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB06.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB06.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0007")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB07.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB07.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB07.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0008")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB08.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB08.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB08.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0009")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB09.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB09.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB09.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0010")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB10.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB10.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB10.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0011")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB11.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB11.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB11.at(i).spreadPnL_opening;
		}
		return;
	}
	if(filename == "SBB_0012")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			book_vector.at(i).combinedPnL_opening += PnL_SBB12.at(i).combinedPnL_opening;
			book_vector.at(i).ratePnL_opening += PnL_SBB12.at(i).ratePnL_opening;
			book_vector.at(i).spreadPnL_opening += PnL_SBB12.at(i).spreadPnL_opening;
		}
		return;
	}
}

const void FileManager::updateBookVector_Closing(string filename, long amount)
{
	if(filename == "SBB_0001")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			// you can make this check only once for SBB_xxx
			if(!PnL_SBB01.at(i).isClosingAccessed)
			{
				PnL_SBB01.at(i).combinedPnL_closing = PnL_SBB01.at(i).bondPriceChange_combined * amount;
				PnL_SBB01.at(i).ratePnL_closing = PnL_SBB01.at(i).bondPriceChange_rate * amount;
				PnL_SBB01.at(i).spreadPnL_closing = PnL_SBB01.at(i).bondPriceChange_spread * amount;
				PnL_SBB01.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB01.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB01.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB01.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0002")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB02.at(i).isClosingAccessed)
			{
				PnL_SBB02.at(i).combinedPnL_closing = PnL_SBB02.at(i).bondPriceChange_combined * amount;
				PnL_SBB02.at(i).ratePnL_closing = PnL_SBB02.at(i).bondPriceChange_rate * amount;
				PnL_SBB02.at(i).spreadPnL_closing = PnL_SBB02.at(i).bondPriceChange_spread * amount;
				PnL_SBB02.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB02.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB02.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB02.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0003")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB03.at(i).isClosingAccessed)
			{
				PnL_SBB03.at(i).combinedPnL_closing = PnL_SBB03.at(i).bondPriceChange_combined * amount;
				PnL_SBB03.at(i).ratePnL_closing = PnL_SBB03.at(i).bondPriceChange_rate * amount;
				PnL_SBB03.at(i).spreadPnL_closing = PnL_SBB03.at(i).bondPriceChange_spread * amount;
				PnL_SBB03.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB03.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB03.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB03.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0004")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB04.at(i).isClosingAccessed)
			{
				PnL_SBB04.at(i).combinedPnL_closing = PnL_SBB04.at(i).bondPriceChange_combined * amount;
				PnL_SBB04.at(i).ratePnL_closing = PnL_SBB04.at(i).bondPriceChange_rate * amount;
				PnL_SBB04.at(i).spreadPnL_closing = PnL_SBB04.at(i).bondPriceChange_spread * amount;
				PnL_SBB04.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB04.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB04.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB04.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0005")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB05.at(i).isClosingAccessed)
			{
				PnL_SBB05.at(i).combinedPnL_closing = PnL_SBB05.at(i).bondPriceChange_combined * amount;
				PnL_SBB05.at(i).ratePnL_closing = PnL_SBB05.at(i).bondPriceChange_rate * amount;
				PnL_SBB05.at(i).spreadPnL_closing = PnL_SBB05.at(i).bondPriceChange_spread * amount;
				PnL_SBB05.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB05.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB05.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB05.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0006")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB06.at(i).isClosingAccessed)
			{
				PnL_SBB06.at(i).combinedPnL_closing = PnL_SBB06.at(i).bondPriceChange_combined * amount;
				PnL_SBB06.at(i).ratePnL_closing = PnL_SBB06.at(i).bondPriceChange_rate * amount;
				PnL_SBB06.at(i).spreadPnL_closing = PnL_SBB06.at(i).bondPriceChange_spread * amount;
				PnL_SBB06.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB06.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB06.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB06.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0007")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB07.at(i).isClosingAccessed)
			{
				PnL_SBB07.at(i).combinedPnL_closing = PnL_SBB07.at(i).bondPriceChange_combined * amount;
				PnL_SBB07.at(i).ratePnL_closing = PnL_SBB07.at(i).bondPriceChange_rate * amount;
				PnL_SBB07.at(i).spreadPnL_closing = PnL_SBB07.at(i).bondPriceChange_spread * amount;
				PnL_SBB07.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB07.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB07.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB07.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0008")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB08.at(i).isClosingAccessed)
			{
				PnL_SBB08.at(i).combinedPnL_closing = PnL_SBB08.at(i).bondPriceChange_combined * amount;
				PnL_SBB08.at(i).ratePnL_closing = PnL_SBB08.at(i).bondPriceChange_rate * amount;
				PnL_SBB08.at(i).spreadPnL_closing = PnL_SBB08.at(i).bondPriceChange_spread * amount;
				PnL_SBB08.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB08.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB08.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB08.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0009")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB09.at(i).isClosingAccessed)
			{
				PnL_SBB09.at(i).combinedPnL_closing = PnL_SBB09.at(i).bondPriceChange_combined * amount;
				PnL_SBB09.at(i).ratePnL_closing = PnL_SBB09.at(i).bondPriceChange_rate * amount;
				PnL_SBB09.at(i).spreadPnL_closing = PnL_SBB09.at(i).bondPriceChange_spread * amount;
				PnL_SBB09.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB09.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB09.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB09.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0010")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB10.at(i).isClosingAccessed)
			{
				PnL_SBB10.at(i).combinedPnL_closing = PnL_SBB10.at(i).bondPriceChange_combined * amount;
				PnL_SBB10.at(i).ratePnL_closing = PnL_SBB10.at(i).bondPriceChange_rate * amount;
				PnL_SBB10.at(i).spreadPnL_closing = PnL_SBB10.at(i).bondPriceChange_spread * amount;
				PnL_SBB10.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB10.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB10.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB10.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0011")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB11.at(i).isClosingAccessed)
			{
				PnL_SBB11.at(i).combinedPnL_closing = PnL_SBB11.at(i).bondPriceChange_combined * amount;
				PnL_SBB11.at(i).ratePnL_closing = PnL_SBB11.at(i).bondPriceChange_rate * amount;
				PnL_SBB11.at(i).spreadPnL_closing = PnL_SBB11.at(i).bondPriceChange_spread * amount;
				PnL_SBB11.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB11.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB11.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB11.at(i).spreadPnL_closing;
		}
		return;
	}
	if(filename == "SBB_0012")
	{
		for(unsigned i = 0; i < book_vector.size(); i++)
		{
			if(!PnL_SBB12.at(i).isClosingAccessed)
			{
				PnL_SBB12.at(i).combinedPnL_closing = PnL_SBB12.at(i).bondPriceChange_combined * amount;
				PnL_SBB12.at(i).ratePnL_closing = PnL_SBB12.at(i).bondPriceChange_rate * amount;
				PnL_SBB12.at(i).spreadPnL_closing = PnL_SBB12.at(i).bondPriceChange_spread * amount;
				PnL_SBB12.at(i).isClosingAccessed = true;
			}

			book_vector.at(i).combinedPnL_closing += PnL_SBB12.at(i).combinedPnL_closing;
			book_vector.at(i).ratePnL_closing += PnL_SBB12.at(i).ratePnL_closing;
			book_vector.at(i).spreadPnL_closing += PnL_SBB12.at(i).spreadPnL_closing;
		}
		return;
	}
}

const void FileManager::updatePnLVector(string filename, vector<PnL> *temp)
{

	if(filename == "SBB_0001")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB01.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0002")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB02.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0003")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB03.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0004")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB04.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0005")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB05.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0006")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB06.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0007")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB07.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0008")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB08.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0009")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB09.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0010")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB10.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0011")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB11.push_back(temp->at(i));
		}
		return;
	}
	if(filename == "SBB_0012")
	{
		for(unsigned i = 0; i < temp->size(); i++)
		{
			PnL_SBB12.push_back(temp->at(i));
		}
		return;
	}
}

const bool FileManager::isFileRead(string filename)
{
	  vector<string>::iterator iter;

	  iter = find (filesRead.begin(), filesRead.end(), filename);

	  if(iter == filesRead.end())
	  {
		return false;
	  }
	  else
	  {
		  return true;
	  }
}

const vector<double>* FileManager::returnStaticResults(long confidenceInterval)
{
	addNotional_Risk_Lgd();
	addVaRs(confidenceInterval);
	//addRisk_MarketValue_TwoYrHedge();

	return results;
}

const void FileManager::addVaRs(long confidenceInterval)
{
	int num_of_PnL = book_vector.size();
	int index = num_of_PnL - (num_of_PnL * confidenceInterval / 100.0);

	//VaR_Opening
	sort(book_vector.begin(), book_vector.end(), PnL::compareTo_opening);
	PnL pnl_opening = book_vector.at(index);

	//VaR_Closing
	sort(book_vector.begin(), book_vector.end(), PnL::compareTo_closing);
	PnL pnl_closing = book_vector.at(index);

	/*spread_opening, spread_closing, spread_change
	  rate_opening, rate_closing, rate_change
	  combined_opening, combined_closing, combined_change
	*/
	results->push_back(pnl_opening.spreadPnL_opening/1000.0);
	results->push_back(pnl_closing.spreadPnL_closing /1000.0);
	results->push_back((pnl_closing.spreadPnL_closing - pnl_opening.spreadPnL_opening)/1000.0);
	results->push_back(pnl_opening.ratePnL_opening/1000.0);
	results->push_back(pnl_closing.ratePnL_closing /1000.0);
	results->push_back((pnl_closing.ratePnL_closing - pnl_opening.ratePnL_opening)/1000.0);
	results->push_back(pnl_opening.combinedPnL_opening/1000.0);
	results->push_back(pnl_closing.combinedPnL_closing /1000.0);
	results->push_back((pnl_closing.combinedPnL_closing - pnl_opening.combinedPnL_opening)/1000.0);
}

const void FileManager::readYieldCurve(const char* yieldcurveFile)
{
	string rateType;
	long settlementDate, maturityDate, frequency, period;
	double rate, coupon;
	char yieldcurveBondLine[256];

	yieldcurve = fopen (yieldcurveFile , "r");

	if(yieldcurve == NULL)
	{
		fprintf(errorLog,"File 'yieldcurve.txt' cannot be opened!");
  		exit(1);
	}

	while(fgets(yieldcurveBondLine,256,yieldcurve))
	{
		if('#' == yieldcurveBondLine[0])
		{
			continue;
		}

		strtok(yieldcurveBondLine, " \t");
		strtok(NULL, " \t");

		settlementDate = atol(strtok(NULL, " \t"));

		coupon = atof(strtok(NULL, " \t"));

		maturityDate = atol(strtok(NULL, " \t"));

		frequency = atol(strtok(NULL, " \t"));

		rateType = strtok(NULL, " \t");

		rate = atof(strtok(NULL, " \t"));

		period = (maturityDate-settlementDate)/10000;

		yieldPeriods.push_back(period);
		yieldRates.push_back(rate);

		if (period == 2)
		{
			rate = rate/100;
			double bondPrice = calculator.calculateBondPrice(0, 1, period, coupon, frequency, rate);
			Bond::twoYrDVO1 = calculator.calculateDVO1(period, coupon, frequency, rate, bondPrice); //am I supposed to divide by 100?
		}
	}
	isYieldCurveRead = true;

	fclose(yieldcurve);
}

const double FileManager::calculateYieldFromSpread(double rate, long period)
{
	long difference = INT_MAX;
	double yieldRate;
	long temp;

	for (unsigned int i = 0; i < yieldPeriods.size(); i++)
	{
		temp = fabs(period - yieldPeriods[i]);

		if (temp == 0)
		{
			yieldRate = yieldRates[i];
			break;
		}
		else if (temp < difference)
		{
			difference = temp;
			yieldRate = yieldRates[i];
		}
		else if (temp == difference)
		{
			yieldRate = min(yieldRate, yieldRates[i]);
		}
	}

	return (yieldRate + rate);
}

const void FileManager::updateAmount_Risk_LGD(string tradingbook, string ticker ,string quality, long amount, double risk)
{
	double lgd = calculator.calculateAmountAdjustedLGD(quality, amount);

	if(tradingbook == "opening")
	{
		//categorized according to ticker
		if(ticker == "GM")
		{
			Bond::totalOpeningAmount_GM += amount;
			Bond::totalOpeningRisk_GM += risk;
			Bond::totalOpeningLGD_GM += lgd;
		}
		else if(ticker == "FG")
		{
			Bond::totalOpeningAmount_FG += amount;
			Bond::totalOpeningRisk_FG += risk;
			Bond::totalOpeningLGD_FG += lgd;
		}
		else if(ticker == "XY")
		{
			Bond::totalOpeningAmount_XY += amount;
			Bond::totalOpeningRisk_XY += risk;
			Bond::totalOpeningLGD_XY += lgd;
		}
		else if(ticker == "TT")
		{
			Bond::totalOpeningAmount_TT += amount;
			Bond::totalOpeningRisk_TT += risk;
			Bond::totalOpeningLGD_TT += lgd;
		}
		else if(ticker == "YU")
		{
			Bond::totalOpeningAmount_YU += amount;
			Bond::totalOpeningRisk_YU += risk;
			Bond::totalOpeningLGD_YU += lgd;
		}
		else
		{
			cout<<"Error: Unidentified ticker '"<<ticker<<"'"<<endl;
		}

		//categorized according to quality
		if(quality == "BB")
		{
			Bond::totalOpeningAmount_BB += amount;
			Bond::totalOpeningRisk_BB += risk;
			Bond::totalOpeningLGD_BB += lgd;

			Bond::totalOpeningAmount_junk += amount;
			Bond::totalOpeningRisk_junk += risk;
			Bond::totalOpeningLGD_junk += lgd;
		}
		else if(quality == "BBB")
		{
			Bond::totalOpeningAmount_BBB += amount;
			Bond::totalOpeningRisk_BBB += risk;
			Bond::totalOpeningLGD_BBB += lgd;

			Bond::totalOpeningAmount_IG += amount;
			Bond::totalOpeningRisk_IG += risk;
			Bond::totalOpeningLGD_IG += lgd;
		}
		else if(quality == "AA")
		{
			Bond::totalOpeningAmount_AA += amount;
			Bond::totalOpeningRisk_AA += risk;
			Bond::totalOpeningLGD_AA += lgd;

			Bond::totalOpeningAmount_IG += amount;
			Bond::totalOpeningRisk_IG += risk;
			Bond::totalOpeningLGD_IG += lgd;
		}
		else if(quality == "AAA")
		{
			Bond::totalOpeningAmount_AAA += amount;
			Bond::totalOpeningRisk_AAA += risk;
			Bond::totalOpeningLGD_AAA += lgd;

			Bond::totalOpeningAmount_IG += amount;
			Bond::totalOpeningRisk_IG += risk;
			Bond::totalOpeningLGD_IG += lgd;
		}
		else if(quality == "A")
		{
			Bond::totalOpeningAmount_A += amount;
			Bond::totalOpeningRisk_A += risk;
			Bond::totalOpeningLGD_A += lgd;

			Bond::totalOpeningAmount_IG += amount;
			Bond::totalOpeningRisk_IG += risk;
			Bond::totalOpeningLGD_IG += lgd;
		}
		else if(quality == "B")
		{
			Bond::totalOpeningAmount_B += amount;
			Bond::totalOpeningRisk_B += risk;
			Bond::totalOpeningLGD_B += lgd;

			Bond::totalOpeningAmount_junk += amount;
			Bond::totalOpeningRisk_junk += risk;
			Bond::totalOpeningLGD_junk += lgd;
		}
		else if(quality == "C")
		{
			Bond::totalOpeningAmount_C += amount;
			Bond::totalOpeningRisk_C += risk;
			Bond::totalOpeningLGD_C += lgd;

			Bond::totalOpeningAmount_junk += amount;
			Bond::totalOpeningRisk_junk += risk;
			Bond::totalOpeningLGD_junk += lgd;
		}
		else if(quality == "CC")
		{
			Bond::totalOpeningAmount_CC += amount;
			Bond::totalOpeningRisk_CC += risk;
			Bond::totalOpeningLGD_CC += lgd;

			Bond::totalOpeningAmount_junk += amount;
			Bond::totalOpeningRisk_junk += risk;
			Bond::totalOpeningLGD_junk += lgd;
		}
		else if(quality == "CCC")
		{
			Bond::totalOpeningAmount_CCC += amount;
			Bond::totalOpeningRisk_CCC += risk;
			Bond::totalOpeningLGD_CCC += lgd;

			Bond::totalOpeningAmount_junk += amount;
			Bond::totalOpeningRisk_junk += risk;
			Bond::totalOpeningLGD_junk += lgd;
		}
		else if(quality == "D")
		{
			Bond::totalOpeningAmount_D += amount;
			Bond::totalOpeningRisk_D += risk;
			Bond::totalOpeningLGD_D += lgd;

			Bond::totalOpeningAmount_junk += amount;
			Bond::totalOpeningRisk_junk += risk;
			Bond::totalOpeningLGD_junk += lgd;
		}
		else
		{
			cout<<"Error: Unidentified quality '"<<quality<<"'"<<endl;
		}
	}

	if(tradingbook == "closing")
	{
		//categorized according to ticker
		if(ticker == "GM")
		{
			Bond::totalClosingAmount_GM += amount;
			Bond::totalClosingRisk_GM += risk;
			Bond::totalClosingLGD_GM += lgd;
		}
		else if(ticker == "FG")
		{
			Bond::totalClosingAmount_FG += amount;
			Bond::totalClosingRisk_FG += risk;
			Bond::totalClosingLGD_FG += lgd;
		}
		else if(ticker == "XY")
		{
			Bond::totalClosingAmount_XY += amount;
			Bond::totalClosingRisk_XY += risk;
			Bond::totalClosingLGD_XY += lgd;
		}
		else if(ticker == "TT")
		{
			Bond::totalClosingAmount_TT += amount;
			Bond::totalClosingRisk_TT += risk;
			Bond::totalClosingLGD_TT += lgd;
		}
		else if(ticker == "YU")
		{
			Bond::totalClosingAmount_YU += amount;
			Bond::totalClosingRisk_YU += risk;
			Bond::totalClosingLGD_YU += lgd;
		}
		else
		{
			cout<<"Error: Unidentified ticker '"<<ticker<<"'"<<endl;
		}

		//categorized according to quality
		if(quality == "BB")
		{
			Bond::totalClosingAmount_BB += amount;
			Bond::totalClosingRisk_BB += risk;
			Bond::totalClosingLGD_BB += lgd;

			Bond::totalClosingAmount_junk += amount;
			Bond::totalClosingRisk_junk += risk;
			Bond::totalClosingLGD_junk += lgd;
		}
		else if(quality == "BBB")
		{
			Bond::totalClosingAmount_BBB += amount;
			Bond::totalClosingRisk_BBB += risk;
			Bond::totalClosingLGD_BBB += lgd;

			Bond::totalClosingAmount_IG += amount;
			Bond::totalClosingRisk_IG += risk;
			Bond::totalClosingLGD_IG += lgd;
		}
		else if(quality == "AA")
		{
			Bond::totalClosingAmount_AA += amount;
			Bond::totalClosingRisk_AA += risk;
			Bond::totalClosingLGD_AA += lgd;

			Bond::totalClosingAmount_IG += amount;
			Bond::totalClosingRisk_IG += risk;
			Bond::totalClosingLGD_IG += lgd;
		}
		else if(quality == "B")
		{
			Bond::totalClosingAmount_B += amount;
			Bond::totalClosingRisk_B += risk;
			Bond::totalClosingLGD_B += lgd;

			Bond::totalClosingAmount_junk += amount;
			Bond::totalClosingRisk_junk += risk;
			Bond::totalClosingLGD_junk += lgd;
		}
		else if(quality == "AAA")
		{
			Bond::totalClosingAmount_AAA += amount;
			Bond::totalClosingRisk_AAA += risk;
			Bond::totalClosingLGD_AAA += lgd;

			Bond::totalClosingAmount_IG += amount;
			Bond::totalClosingRisk_IG += risk;
			Bond::totalClosingLGD_IG += lgd;
		}
		else if(quality == "A")
		{
			Bond::totalClosingAmount_A += amount;
			Bond::totalClosingRisk_A += risk;
			Bond::totalClosingLGD_A += lgd;

			Bond::totalClosingAmount_IG += amount;
			Bond::totalClosingRisk_IG += risk;
			Bond::totalClosingLGD_IG += lgd;
		}
		else if(quality == "C")
		{
			Bond::totalClosingAmount_C += amount;
			Bond::totalClosingRisk_C += risk;
			Bond::totalClosingLGD_C += lgd;

			Bond::totalClosingAmount_junk += amount;
			Bond::totalClosingRisk_junk += risk;
			Bond::totalClosingLGD_junk += lgd;
		}
		else if(quality == "CC")
		{
			Bond::totalClosingAmount_CC += amount;
			Bond::totalClosingRisk_CC += risk;
			Bond::totalClosingLGD_CC += lgd;

			Bond::totalClosingAmount_junk += amount;
			Bond::totalClosingRisk_junk += risk;
			Bond::totalClosingLGD_junk += lgd;
		}
		else if(quality == "CCC")
		{
			Bond::totalClosingAmount_CCC += amount;
			Bond::totalClosingRisk_CCC += risk;
			Bond::totalClosingLGD_CCC += lgd;

			Bond::totalClosingAmount_junk += amount;
			Bond::totalClosingRisk_junk += risk;
			Bond::totalClosingLGD_junk += lgd;
		}
		else if(quality == "D")
		{
			Bond::totalClosingAmount_D += amount;
			Bond::totalClosingRisk_D += risk;
			Bond::totalClosingLGD_D += lgd;

			Bond::totalClosingAmount_junk += amount;
			Bond::totalClosingRisk_junk += risk;
			Bond::totalClosingLGD_junk += lgd;
		}
		else
		{
			cout<<"Error: Unidentified quality '"<<quality<<"'"<<endl;
		}
	}
}

const void FileManager::addNotional_Risk_Lgd()
{
	results->push_back(Bond::totalOpeningAmount_GM);
	results->push_back(Bond::totalOpeningRisk_GM);
	results->push_back(Bond::totalOpeningLGD_GM);
	results->push_back(Bond::totalClosingAmount_GM);
	results->push_back(Bond::totalClosingRisk_GM);
	results->push_back(Bond::totalClosingLGD_GM);
	results->push_back(Bond::totalClosingAmount_GM - Bond::totalOpeningAmount_GM);
	results->push_back(Bond::totalClosingRisk_GM - Bond::totalOpeningRisk_GM);
	results->push_back(Bond::totalClosingLGD_GM - Bond::totalOpeningLGD_GM);

	results->push_back(Bond::totalOpeningAmount_FG);
	results->push_back(Bond::totalOpeningRisk_FG);
	results->push_back(Bond::totalOpeningLGD_FG);
	results->push_back(Bond::totalClosingAmount_FG);
	results->push_back(Bond::totalClosingRisk_FG);
	results->push_back(Bond::totalClosingLGD_FG);
	results->push_back(Bond::totalClosingAmount_FG - Bond::totalOpeningAmount_FG);
	results->push_back(Bond::totalClosingRisk_FG - Bond::totalOpeningRisk_FG);
	results->push_back(Bond::totalClosingLGD_FG - Bond::totalOpeningLGD_FG);

	results->push_back(Bond::totalOpeningAmount_XY);
	results->push_back(Bond::totalOpeningRisk_XY);
	results->push_back(Bond::totalOpeningLGD_XY);
	results->push_back(Bond::totalClosingAmount_XY);
	results->push_back(Bond::totalClosingRisk_XY);
	results->push_back(Bond::totalClosingLGD_XY);
	results->push_back(Bond::totalClosingAmount_XY - Bond::totalOpeningAmount_XY);
	results->push_back(Bond::totalClosingRisk_XY - Bond::totalOpeningRisk_XY);
	results->push_back(Bond::totalClosingLGD_XY - Bond::totalOpeningLGD_XY);

	results->push_back(Bond::totalOpeningAmount_TT);
	results->push_back(Bond::totalOpeningRisk_TT);
	results->push_back(Bond::totalOpeningLGD_TT);
	results->push_back(Bond::totalClosingAmount_TT);
	results->push_back(Bond::totalClosingRisk_TT);
	results->push_back(Bond::totalClosingLGD_TT);
	results->push_back(Bond::totalClosingAmount_TT - Bond::totalOpeningAmount_TT);
	results->push_back(Bond::totalClosingRisk_TT - Bond::totalOpeningRisk_TT);
	results->push_back(Bond::totalClosingLGD_TT - Bond::totalOpeningLGD_TT);

	results->push_back(Bond::totalOpeningAmount_YU);
	results->push_back(Bond::totalOpeningRisk_YU);
	results->push_back(Bond::totalOpeningLGD_YU);
	results->push_back(Bond::totalClosingAmount_YU);
	results->push_back(Bond::totalClosingRisk_YU);
	results->push_back(Bond::totalClosingLGD_YU);
	results->push_back(Bond::totalClosingAmount_YU - Bond::totalOpeningAmount_YU);
	results->push_back(Bond::totalClosingRisk_YU - Bond::totalOpeningRisk_YU);
	results->push_back(Bond::totalClosingLGD_YU - Bond::totalOpeningLGD_YU);

	results->push_back(Bond::totalOpeningAmount_AAA);
	results->push_back(Bond::totalOpeningRisk_AAA);
	results->push_back(Bond::totalOpeningLGD_AAA);
	results->push_back(Bond::totalClosingAmount_AAA);
	results->push_back(Bond::totalClosingRisk_AAA);
	results->push_back(Bond::totalClosingLGD_AAA);
	results->push_back(Bond::totalClosingAmount_AAA - Bond::totalOpeningAmount_AAA);
	results->push_back(Bond::totalClosingRisk_AAA - Bond::totalOpeningRisk_AAA);
	results->push_back(Bond::totalClosingLGD_AAA - Bond::totalOpeningLGD_AAA);

	results->push_back(Bond::totalOpeningAmount_AA);
	results->push_back(Bond::totalOpeningRisk_AA);
	results->push_back(Bond::totalOpeningLGD_AA);
	results->push_back(Bond::totalClosingAmount_AA);
	results->push_back(Bond::totalClosingRisk_AA);
	results->push_back(Bond::totalClosingLGD_AA);
	results->push_back(Bond::totalClosingAmount_AA - Bond::totalOpeningAmount_AA);
	results->push_back(Bond::totalClosingRisk_AA - Bond::totalOpeningRisk_AA);
	results->push_back(Bond::totalClosingLGD_AA - Bond::totalOpeningLGD_AA);

	results->push_back(Bond::totalOpeningAmount_A);
	results->push_back(Bond::totalOpeningRisk_A);
	results->push_back(Bond::totalOpeningLGD_A);
	results->push_back(Bond::totalClosingAmount_A);
	results->push_back(Bond::totalClosingRisk_A);
	results->push_back(Bond::totalClosingLGD_A);
	results->push_back(Bond::totalClosingAmount_A - Bond::totalOpeningAmount_A);
	results->push_back(Bond::totalClosingRisk_A - Bond::totalOpeningRisk_A);
	results->push_back(Bond::totalClosingLGD_A - Bond::totalOpeningLGD_A);

	results->push_back(Bond::totalOpeningAmount_BBB);
	results->push_back(Bond::totalOpeningRisk_BBB);
	results->push_back(Bond::totalOpeningLGD_BBB);
	results->push_back(Bond::totalClosingAmount_BBB);
	results->push_back(Bond::totalClosingRisk_BBB);
	results->push_back(Bond::totalClosingLGD_BBB);
	results->push_back(Bond::totalClosingAmount_BBB - Bond::totalOpeningAmount_BBB);
	results->push_back(Bond::totalClosingRisk_BBB - Bond::totalOpeningRisk_BBB);
	results->push_back(Bond::totalClosingLGD_BBB - Bond::totalOpeningLGD_BBB);

	results->push_back(Bond::totalOpeningAmount_BB);
	results->push_back(Bond::totalOpeningRisk_BB);
	results->push_back(Bond::totalOpeningLGD_BB);
	results->push_back(Bond::totalClosingAmount_BB);
	results->push_back(Bond::totalClosingRisk_BB);
	results->push_back(Bond::totalClosingLGD_BB);
	results->push_back(Bond::totalClosingAmount_BB - Bond::totalOpeningAmount_BB);
	results->push_back(Bond::totalClosingRisk_BB - Bond::totalOpeningRisk_BB);
	results->push_back(Bond::totalClosingLGD_BB - Bond::totalOpeningLGD_BB);

	results->push_back(Bond::totalOpeningAmount_B);
	results->push_back(Bond::totalOpeningRisk_B);
	results->push_back(Bond::totalOpeningLGD_B);
	results->push_back(Bond::totalClosingAmount_B);
	results->push_back(Bond::totalClosingRisk_B);
	results->push_back(Bond::totalClosingLGD_B);
	results->push_back(Bond::totalClosingAmount_B - Bond::totalOpeningAmount_B);
	results->push_back(Bond::totalClosingRisk_B - Bond::totalOpeningRisk_B);
	results->push_back(Bond::totalClosingLGD_B - Bond::totalOpeningLGD_B);

	results->push_back(Bond::totalOpeningAmount_CCC);
	results->push_back(Bond::totalOpeningRisk_CCC);
	results->push_back(Bond::totalOpeningLGD_CCC);
	results->push_back(Bond::totalClosingAmount_CCC);
	results->push_back(Bond::totalClosingRisk_CCC);
	results->push_back(Bond::totalClosingLGD_CCC);
	results->push_back(Bond::totalClosingAmount_CCC - Bond::totalOpeningAmount_CCC);
	results->push_back(Bond::totalClosingRisk_CCC - Bond::totalOpeningRisk_CCC);
	results->push_back(Bond::totalClosingLGD_CCC - Bond::totalOpeningLGD_CCC);

	results->push_back(Bond::totalOpeningAmount_CC);
	results->push_back(Bond::totalOpeningRisk_CC);
	results->push_back(Bond::totalOpeningLGD_CC);
	results->push_back(Bond::totalClosingAmount_CC);
	results->push_back(Bond::totalClosingRisk_CC);
	results->push_back(Bond::totalClosingLGD_CC);
	results->push_back(Bond::totalClosingAmount_CC - Bond::totalOpeningAmount_CC);
	results->push_back(Bond::totalClosingRisk_CC - Bond::totalOpeningRisk_CC);
	results->push_back(Bond::totalClosingLGD_CC - Bond::totalOpeningLGD_CC);

	results->push_back(Bond::totalOpeningAmount_C);
	results->push_back(Bond::totalOpeningRisk_C);
	results->push_back(Bond::totalOpeningLGD_C);
	results->push_back(Bond::totalClosingAmount_C);
	results->push_back(Bond::totalClosingRisk_C);
	results->push_back(Bond::totalClosingLGD_C);
	results->push_back(Bond::totalClosingAmount_C - Bond::totalOpeningAmount_C);
	results->push_back(Bond::totalClosingRisk_C - Bond::totalOpeningRisk_C);
	results->push_back(Bond::totalClosingLGD_C - Bond::totalOpeningLGD_C);

	results->push_back(Bond::totalOpeningAmount_D);
	results->push_back(Bond::totalOpeningRisk_D);
	results->push_back(Bond::totalOpeningLGD_D);
	results->push_back(Bond::totalClosingAmount_D);
	results->push_back(Bond::totalClosingRisk_D);
	results->push_back(Bond::totalClosingLGD_D);
	results->push_back(Bond::totalClosingAmount_D - Bond::totalOpeningAmount_D);
	results->push_back(Bond::totalClosingRisk_D - Bond::totalOpeningRisk_D);
	results->push_back(Bond::totalClosingLGD_D - Bond::totalOpeningLGD_D);

	results->push_back(Bond::totalOpeningAmount_IG);
	results->push_back(Bond::totalOpeningRisk_IG);
	results->push_back(Bond::totalOpeningLGD_IG);
	results->push_back(Bond::totalClosingAmount_IG);
	results->push_back(Bond::totalClosingRisk_IG);
	results->push_back(Bond::totalClosingLGD_IG);

	results->push_back(Bond::totalClosingAmount_IG - Bond::totalOpeningAmount_IG);
	results->push_back(Bond::totalClosingRisk_IG - Bond::totalOpeningRisk_IG);
	results->push_back(Bond::totalClosingLGD_IG - Bond::totalOpeningLGD_IG);

	results->push_back(Bond::totalOpeningAmount_junk);
	results->push_back(Bond::totalOpeningRisk_junk);
	results->push_back(Bond::totalOpeningLGD_junk);
	results->push_back(Bond::totalClosingAmount_junk);
	results->push_back(Bond::totalClosingRisk_junk);
	results->push_back(Bond::totalClosingLGD_junk);
	results->push_back(Bond::totalClosingAmount_junk - Bond::totalOpeningAmount_junk);
	results->push_back(Bond::totalClosingRisk_junk - Bond::totalOpeningRisk_junk);
	results->push_back(Bond::totalClosingLGD_junk - Bond::totalOpeningLGD_junk);
}

/*const void FileManager::addRisk_MarketValue_TwoYrHedge()
{
	//2yr: risk, marketvalue
	//5yr: risk, marketvalue
	//10yr: risk, marketvalue
	//30yr: risk, marketvalue
	//shifted 2Yr hedge for 2yr bucket
	//shifted 2Yr hedge for 5yr bucket
	//shifted 2Yr hedge for 10yr bucket
	//shifted 2Yr hedge for 30yr bucket

	results->push_back(Bond::originalTotalRisk_2yr);
	results->push_back(Bond::originalTotalMarketValue_2yr);
	results->push_back(Bond::originalTotalRisk_5yr);
	results->push_back(Bond::originalTotalMarketValue_5yr);
	results->push_back(Bond::originalTotalRisk_10yr);
	results->push_back(Bond::originalTotalMarketValue_10yr);
	results->push_back(Bond::originalTotalRisk_30yr);
	results->push_back(Bond::originalTotalMarketValue_30yr);

	results->push_back(Bond::originalTotalRisk_2yr / Bond::twoYrDVO1);
	results->push_back(Bond::originalTotalRisk_5yr / Bond::twoYrDVO1);
	results->push_back(Bond::originalTotalRisk_10yr / Bond::twoYrDVO1);
	results->push_back(Bond::originalTotalRisk_30yr / Bond::twoYrDVO1);
}*/

const double* FileManager::returnDynamicResults()
{
	dynamicValues[0] = Bond::originalTotalRisk_2yr;
	dynamicValues[1] = Bond::originalTotalMarketValue_2yr;
	dynamicValues[2] = Bond::originalTotalRisk_5yr;
	dynamicValues[3] = Bond::originalTotalMarketValue_5yr;
	dynamicValues[4] = Bond::originalTotalRisk_10yr;
	dynamicValues[5] = Bond::originalTotalMarketValue_10yr;
	dynamicValues[6] = Bond::originalTotalRisk_30yr;
	dynamicValues[7] = Bond::originalTotalMarketValue_30yr;

	dynamicValues[8] = Bond::originalTotalRisk_2yr / Bond::twoYrDVO1;
	dynamicValues[9] = Bond::originalTotalRisk_5yr / Bond::twoYrDVO1;
	dynamicValues[10] = Bond::originalTotalRisk_10yr / Bond::twoYrDVO1;
	dynamicValues[11] = Bond::originalTotalRisk_30yr / Bond::twoYrDVO1;

	return dynamicValues;
}

//edit this if you need to plot opening book or both
const long* FileManager::returnGraphData()
{
	sort(book_vector.begin(), book_vector.end(), PnL::compareTo_closing);
	double smallestPnl = book_vector.front().combinedPnL_closing;
	//cout<<"smallestPnl: "<< smallestPnl << endl;
	double largestPnl = book_vector.back().combinedPnL_closing;
	//cout<<"largestPnl: "<< largestPnl << endl;
	long range = (largestPnl - smallestPnl) / 6; //perhaps I should use a double?
	//cout<<"range: "<< range << endl;

	long frequency1 = 0;
	long frequency2 = 0;
	long frequency3 = 0;
	long frequency4 = 0;
	long frequency5 = 0;
	long frequency6 = 0;

	
	/*printf("smallestpnl %f largestpnl %f\n", smallestPnl, largestPnl);
	for(unsigned i = 0; i < book_vector.size(); i++)
	{
		printf("book_vector.at %f\n", book_vector.at(i).combinedPnL_closing);
	}*/
	
	for(unsigned i = 0; i < book_vector.size(); i++)
	{
		if(book_vector.at(i).combinedPnL_closing >= smallestPnl && book_vector.at(i).combinedPnL_closing <= smallestPnl + range)
		{
			frequency1++;
		}
		else if(book_vector.at(i).combinedPnL_closing > smallestPnl + range && book_vector.at(i).combinedPnL_closing <= smallestPnl + (range*2))
		{
			frequency2++;
		}
		else if(book_vector.at(i).combinedPnL_closing > smallestPnl + (range*2) && book_vector.at(i).combinedPnL_closing <= smallestPnl + (range*3))
		{
			frequency3++;
		}
		else if(book_vector.at(i).combinedPnL_closing > smallestPnl + (range*3) && book_vector.at(i).combinedPnL_closing <= smallestPnl + (range*4))
		{
			frequency4++;
			//cout<<"Hey, frequency4 = "<< frequency4 <<endl;
		}
		else if(book_vector.at(i).combinedPnL_closing > smallestPnl + (range*4) && book_vector.at(i).combinedPnL_closing <= smallestPnl + (range*5))
		{
			frequency5++;
		}
		else if(book_vector.at(i).combinedPnL_closing > smallestPnl + (range*5) && book_vector.at(i).combinedPnL_closing <= largestPnl)
		{
			frequency6++;
		}
	}

	for(unsigned i = 0; i < 6; i++)
	{
		graphData[i] = (smallestPnl + range * (i+1))/1000;
	}

	graphData[6] = frequency1;
	graphData[7] = frequency2;
	graphData[8] = frequency3;
	graphData[9] = frequency4;
	graphData[10] = frequency5;
	graphData[11] = frequency6;

	/*for(unsigned i = 0; i<12; i++)
	{
		cout<<"graphData[i] = "<< graphData[i] <<endl;
	}*/

	return graphData;
}

const double* FileManager::calculateBumpedMarketValue_Risk_TwoYrHedge(int bump, string bucket)
{
	//find out if the bump should be multiplied by ONE_BP or not

	double totalBumpedMarketValue = 0.0;
	double totalBumpedRisk = 0.0;
	double bumpedBondPrice, dvo1;
	Bond bond; //change this to Bond* bond for speed

	if(bucket == "2_yr")
	{
		bump_2yr += bump;
		//cout<<"bump: "<<bump_2yr<<endl;

		for(unsigned i = 0; i < Bond::allBonds_2yr.size(); i++)
		{
			bond = Bond::allBonds_2yr.at(i);
			//cout<<"rate type: "<<bond.rateType<<endl;

			if(bond.rateType == "YIELD")
			{
				bumpedBondPrice = bond.originalBondPrice;
				dvo1 = bond.dvo1;
			}
			else if(bond.rateType == "SPREAD")
			{
				bumpedBondPrice = calculator.calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump_2yr * ONE_BP));
				dvo1 = calculator.calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump_2yr * ONE_BP), bumpedBondPrice);
				//cout<<"old bond price: "<< bond.originalBondPrice<<endl;
				//cout<<"new bond price: "<< bumpedBondPrice<<endl;
				//cout<<"old dvo1: "<< bond.dvo1<<endl;
				//cout<<"new dvo1: "<< dvo1<<endl;

			}else{cout<< "Error in the dynamic section" << endl;}

			totalBumpedRisk += (dvo1/100 * bond.amount);
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
		//cout<<"total risk: "<< totalBumpedRisk<<endl;
		//cout<<"total market value: "<< totalBumpedMarketValue<<endl;
	}
	else if(bucket == "5_yr")
	{
		bump_5yr += bump;
		for(unsigned i = 0; i < Bond::allBonds_5yr.size(); i++)
		{
			bond = Bond::allBonds_5yr.at(i);

			if(bond.rateType == "YIELD")
			{
				bumpedBondPrice = bond.originalBondPrice;
				dvo1 = bond.dvo1;
			}
			else if(bond.rateType == "SPREAD")
			{
				bumpedBondPrice = calculator.calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump_5yr * ONE_BP));
				dvo1 = calculator.calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump_5yr * ONE_BP), bumpedBondPrice);
			}else{cout<< "Error in the dynamic section" <<  endl;}

			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}
	else if(bucket == "10_yr")
	{
		bump_10yr += bump;
		for(unsigned i = 0; i < Bond::allBonds_10yr.size(); i++)
		{
			bond = Bond::allBonds_10yr.at(i);

			if(bond.rateType == "YIELD")
			{
				bumpedBondPrice = bond.originalBondPrice;
				dvo1 = bond.dvo1;
			}
			else if(bond.rateType == "SPREAD")
			{
				bumpedBondPrice = calculator.calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump_10yr * ONE_BP));
				dvo1 = calculator.calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump_10yr * ONE_BP), bumpedBondPrice);
			}else{cout<< "Error in the dynamic section" << endl;}

			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}
	else if(bucket == "30_yr")
	{
		bump_30yr += bump;
		for(unsigned i = 0; i < Bond::allBonds_30yr.size(); i++)
		{
			bond = Bond::allBonds_30yr.at(i);

			if(bond.rateType == "YIELD")
			{
				bumpedBondPrice = bond.originalBondPrice;
				dvo1 = bond.dvo1;
			}
			else if(bond.rateType == "SPREAD")
			{
				bumpedBondPrice = calculator.calculateBondPrice(0, 0, bond.period, bond.coupon, bond.frequency, bond.rate + (bump_30yr * ONE_BP));
				dvo1 = calculator.calculateDVO1(bond.period, bond.coupon, bond.frequency, bond.rate + (bump_30yr * ONE_BP), bumpedBondPrice);
			}else{cout<< "Error in the dynamic section" << endl;}

			totalBumpedRisk += dvo1/100 * bond.amount;
			totalBumpedMarketValue += bumpedBondPrice/100 * bond.amount;
		}
	}else{cout<< "Error in the dynamic section, bucket name: '" << bucket << "' is not recognized!"<< endl;}

	dynamicResults[0] = totalBumpedRisk;
	dynamicResults[1] = totalBumpedMarketValue;
	dynamicResults[2] = totalBumpedRisk / Bond::twoYrDVO1;

	return dynamicResults;
}

const void FileManager::back_To_Original()
{
	bump_2yr = 0;
	bump_5yr = 0;
	bump_10yr = 0;
	bump_30yr = 0;
}
