/*
 * BondRatings.cc Class to store the LGD mppping for the Quality type S&P/Fitch.
 *                This is a Singleton class, and it uses a map data structure
 *
 *  Created on: Oct 15, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#include "BondRatings.h"


/** Constructor for the BondRatings class. Populates the map elements when called **/

BondRatings::BondRatings()
{
	this->bondRatings["AAA"] = 0;
	this->bondRatings["AA"] = 0;
	this->bondRatings["A"] = 0;
	this->bondRatings["BBB"] = 0;
	this->bondRatings["BB"] = 0.2;
	this->bondRatings["B"] = 0.5;
	this->bondRatings["CCC"] = 0.5;
	this->bondRatings["CC"] = 30/70;
	this->bondRatings["C"] = 0.5;
	this->bondRatings["D"] = 1;
}

BondRatings::~BondRatings(){ }

//BondRatings* BondRatings::bondRatingsInstance = NULL;

/** Static method to return the single BondRatings instance. Calls the constructor
 *  iff  an instance has not yet been generated, and returns only one instance
 *
BondRatings* BondRatings::getInstance()
{
	if (bondRatingsInstance == NULL)
	{
		bondRatingsInstance = new BondRatings();
	}

	return bondRatingsInstance;
}
*/

/** Method to return the LGD for a Quality type
 *  Input parameter is Quality
 *  Returns LGD for the quality specified
 */
const double BondRatings::getLGD(string quality)
{
	return bondRatings.find(quality)->second;
}
