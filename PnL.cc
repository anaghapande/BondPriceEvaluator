/*
 * PnL.cc
 *
 *  Created on: Dec 3, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 *
 */

#include "PnL.h"

PnL::PnL()
{
	// TODO Auto-generated constructor stub

}

PnL::~PnL()
{
	// TODO Auto-generated destructor stub
}

const bool PnL::compareTo_opening(PnL pnl1, PnL pnl2)
{
	return pnl1.combinedPnL_opening < pnl2.combinedPnL_opening;
}

const bool PnL::compareTo_closing(PnL pnl1, PnL pnl2)
{
	return pnl1.combinedPnL_closing < pnl2.combinedPnL_closing;
}
