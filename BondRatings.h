/*
 * BondRatings.h Header file for the BondRatings Class
 *
 *  Created on: Oct 15, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#ifndef BONDRATINGS_H_
#define BONDRATINGS_H_
#include <map>
#include <string>

using namespace std;

class BondRatings
{
	public:
		virtual ~BondRatings();
		BondRatings();
		//static BondRatings* getInstance();
		const double getLGD(string rateType);

	private:
		//static BondRatings* bondRatingsInstance;
		map <string, double> bondRatings;

};

#endif /* BONDRATINGS_H_ */
