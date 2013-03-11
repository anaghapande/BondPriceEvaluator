/*
 * PnL.h
 *
 *  Created on: Dec 3, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#ifndef PNL_H_
#define PNL_H_

class PnL
{
	public:
		PnL();
		virtual ~PnL();

	//private:
		double bondPriceChange_combined;
		double bondPriceChange_rate;
		double bondPriceChange_spread;

		double combinedPnL_opening;
		double spreadPnL_opening;
		double ratePnL_opening;
		double combinedPnL_closing;
		double spreadPnL_closing;
		double ratePnL_closing;

		bool isClosingAccessed;

		static const bool compareTo_opening(PnL pnl1, PnL pnl2);
		static const bool compareTo_closing(PnL pnl1, PnL pnl2);

};

#endif /* PNL_H_ */
