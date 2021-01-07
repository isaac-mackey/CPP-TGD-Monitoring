/*
 * Assignment.h
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include "Rule.h"
#include "ProcessAtom.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class Assignment {
public:

	string typeOfAssignment;
	vector<string> variables;
	std::map <string, bool> variablesDefinedFlags;
	std::map <string, string> values;
	int expirationTime;

	std::vector<ProcessAtom*> missingProcessAtoms;
	std::vector<ProcessAtom*> seenProcessAtoms;
	Rule* rulePointer;
	bool complete;
	bool matched;
	vector<Assignment*> matchingAssignments;


	Assignment(vector<string> variables, std::map<string, string> values,
			std::map<string,bool> variablesDefinedFlags, string typeOfAssignment,
			Rule* rulePtr, vector<ProcessAtom*> missingProcessAtoms, vector<ProcessAtom*> seenProcessAtoms,
			vector<Assignment*> matchingAssignments);
	virtual ~Assignment();

	int computeExpirationTime();
	void printAssignment();
};

#endif /* ASSIGNMENT_H_ */
