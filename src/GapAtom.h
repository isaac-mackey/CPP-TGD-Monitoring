/*
 * GapAtom.h
 *
 *  Created on: Oct 9, 2020
 *      Author: isaacmackey
 */

#ifndef GAPATOM_H_
#define GAPATOM_H_

#include <string>
#include <map>

using namespace std;

class GapAtom {
public:
	GapAtom(std::string lhs, std::string rhs, std::string direction, std::string units, int gap);

	std::string lhs, rhs, direction, units;
	int gap;

	virtual ~GapAtom();
	void printGapAtom();
	bool checkTruthValueWithAssignment(std::map<string, string> assignment);
};

#endif /* GAPATOM_H_ */
