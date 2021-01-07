/*
 * Rule.h
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#ifndef RULE_H_
#define RULE_H_

#include "ProcessAtom.h"
#include "GapAtom.h"

#include <string>
#include <vector>

using namespace std;

class Rule {
public:

	string ruleName;
	std::vector<ProcessAtom*> bodyProcessAtoms, headProcessAtoms;
	std::vector<GapAtom*> bodyGapAtoms, headGapAtoms;
	std::vector<string> bodyProcessNames, headProcessNames, bodyVariables, headVariables;

	Rule(std::string ruleName,
			std::vector<ProcessAtom*> bodyProcessAtoms,
			std::vector<GapAtom*> bodyGapAtoms,
			std::vector<ProcessAtom*> headProcessAtoms,
			std::vector<GapAtom*> headGapAtoms);

	virtual ~Rule();
	void printRule();
};

#endif /* RULE_H_ */
