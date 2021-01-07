/*
 * Rule.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: isaacmackey
 */

#include "Rule.h"
#include "ProcessAtom.h"
#include "GapAtom.h"
#include "Constraint.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

Rule::Rule(std::string ruleName,
		std::vector<ProcessAtom*> bodyProcessAtoms,
		std::vector<GapAtom*> bodyGapAtoms,
		std::vector<ProcessAtom*> headProcessAtoms,
		std::vector<GapAtom*> headGapAtoms) {

	this->ruleName = ruleName;
	this->bodyProcessAtoms = bodyProcessAtoms;
	this->bodyGapAtoms= bodyGapAtoms;
	this->headProcessAtoms = headProcessAtoms;
	this->headGapAtoms = headGapAtoms;

	// go through body atoms to collect process names, variables
	std::vector<ProcessAtom*>::iterator atomIter, end;
	for( atomIter = bodyProcessAtoms.begin(), end = bodyProcessAtoms.end(); atomIter != end; ++atomIter){

		// add the process name to the vector of body process names
		bodyProcessNames.push_back((*atomIter)->processName);
		std::vector<string>::iterator variableIter, end2;
		for (variableIter = (*atomIter)->variables.begin(), end2 = (*atomIter)->variables.end(); variableIter != end2; ++variableIter){

			if (std::find(bodyVariables.begin(), bodyVariables.end(), *variableIter) == bodyVariables.end()) {
			  // add the variable to the vector of body variables
			  bodyVariables.push_back(*variableIter);
			}

		}

	}

	// go through head atoms and collect process names, variables
	for( atomIter = headProcessAtoms.begin(), end = headProcessAtoms.end(); atomIter != end; ++atomIter){

		// add the process name to the vector of head process names
		headProcessNames.push_back((*atomIter)->processName);
		std::vector<string>::iterator variableIter, end3;
		for (variableIter = (*atomIter)->variables.begin(), end3 = (*atomIter)->variables.end(); variableIter != end3; ++variableIter){

			if (std::find(headVariables.begin(), headVariables.end(), *variableIter) == headVariables.end()) {
			  // add the variables to the vector of head variables
			  headVariables.push_back(*variableIter);
			}

		}

	}

}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

void Rule::printRule(){

	cout << "Rule: " << this->ruleName << endl;
	cout << "if" << endl;
	cout << endl;
	for(int i = 0; i < bodyProcessAtoms.size(); i++){
		bodyProcessAtoms[i]->printProcessAtom();
		cout << endl;
	}
	for(int i = 0; i < bodyGapAtoms.size(); i++){
		bodyGapAtoms[i]->printGapAtom();
		cout << endl;
	}
	cout << "then" << endl;
	for(int i = 0; i < headProcessAtoms.size(); i++){
		headProcessAtoms[i]->printProcessAtom();
		cout << endl;
	}
	for(int i = 0; i < headGapAtoms.size(); i++){
		headGapAtoms[i]->printGapAtom();
		cout << endl;
	}
	return;
}

